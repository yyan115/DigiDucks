#pragma once

#include <filesystem>

namespace PlatformPaths
{
    // Selects a directory containing Resources while preserving a valid
    // working directory supplied by an existing Visual Studio setup.
    bool UseRuntimeDirectory();

    // Returns a writable, per-user directory suitable for saves and settings.
    std::filesystem::path GetUserDataDirectory();
}
