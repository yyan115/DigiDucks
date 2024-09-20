#pragma once

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define DUCKLOG_INFO(message, ...) LoggerManager::GetInstance().LogInfo(message, __VA_ARGS__);
#define DUCKLOG_WARNING(message, ...) LoggerManager::GetInstance().LogWarning(message, __VA_ARGS__);
#define DUCKLOG_ERROR(message, ...) LoggerManager::GetInstance().LogError(message, __VA_ARGS__);
#define DUCKLOG_CRASH(ex) LoggerManager::GetInstance().LogCrash(ex, __FILE__, __func__, __LINE__);

class DUCKENGINE_API LoggerManager {
public:
    static LoggerManager& GetInstance();

    // Log messages of different levels
    template <typename... Args>
    void LogInfo(const std::string& message, Args&&... args);

    template <typename... Args>
    void LogWarning(const std::string& message, Args&&... args);

    template <typename... Args>
    void LogError(const std::string& message, Args&&... args);

    // Log crash
    void LogCrash(const char* ex, const char* file, const char* func, int line);

    

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
void LoggerManager::LogError(const std::string& message, Args&&... args) {
    std::string ss = FormatString(message, std::forward<Args>(args)...);
    // Write the log with the additional information
    WriteLog("ERROR", ss);
}

// Log crash 
void LoggerManager::LogCrash(const char* ex, const char* file, const char* func, int line) {
    std::stringstream ss;
    ss << "Exception thrown: " << ex << "\n[FILE " << file << "]" << "\nFunction: " << func << "\nLine : " << line;
    std::cerr << "GAME CRASHED. CHECK GAME_LOG FILE FOR MORE INFO\n" << std::endl;
    WriteLog("CRASH", ss.str());
}