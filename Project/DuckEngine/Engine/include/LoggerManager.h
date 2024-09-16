#pragma once

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include <string>
#include <fstream>

class DUCKENGINE_API LoggerManager {
public:
    static LoggerManager& GetInstance();

    // Log messages of different levels
    void LogInfo(const std::string& message);
    void LogWarning(const std::string& message);
    void LogError(const std::string& message);
    void LogException(const std::exception& ex);

    // Log crash
    void LogCrash(const std::string& crashMessage, const std::string& stackTrace);

private:
    LoggerManager();
    ~LoggerManager();

    // Write a log entry
    void WriteLog(const std::string& level, const std::string& message);

    std::ofstream logFile;  // Log file for logging

    void WriteToFile(const std::string& message);  // Helper to write to the log file
};
