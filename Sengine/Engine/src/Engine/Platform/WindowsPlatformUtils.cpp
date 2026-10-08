#include "PlatformUtils.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

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
}//namespace Sengine::Platform
