#include "PlatformUtils.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <ShlObj.h>
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Ole32.lib")

namespace Sengine::Platform
{
	ProcessResult RunProcess(const std::string& commandLine, const std::filesystem::path& workingDirectory)
	{
		ProcessResult result;

		//Pipe for capturing the child's stdout/stderr.
		SECURITY_ATTRIBUTES security{ sizeof(security), nullptr, TRUE };
		HANDLE readPipe = nullptr;
		HANDLE writePipe = nullptr;
		if (!CreatePipe(&readPipe, &writePipe, &security, 0))
		{
			result.Output = "CreatePipe failed: " + std::to_string(GetLastError());
			return result;
		}
		SetHandleInformation(readPipe, HANDLE_FLAG_INHERIT, 0); //Only the write end goes to the child.

		STARTUPINFOA startup{};
		startup.cb = sizeof(startup);
		startup.dwFlags = STARTF_USESTDHANDLES;
		startup.hStdOutput = writePipe;
		startup.hStdError = writePipe;
		startup.hStdInput = GetStdHandle(STD_INPUT_HANDLE);

		PROCESS_INFORMATION process{};
		std::string command = commandLine; //CreateProcess may write to this buffer.
		const std::string cwd = workingDirectory.string();

		const BOOL created = CreateProcessA(nullptr, command.data(), nullptr, nullptr, TRUE, CREATE_NO_WINDOW,
			nullptr, cwd.empty() ? nullptr : cwd.c_str(), &startup, &process);

		CloseHandle(writePipe); //Must close our copy, or ReadFile never sees end-of-file.

		if (!created)
		{
			CloseHandle(readPipe);
			result.Output = "CreateProcess failed: " + std::to_string(GetLastError());
			return result;
		}
		result.Started = true;

		char buffer[512];
		DWORD bytesRead = 0;
		while (ReadFile(readPipe, buffer, sizeof(buffer), &bytesRead, nullptr) && bytesRead > 0)
		{
			result.Output.append(buffer, bytesRead);
		}

		WaitForSingleObject(process.hProcess, INFINITE);
		DWORD exitCode = 0;
		GetExitCodeProcess(process.hProcess, &exitCode);
		result.ExitCode = static_cast<int>(exitCode);

		CloseHandle(process.hProcess);
		CloseHandle(process.hThread);
		CloseHandle(readPipe);
		return result;
	}

	std::filesystem::path GetExecutablePath()
	{
		std::wstring buffer(MAX_PATH, L'\0');
		while (true)
		{
			const DWORD length = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
			if (length == 0) return {};
			if (length < buffer.size()) { buffer.resize(length); return buffer; }
			buffer.resize(buffer.size() * 2); //Path longer than MAX_PATH: grow and retry.
		}
	}

	std::filesystem::path GetDocumentsPath()
	{
		//Handles Documents being redirected (e.g. to OneDrive), unlike %USERPROFILE%/Documents.
		PWSTR raw = nullptr;
		std::filesystem::path result;
		if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &raw)))
			result = raw;
		CoTaskMemFree(raw);
		return result;
	}
}//namespace Sengine::Platform
