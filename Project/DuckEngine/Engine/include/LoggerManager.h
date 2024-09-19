#pragma once

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include <string>
#include <fstream>
#include <sstream>

#define DUCKLOG_INFO(message, ...) LoggerManager::GetInstance().LogInfo(message, __VA_ARGS__);
#define DUCKLOG_WARNING(message, ...) LoggerManager::GetInstance().LogWarning(message, __VA_ARGS__);
#define DUCKLOG_ERROR(ex, message, ...) LoggerManager::GetInstance().LogError(ex, message, __VA_ARGS__);

class DUCKENGINE_API LoggerManager {
public:
    static LoggerManager& GetInstance();

    // Log messages of different levels
    template <typename... Args>
    void LogInfo(const std::string& message, Args&&... args);

    template <typename... Args>
    void LogWarning(const std::string& message, Args&&... args);

    template <typename... Args>
    void LogError(const std::exception& ex,const std::string& message, Args&&... args);

    // Log crash
    void LogCrash(const std::string& crashMessage, const std::string& stackTrace);

    

private:
    LoggerManager();
    ~LoggerManager();

    template <typename... Args>
    std::string FormatString(const std::string& format, Args&&... args);

    // Write a log entry
    void WriteLog(const std::string& level, const std::string& message);

    std::ofstream logFile;  // Log file for logging

    void WriteToFile(const std::string& message);  // Helper to write to the log file
};



// Helper function to format the message
template <typename... Args>
std::string LoggerManager::FormatString(const std::string& format, Args&&... args) {
    std::ostringstream oss;
    oss << format.c_str();
    ((oss << std::forward<Args>(args)), ...);
    return oss.str();
}

// Log info-level messages
template <typename... Args>
void LoggerManager::LogInfo(const std::string& message, Args&&... args) {
    std::string ss = FormatString(message, std::forward<Args>(args)...);
    WriteLog("INFO", ss);
}

// Log warning-level messages
template <typename... Args>
void LoggerManager::LogWarning(const std::string& message, Args&&... args) {
    std::string ss = FormatString(message, std::forward<Args>(args)...);
    WriteLog("WARNING", ss);
}

// Log error-level messages
template <typename... Args>
void LoggerManager::LogError(const std::exception& ex, const std::string& message, Args&&... args) {
    std::string fMessage = FormatString(message, std::forward<Args>(args)...);
    std::stringstream ss;
    ss << "Exception: " << ex.what() << " - " << fMessage;
    WriteLog("ERROR", ss.str());
}