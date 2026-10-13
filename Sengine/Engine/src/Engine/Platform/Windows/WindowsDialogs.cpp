#ifdef FE_PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <objbase.h>
#include <shellapi.h>
#include <ShObjIdl.h>
#include <wrl/client.h>

#include "Engine/Platform/Dialogs.h"

#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "Shell32.lib")

namespace Sengine::Platform
{
	namespace fs = std::filesystem;

	namespace
	{
		using Microsoft::WRL::ComPtr; //Releases COM objects automatically.

		//Initialises COM for this thread while in scope. Only uninitialises if this object initialised it.
		class ComScope
		{
		public:
			ComScope() : m_Result(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)) {}
			~ComScope() { if (SUCCEEDED(m_Result)) CoUninitialize(); }

			ComScope(const ComScope&) = delete;
			ComScope& operator=(const ComScope&) = delete;

		private:
			HRESULT m_Result;
		};

		std::wstring Widen(const std::string& text)
		{
			if (text.empty()) return {};

			const int size = MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0);
			std::wstring result(size, L'\0');
			MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), result.data(), size);
			return result;
		}

		//Shell APIs want backslashes and no trailing separator ("D:/Games/" -> "D:\Games").
		std::wstring NativePath(const fs::path& path)
		{
			fs::path native = path.lexically_normal();
			native.make_preferred();

			std::wstring result = native.wstring();
			while (result.size() > 3 && result.back() == L'\\')
				result.pop_back();
			return result;
		}

		void SetStartFolder(IFileDialog* dialog, const fs::path& folder)
		{
			std::error_code error;
			if (folder.empty() || !fs::is_directory(folder, error))
				return;

			ComPtr<IShellItem> item;
			if (SUCCEEDED(SHCreateItemFromParsingName(NativePath(folder).c_str(), nullptr, IID_PPV_ARGS(&item))))
				dialog->SetFolder(item.Get());
		}

		std::optional<fs::path> ShowFileDialog(const CLSID& type, DWORD extraOptions, void* owner,
			const std::vector<FileFilter>& filters, const fs::path& startFolder)
		{
			ComScope com; //Declared first so it is destroyed after the dialog is released.

			ComPtr<IFileDialog> dialog;
			if (FAILED(CoCreateInstance(type, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog))))
				return std::nullopt;

			DWORD options = 0;
			dialog->GetOptions(&options);
			dialog->SetOptions(options | FOS_FORCEFILESYSTEM | extraOptions);

			//COMDLG_FILTERSPEC only holds pointers, so these strings must outlive Show().
			std::vector<std::wstring> names;
			std::vector<std::wstring> patterns;
			for (const FileFilter& filter : filters)
			{
				names.push_back(Widen(filter.Name));
				patterns.push_back(Widen(filter.Pattern));
			}

			std::vector<COMDLG_FILTERSPEC> specs;
			for (size_t i = 0; i < names.size(); ++i)
				specs.push_back({ names[i].c_str(), patterns[i].c_str() });

			if (!specs.empty())
			{
				dialog->SetFileTypes(static_cast<UINT>(specs.size()), specs.data());
				dialog->SetFileTypeIndex(1);

				//"*.scene;*.json" -> "scene": added when the user types a name without an extension.
				const std::wstring& first = patterns.front();
				if (const size_t dot = first.find(L'.'); dot != std::wstring::npos)
				{
					const std::wstring extension = first.substr(dot + 1, first.find(L';') - dot - 1);
					if (!extension.empty() && extension != L"*")
						dialog->SetDefaultExtension(extension.c_str());
				}
			}

			SetStartFolder(dialog.Get(), startFolder);

			if (FAILED(dialog->Show(static_cast<HWND>(owner)))) //Also fails when the user presses Cancel.
				return std::nullopt;

			ComPtr<IShellItem> item;
			if (FAILED(dialog->GetResult(&item)))
				return std::nullopt;

			PWSTR raw = nullptr;
			if (FAILED(item->GetDisplayName(SIGDN_FILESYSPATH, &raw)))
				return std::nullopt;

			fs::path result(raw);
			CoTaskMemFree(raw);
			return result;
		}

		bool ShellOpen(const wchar_t* verb, const std::wstring& file, const std::wstring& arguments = {})
		{
			const HINSTANCE result = ShellExecuteW(nullptr, verb, file.c_str(),
				arguments.empty() ? nullptr : arguments.c_str(), nullptr, SW_SHOWNORMAL);
			return reinterpret_cast<INT_PTR>(result) > 32; //ShellExecute returns a value > 32 on success.
		}
	}//namespace

	void ShowMessageBox(void* owner, const std::string& title, const std::string& text, EMessageBoxType type)
	{
		UINT flags = MB_OK | MB_SETFOREGROUND | (owner ? MB_APPLMODAL : MB_TASKMODAL);
		switch (type)
		{
		case EMessageBoxType::Info:    flags |= MB_ICONINFORMATION; break;
		case EMessageBoxType::Warning: flags |= MB_ICONWARNING; break;
		case EMessageBoxType::Error:   flags |= MB_ICONERROR; break;
		}

		MessageBoxW(static_cast<HWND>(owner), Widen(text).c_str(), Widen(title).c_str(), flags);
	}

	std::optional<fs::path> PickFolder(void* owner, const fs::path& startFolder)
	{
		return ShowFileDialog(CLSID_FileOpenDialog, FOS_PICKFOLDERS | FOS_PATHMUSTEXIST, owner, {}, startFolder);
	}

	std::optional<fs::path> OpenFile(void* owner, const std::vector<FileFilter>& filters, const fs::path& startFolder)
	{
		return ShowFileDialog(CLSID_FileOpenDialog, FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST, owner, filters, startFolder);
	}

	std::optional<fs::path> SaveFile(void* owner, const std::vector<FileFilter>& filters, const fs::path& startFolder)
	{
		return ShowFileDialog(CLSID_FileSaveDialog, FOS_OVERWRITEPROMPT | FOS_PATHMUSTEXIST, owner, filters, startFolder);
	}

	bool OpenInFileExplorer(const fs::path& path)
	{
		std::error_code error;
		if (fs::is_regular_file(path, error))
			return ShellOpen(L"open", L"explorer.exe", L"/select,\"" + NativePath(path) + L"\"");

		return ShellOpen(L"explore", NativePath(path));
	}

	bool OpenWithDefaultApp(const fs::path& path)
	{
		return ShellOpen(L"open", NativePath(path));
	}
}//namespace Sengine::Platform

#endif //FE_PLATFORM_WINDOWS
