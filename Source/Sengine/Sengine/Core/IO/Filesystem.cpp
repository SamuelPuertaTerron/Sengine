#include "Globals.h"
#include "Filesystem.h"

namespace Sengine
{
	File Filesystem::WriteFile(const fs::path& path, const std::string& contents)
	{
		File file;
		file.Path = path;
		file.Contents = contents;

		std::ofstream outputFile(path);
		if (!outputFile.is_open())
		{
			Logger::Log("File { " + path.string() + " } could not be created", ELogType::Error, ELogAreaType::Engine);
			return {};
		}

		outputFile << contents;

		return file;
	}

	File Filesystem::WriteFileToDirectory(const Directory& directory, const std::string& filename, const std::string& contents)
	{
		File file;
		file.Path = directory.Path / filename;
		file.Contents = contents;

		std::ofstream outputFile(directory.Path);
		if (!outputFile.is_open())
		{
			Logger::Log("File { " + directory.Path.string() + " } could not be created", ELogType::Error, ELogAreaType::Engine);
			return {};
		}
		outputFile << contents;

		return file;
	}

	std::string Filesystem::ReadFileContents(const File& file)
	{
		if (file.Contents.empty())
		{
			std::ifstream inputFile(file.Path);
			if (inputFile)
			{
				return { (std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>() };
			}
		}
		return file.Contents;
	}

	std::string Filesystem::ReadFileContents(const fs::path& path)
	{
		std::ifstream inputFile(path);
		if (inputFile)
		{
			return { (std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>() };
		}

		return EmptyString;
	}

	void Filesystem::AppendToFile(File& file, const std::string& contents)
	{
		if (!FileExist(file))
		{
			file = WriteFile(file.Path, contents);
			return;
		}

		file.Contents += "\n" + contents;

		std::ofstream of(file.Path, std::ios::app);
		of << contents;
	}

	File Filesystem::AppendToFile(const fs::path& path, const std::string& contents)
	{
		File file;
		file.Path = path;
		file.Contents += contents;

		std::ofstream outputFile(path, std::ios::app);
		if (!outputFile.is_open())
		{
			Logger::Log("File { " + path.string() + " } could not be created", ELogType::Error, ELogAreaType::Engine);
			return {};
		}
		outputFile << contents;

		return file;
	}

	bool Filesystem::FileExist(const File& file)
	{
		return fs::exists(file.Path) && fs::is_regular_file(file.Path);
	}

	void Filesystem::ClearFileText(File& file)
	{
		std::ofstream outputFile(file.Path, std::ofstream::out, std::ofstream::trunc);
		outputFile.close();
		file.Contents.clear();
	}

	Directory Filesystem::WriteDirectory(const fs::path& path)
	{
		Directory directory;
		directory.Path = path;

		if (!fs::create_directory(path))
		{
			Logger::Log("Directory { " + path.string() + " } could not be created", ELogType::Error, ELogAreaType::Engine);
			return {};
		}

		return directory;
	}

	std::vector<File> Filesystem::GetFilesInDirectory(const Directory& directory, bool loadContents)
	{
		std::vector<File> files;

		if (!DirectoryExist(directory))
		{
			Logger::Log("Directory { " + directory.Path.string() + " } does not exist", ELogType::Error, ELogAreaType::Engine);
			return files;
		}

		for (const auto& entry : fs::directory_iterator(directory.Path))
		{
			if (fs::is_regular_file(entry))
			{
				File file;
				file.Path = entry.path();

				if (loadContents)
					file.Contents = ReadFileContents(file);

				files.push_back(file);
			}
		}

		return files;
	}

	bool Filesystem::DirectoryExist(const Directory& directory)
	{
		return fs::exists(directory.Path) && fs::is_directory(directory.Path);
	}
}


