#include "ProjectSettings.h"
#include "SaveLoadManager.h"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace
{
    bool Expect(bool condition, const char* message)
    {
        if (!condition)
        {
            std::cerr << "FAILED: " << message << std::endl;
        }
        return condition;
    }

    bool NearlyEqual(float left, float right)
    {
        return std::fabs(left - right) < 0.0001f;
    }

    bool WriteText(const std::filesystem::path& path, const std::string& text)
    {
        std::ofstream output(path, std::ios::trunc);
        output << text;
        return output.good();
    }
}

int main()
{
    const std::filesystem::path testDirectory =
        std::filesystem::temp_directory_path() / "quack-kitchen-save-tests";
    const std::filesystem::path savePath = testDirectory / "save.json";
    std::error_code error;
    std::filesystem::create_directories(testDirectory, error);
    if (!Expect(!error, "could not create temporary test directory"))
    {
        return 1;
    }

    SaveLoadManager::SetSavePath(savePath.string());
    ProjectSettings::SetMasterVolume(0.75f);
    ProjectSettings::SetVolumeCategory("BGM", 0.65f);
    ProjectSettings::SetVolumeCategory("SFX", 0.55f);
    ProjectSettings::SetTargetFPS(60);
    ProjectSettings::SetUseVSync(true);

    bool passed = true;

    passed &= Expect(WriteText(savePath, R"({
        "volume": {"master": 0.4, "music": 0.3, "sfx": 0.2},
        "currentLevel": 3,
        "video": {"targetFPS": 120, "useVSync": false}
    })"), "could not write valid save fixture");
    passed &= Expect(SaveLoadManager::LoadGame(), "valid save should load");
    passed &= Expect(NearlyEqual(SaveLoadManager::masterVolume, 0.4f),
        "valid master volume was not loaded");
    passed &= Expect(NearlyEqual(SaveLoadManager::musicVolume, 0.3f),
        "valid music volume was not loaded");
    passed &= Expect(NearlyEqual(SaveLoadManager::sfxVolume, 0.2f),
        "valid SFX volume was not loaded");
    passed &= Expect(SaveLoadManager::currentLevel == 3,
        "valid level progress was not loaded");
    passed &= Expect(SaveLoadManager::targetFPS == 120,
        "valid target FPS was not loaded");
    passed &= Expect(!SaveLoadManager::useVSync,
        "valid VSync setting was not loaded");

    passed &= Expect(WriteText(savePath, R"({
        "volume": {"master": 2.0, "music": -1.0, "sfx": "loud"},
        "currentLevel": 99,
        "video": {"targetFPS": 999, "useVSync": "yes"}
    })"), "could not write invalid-field save fixture");
    passed &= Expect(SaveLoadManager::LoadGame(),
        "well-formed save with invalid fields should be repaired");
    passed &= Expect(NearlyEqual(SaveLoadManager::masterVolume, 1.0f),
        "master volume should be clamped");
    passed &= Expect(NearlyEqual(SaveLoadManager::musicVolume, 0.0f),
        "music volume should be clamped");
    passed &= Expect(NearlyEqual(SaveLoadManager::sfxVolume, 0.55f),
        "invalid SFX volume should use the project default");
    passed &= Expect(SaveLoadManager::currentLevel == 5,
        "level progress should be clamped to the final level");
    passed &= Expect(SaveLoadManager::targetFPS == 240,
        "target FPS should be clamped to the options range");
    passed &= Expect(SaveLoadManager::useVSync,
        "invalid VSync should use the project default");

    passed &= Expect(WriteText(savePath, "{ not valid JSON"),
        "could not write malformed save fixture");
    passed &= Expect(!SaveLoadManager::LoadGame(),
        "malformed JSON should report recovery");
    passed &= Expect(NearlyEqual(SaveLoadManager::masterVolume, 0.75f),
        "malformed save should restore default master volume");
    passed &= Expect(NearlyEqual(SaveLoadManager::musicVolume, 0.65f),
        "malformed save should restore default music volume");
    passed &= Expect(NearlyEqual(SaveLoadManager::sfxVolume, 0.55f),
        "malformed save should restore default SFX volume");
    passed &= Expect(SaveLoadManager::currentLevel == -1,
        "malformed save should restore default progress");
    passed &= Expect(SaveLoadManager::targetFPS == 60,
        "malformed save should restore default target FPS");
    passed &= Expect(SaveLoadManager::useVSync,
        "malformed save should restore default VSync");

    std::filesystem::remove(savePath, error);
    std::filesystem::remove(testDirectory, error);
    return passed ? 0 : 1;
}
