#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace Sengine::Platform
{
	enum class EMessageBoxType { Info, Warning, Error };

	struct FileFilter
	{
		std::string Name;    
		std::string Pattern;
	};

	void ShowMessageBox(void* owner, const std::string& title, const std::string& text, EMessageBoxType type = EMessageBoxType::Info);

	std::optional<std::filesystem::path> PickFolder(void* owner, const std::filesystem::path& startFolder = {});
	std::optional<std::filesystem::path> OpenFile(void* owner, const std::vector<FileFilter>& filters = {}, const std::filesystem::path& startFolder = {});
	std::optional<std::filesystem::path> SaveFile(void* owner, const std::vector<FileFilter>& filters = {}, const std::filesystem::path& startFolder = {});

	bool OpenInFileExplorer(const std::filesystem::path& path);

	bool OpenWithDefaultApp(const std::filesystem::path& path);
}//namespace Sengine::Platform
