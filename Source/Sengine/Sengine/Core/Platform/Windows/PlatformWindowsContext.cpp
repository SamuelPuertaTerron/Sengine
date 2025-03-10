#include "Globals.h"
#include "PlatformWindowsContext.h"

namespace Sengine::Windows
{
	bool PlatformWindowsContext::ShowMessageBox(const std::string& text, const std::string& title, EMessageBoxType type)
	{
		UINT flags = MB_APPLMODAL | MB_SETFOREGROUND;

		switch (type)
		{
		case EMessageBoxType::None:
			break;
		case EMessageBoxType::Info:
			flags |= MB_ICONINFORMATION;
			break;
		case EMessageBoxType::Warning:
			flags |= MB_ICONWARNING;
			break;
		case EMessageBoxType::Error:
			flags |= MB_ICONERROR;
			break;
		}

		flags |= MB_OKCANCEL;
		int result = MessageBox(nullptr, std::wstring(text.begin(), text.end()).c_str(), std::wstring(title.begin(), title.end()).c_str(), flags);

		if (result == IDOK)
		{
			return true;
		}

		return false;
	}

	void PlatformWindowsContext::OpenProcess(const std::string& command)
	{
		const std::wstring commandLine = std::wstring(command.begin(), command.end());

		wchar_t* commandLineCopy = _wcsdup(commandLine.c_str());

		STARTUPINFO info;
		PROCESS_INFORMATION processInfo;
		ZeroMemory(&info, sizeof(info));
		info.cb = sizeof(info);
		ZeroMemory(&processInfo, sizeof(processInfo));

		if (CreateProcess(nullptr, commandLineCopy, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &info, &processInfo))
		{
			WaitForSingleObject(processInfo.hProcess, INFINITE);
			CloseHandle(processInfo.hProcess);
			CloseHandle(processInfo.hThread);
		}
		else
		{
			Logger::Log("Failed to create process " + std::to_string(GetLastError()), ELogType::Error, ELogAreaType::Engine);
		}

		free(commandLineCopy);
	}
}//namespace Sengine::Windows

