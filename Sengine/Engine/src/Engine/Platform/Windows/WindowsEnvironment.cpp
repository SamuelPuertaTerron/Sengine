//Deliberately does NOT include Globals.h: Windows.h and raylib.h can't share a translation unit.
#ifdef FE_PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <cstdlib>
#include <cwchar>

#include "Engine/Platform/Environment.h"

#pragma comment(lib, "Advapi32.lib")

namespace Sengine::Platform
{
	namespace
	{
		//HKEY_CURRENT_USER\Environment holds the per-user variables shown in "Edit environment variables for your account".
		constexpr const wchar_t* kUserEnvironmentKey = L"Environment";

		std::wstring Widen(const std::string& text)
		{
			if (text.empty()) return {};

			const int size = MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0);
			std::wstring result(size, L'\0');
			MultiByteToWideChar(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), result.data(), size);
			return result;
		}

		std::string Narrow(const std::wstring& text)
		{
			if (text.empty()) return {};

			const int size = WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), nullptr, 0, nullptr, nullptr);
			std::string result(size, '\0');
			WideCharToMultiByte(CP_UTF8, 0, text.data(), static_cast<int>(text.size()), result.data(), size, nullptr, nullptr);
			return result;
		}
	}//namespace

	std::optional<std::string> GetPersistentEnvironmentVariable(const std::string& name)
	{
		const std::wstring wideName = Widen(name);

		DWORD size = 0;
		if (RegGetValueW(HKEY_CURRENT_USER, kUserEnvironmentKey, wideName.c_str(), RRF_RT_REG_SZ, nullptr, nullptr, &size) != ERROR_SUCCESS)
		{
			//Not saved for this user: fall back to this process's copy (e.g. a system-wide variable).
			if (const char* value = std::getenv(name.c_str()))
				return std::string(value);
			return std::nullopt;
		}

		std::wstring value(size / sizeof(wchar_t), L'\0');
		if (RegGetValueW(HKEY_CURRENT_USER, kUserEnvironmentKey, wideName.c_str(), RRF_RT_REG_SZ, nullptr, value.data(), &size) != ERROR_SUCCESS)
			return std::nullopt;

		value.resize(std::wcslen(value.c_str())); //Drop the terminating null(s).
		return Narrow(value);
	}

	bool SetPersistentEnvironmentVariable(const std::string& name, const std::string& value)
	{
		const std::wstring wideName = Widen(name);
		const std::wstring wideValue = Widen(value);
		const DWORD bytes = static_cast<DWORD>((wideValue.size() + 1) * sizeof(wchar_t));

		if (RegSetKeyValueW(HKEY_CURRENT_USER, kUserEnvironmentKey, wideName.c_str(), REG_SZ, wideValue.c_str(), bytes) != ERROR_SUCCESS)
			return false;

		//Tell Explorer to reload the environment, so programs started afterwards see the new value.
		//Programs that are already running (Visual Studio, open terminals) keep their old copy until restarted.
		SendMessageTimeoutW(HWND_BROADCAST, WM_SETTINGCHANGE, 0, reinterpret_cast<LPARAM>(kUserEnvironmentKey),
			SMTO_ABORTIFHUNG, 5000, nullptr);

		//Update this process as well (including the CRT copy std::getenv reads), so child processes inherit it.
		_wputenv_s(wideName.c_str(), wideValue.c_str());
		return true;
	}
}//namespace Sengine::Platform

#endif //FE_PLATFORM_WINDOWS
