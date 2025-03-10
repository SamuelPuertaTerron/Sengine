/**
 *	Filename: Filesystem.h
 *	Date Created: 26/02/25
 *	Purpose: A simple wrapper around std::filesystem used to create and access files and directories. 
 ***/
#pragma once

namespace Sengine
{
	namespace fs = std::filesystem;

	struct File
	{
		std::string Contents;
		fs::path Path;
	};

	struct Directory
	{
		std::vector<File> Files;
		fs::path Path;
	};

	class Filesystem
	{
	public:
		static File WriteFile(const fs::path& path, const std::string& contents);
		template<typename TData>
		static File WriteFileData(const fs::path& path, const TData& data)
		{

			File file;
			file.Path = path;
			//file.Contents = contents;

			std::ofstream outputFile(path);
			if (!outputFile.is_open())
			{
				Logger::Log("File { " + path.string() + " } could not be created", ELogType::Error, ELogAreaType::Engine);
				return {};
			}

			outputFile << data;

			return file;
		}

		static File WriteFileToDirectory(const Directory& directory, const std::string& filename, const std::string& contents);
		static std::string ReadFileContents(const File& file);
		static std::string ReadFileContents(const fs::path& path);
		static void AppendToFile(File& file, const std::string& contents);
		static File AppendToFile(const fs::path& path, const std::string& contents);
		static bool FileExist(const File& file);
		static void ClearFileText(File& file);

		static Directory WriteDirectory(const fs::path& path);
		static std::vector<File> GetFilesInDirectory(const Directory& directory, bool loadContents = false);
		static bool DirectoryExist(const Directory& directory);
	};
}