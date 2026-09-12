#include "PlatformPaths.h"

#include <cstdlib>
#include <system_error>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace
{
    std::filesystem::path GetExecutableDirectory()
    {
#ifdef _WIN32
        std::wstring executablePath(260, L'\0');
        for (;;)
        {
            const DWORD copied = GetModuleFileNameW(
                nullptr, executablePath.data(),
                static_cast<DWORD>(executablePath.size()));
            if (copied == 0)
            {
                return {};
            }
            if (copied < executablePath.size() - 1)
            {
                executablePath.resize(copied);
                return std::filesystem::path(executablePath).parent_path();
            }
            executablePath.resize(executablePath.size() * 2);
        }
#elif defined(__linux__)
        std::error_code error;
        const std::filesystem::path executablePath =
            std::filesystem::read_symlink("/proc/self/exe", error);
        return error ? std::filesystem::path{} : executablePath.parent_path();
#else
        return {};
#endif
    }

    std::filesystem::path EnvironmentPath(const char* variable)
    {
        const char* value = std::getenv(variable);
        return value && *value ? std::filesystem::path(value)
                               : std::filesystem::path{};
    }

    bool ContainsRuntimeResources(const std::filesystem::path& directory)
    {
        std::error_code error;
        return std::filesystem::is_regular_file(
            directory / "Resources/settings.json", error);
    }
}

bool PlatformPaths::UseRuntimeDirectory()
{
    std::error_code error;
    const std::filesystem::path originalDirectory =
        std::filesystem::current_path(error);
    if (!error && ContainsRuntimeResources(originalDirectory))
    {
        return true;
    }

    const std::filesystem::path executableDirectory = GetExecutableDirectory();
    if (executableDirectory.empty())
    {
        return false;
    }

    const std::filesystem::path candidates[] = {
        executableDirectory,
        executableDirectory.parent_path().parent_path(),
    };
    for (const std::filesystem::path& candidate : candidates)
    {
        if (!candidate.empty() && ContainsRuntimeResources(candidate))
        {
            std::filesystem::current_path(candidate, error);
            return !error;
        }
    }
    return false;
}

std::filesystem::path PlatformPaths::GetUserDataDirectory()
{
    std::filesystem::path directory;

#ifdef _WIN32
    directory = EnvironmentPath("LOCALAPPDATA");
    if (!directory.empty())
    {
        directory /= "DigiPen";
        directory /= "Quack Kitchen";
    }
#else
    directory = EnvironmentPath("XDG_DATA_HOME");
    if (directory.empty())
    {
        directory = EnvironmentPath("HOME");
        if (!directory.empty())
        {
            directory /= ".local/share";
        }
    }
    if (!directory.empty())
    {
        directory /= "quack-kitchen";
    }
#endif

    if (directory.empty())
    {
        std::error_code error;
        directory = std::filesystem::temp_directory_path(error) /
                    "quack-kitchen";
    }

    std::error_code error;
    std::filesystem::create_directories(directory, error);
    return error ? std::filesystem::path{} : directory;
}
