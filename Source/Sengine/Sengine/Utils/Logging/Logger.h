/**
 *	Filename: Logger.h
 *	Date Created: 26/02/25
 *	Purpose: Allows for logging to the screen and a log file 
 *	Last Modified: N/A
 *	Changes:	   N/A
 *
 ***/
#pragma once

#include "Utils/IO/Filesystem.h"

namespace Sengine
{
    // Where in the code is the log coming from: Generic will default
    enum class ELogAreaType
    {
        None = 0,
        Generic,
        Engine,
        Renderer,
        Assert,
    };

    enum class ELogType
    {
        None = 0,
        Debug,
        Info,
        Warning,
        Error,
        Fatal,
    };

    using LogCallback = std::function<void(const std::string&)>;

    class Logger
    {
    public:
        static void Log(const std::string& message, ELogType type = ELogType::Info, ELogAreaType areaType = ELogAreaType::Generic);
        static void RegisterCallback(const LogCallback& callback);

    private:
        static void LogInternal(const std::string& message, ELogType type, ELogAreaType areaType);
        static std::string GetAreaPrefix(ELogAreaType areaType);
        static std::string GetLogTypePrefix(ELogType type);
        static void SetTextColour(int colour);
        static void ResetTextColour();

        inline static LogCallback s_Callback;
        inline static std::vector<std::string> m_MessageList;
        inline static File m_LogFile;
    };
}


