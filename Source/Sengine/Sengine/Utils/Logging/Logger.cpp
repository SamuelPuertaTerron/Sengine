#include "Globals.h"
#include "Logger.h"

namespace Sengine
{
	void Logger::Log(const std::string& message, ELogType type, ELogAreaType areaType)
    {
    	LogInternal(message, type, areaType);

        if (s_Callback)
    		s_Callback(message);
    }

    void Logger::RegisterCallback(const LogCallback& callback)
    {
        s_Callback = callback;
    }

    void Logger::LogInternal(const std::string& message, ELogType type, ELogAreaType areaType)
    {
        std::stringstream logStream;

        logStream << Time::GetCurrentTimeAsString() << " ";

        logStream << GetLogTypePrefix(type) << " ";

        logStream << GetAreaPrefix(areaType) << " " << message << "\n";

        switch (type)
        {   
        case ELogType::None:
            break;
        case ELogType::Debug:
            SetTextColour(36); // Cyan
            break;
        case ELogType::Info:
            SetTextColour(32); // Green
            break;
        case ELogType::Warning:
            SetTextColour(33); // Yellow
            break;
        case ELogType::Error:
            SetTextColour(91); // Bright Red
            break;
        case ELogType::Fatal:
            SetTextColour(31); // Red
            break;
        }
#ifdef SE_DEBUG //Only log  to the console if debug is enable. 
        std::cout << logStream.str();
#endif
        const std::string file = "LogText [" + Time::GetCurrentDateAsString() + "].txt";
        const std::filesystem::path path = "Saved/LogFiles/" + file;

        Filesystem::AppendToFile(path, logStream.str());
        ResetTextColour();
    }

    std::string Logger::GetAreaPrefix(ELogAreaType areaType)
    {
        switch (areaType)
        {
        case ELogAreaType::None:
            return "[None]";
        case ELogAreaType::Engine:
            return "[Engine]";
        case ELogAreaType::Renderer:
            return "[Renderer]";
        case ELogAreaType::Generic:
            return "[Generic]";
        case ELogAreaType::Assert:
            return "[Assert]";
        }

        return "[None]";
    }

    std::string Logger::GetLogTypePrefix(ELogType type)
    {
        switch (type)
        {
        case ELogType::None:
            return "[None]";
        case ELogType::Debug:
            return "[DEBUG]";
        case ELogType::Info:
            return "[INFO]";
        case ELogType::Warning:
            return "[WARNING]";
        case ELogType::Error:
            return "[ERROR]";
        case ELogType::Fatal:
            return "[FATAL]";
        }

        return "[None]";
    }

	void Logger::SetTextColour(int colour)
	{
		std::cout << "\033[" << colour << "m";
	}

	void Logger::ResetTextColour()
	{
		std::cout << "\033[0m";
	}
}