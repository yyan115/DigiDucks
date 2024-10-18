/******************************************************************************/
/*!
\file    LoggerManager.h
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the declaration to handle logging of information, warning, error and crash

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

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
#include <exception>
#include <string>

#define DUCKLOG_INFO(message, ...) LoggerManager::GetInstance().LogInfo(message, __VA_ARGS__);
#define DUCKLOG_WARNING(message, ...) LoggerManager::GetInstance().LogWarning(message, __VA_ARGS__);
#define DUCKLOG_ERROR(message, ...) LoggerManager::GetInstance().LogError(message, __VA_ARGS__);
#define DUCKLOG_CRASH(ex) LoggerManager::GetInstance().LogCrash(ex);
#define THROW_EXCEPTION(message) throw DetailedException(message, __FILE__, __func__, __LINE__)

class DetailedException : public std::exception {
public:
    /****************************************************************
    * @brief Constructs a DetailedException with a message, file, function, and line number
    *
    * @param message - The exception message
    * @param file - The file where the exception occurred
    * @param func - The function where the exception occurred
    * @param line - The line number where the exception occurred
    *
    * @return void
    ***************************************************************/
    DUCKENGINE_API DetailedException(const std::string& message, const char* file, const char* func, int line);

    /****************************************************************
    * @brief Returns the exception message
    *
    * @return The exception message 
    ***************************************************************/
    DUCKENGINE_API const char* what() const noexcept override;

    /****************************************************************
    * @brief Returns the file where the exception occurred
    *
    * @return The file string
    ***************************************************************/
    const char* GetFile() const;

    /****************************************************************
    * @brief Returns the function where the exception occurred
    *
    * @return The function string
    ***************************************************************/
    const char* GetFunction() const;

    /****************************************************************
    * @brief Returns the line number where the exception occurred
    *
    * @return The line number as an integer
    ***************************************************************/
    int GetLine() const;

private:
    std::string message_;
    const char* file_;
    const char* func_;
    int line_;
};

class LoggerManager {
public:
    /****************************************************************
    * @brief Retrieves the instance of the LoggerManager
    *
    * @return A reference to the LoggerManager instance
    ***************************************************************/
    DUCKENGINE_API static LoggerManager& GetInstance();

    /****************************************************************
    * @brief Logs an info-level message
    *
    * @param message - The log message
    * @param args - Additional arguments for formatting the message
    *
    * @return void
    ***************************************************************/
    template <typename... Args>
    void LogInfo(const std::string& message, Args&&... args);

    /****************************************************************
    * @brief Logs a warning-level message
    *
    * @param message - The log message
    * @param args - Additional arguments for formatting the message
    *
    * @return void
    ***************************************************************/
    template <typename... Args>
    void LogWarning(const std::string& message, Args&&... args);

    /****************************************************************
    * @brief Logs an error-level message
    *
    * @param message - The log message
    * @param args - Additional arguments for formatting the message
    *
    * @return void
    ***************************************************************/
    template <typename... Args>
    void LogError(const std::string& message, Args&&... args);

    /****************************************************************
    * @brief Logs a crash with detailed exception information
    *
    * @param ex - The detailed exception object
    *
    * @return void
    ***************************************************************/
    DUCKENGINE_API void LogCrash(const DetailedException& ex);

    

private:
    /****************************************************************
    * @brief Constructs the LoggerManager
    ***************************************************************/
    LoggerManager();

    /****************************************************************
    * @brief Destroys the LoggerManager
    * **************************************************************/
    ~LoggerManager();

    /****************************************************************
    * @brief Formats a message string with given arguments
    *
    * @param format - The format string
    * @param args - Additional arguments to format the message
    *
    * @return The formatted string
    ***************************************************************/
    template <typename... Args>
    std::string FormatString(const std::string& format, Args&&... args);

    /****************************************************************
    * @brief Writes a log entry with a specified level and message
    *
    * @param level - The log level (INFO, WARNING, ERROR)
    * @param message - The log message
    *
    * @return void
    ***************************************************************/
    DUCKENGINE_API void WriteLog(const std::string& level, const std::string& message);

    std::ofstream logFile;  // Log file for logging

    /****************************************************************
    * @brief Writes a formatted message to the log file
    *
    * @param message - The log message to write
    *
    * @return void
    ***************************************************************/
    void WriteToFile(const std::string& message);  
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
    WriteLog("ERROR", ss);
}