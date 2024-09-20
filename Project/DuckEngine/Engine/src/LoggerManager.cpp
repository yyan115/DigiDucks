#include <chrono>
#include <iomanip>
#include <sstream>
#include "LoggerManager.h"
#include "UIDebugConsole.h"

LoggerManager& LoggerManager::GetInstance() {
    static LoggerManager instance;
    return instance;
}

LoggerManager::LoggerManager() {

    logFile.open("game_log.txt", std::ios::out | std::ios::trunc); // Open in truncate mode
    if (!logFile.is_open()) {
        throw std::runtime_error("Failed to open log file.");
    }

    logFile.close();  // Close the file after truncation
    logFile.open("game_log.txt", std::ios::out | std::ios::app);  // Re-open in append mode
}

// destructor
LoggerManager::~LoggerManager() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

// Write log to file and console
void LoggerManager::WriteLog(const std::string& level, const std::string& message) {

    // Get current time
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm localTime;
    localtime_s(&localTime, &now_time_t);

    // Format the log message
    std::stringstream logEntry;
    logEntry << "[" << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S") << "] "
        << "[" << level << "] " << message << "\n";

    // Write log entry to file
    WriteToFile(logEntry.str());

    if (level != "CRASH")
    {
        // Write log to the debug console
        UIDebugConsole::debugConsole.AddLog(level.c_str(), logEntry.str().c_str());
    }
    
}

// Write the log entry to the file
void LoggerManager::WriteToFile(const std::string& message) {
    if (logFile.is_open()) {
        logFile << message;
    }
}
