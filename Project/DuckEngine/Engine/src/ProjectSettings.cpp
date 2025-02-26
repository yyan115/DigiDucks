#include "ProjectSettings.h"
#include "Serialization.h"
#include <iostream>

DUCKENGINE_API std::string ProjectSettings::startLevel = ProjectSettings::DEFAULT_START_LEVEL;
DUCKENGINE_API bool ProjectSettings::useVSync = true;
int ProjectSettings::windowWidth = ProjectSettings::DEFAULT_WINDOW_WIDTH;
int ProjectSettings::windowHeight = ProjectSettings::DEFAULT_WINDOW_HEIGHT;
int ProjectSettings::targetFPS = ProjectSettings::DEFAULT_TARGET_FPS;

// Volume
DUCKENGINE_API float ProjectSettings::masterVolume = ProjectSettings::DEFAULT_MASTER_VOLUME;
DUCKENGINE_API std::unordered_map<std::string, float> ProjectSettings::volumeCategories;

static float SafeGetFloat(const nlohmann::json& j, const std::string& key, float defaultVal)
{
	if (j.contains(key) && j[key].is_number_float())
	{
		return j[key].get<float>();
	}
	return defaultVal;
}

void ProjectSettings::LoadDefaults()
{
	if (startLevel.empty())
	{
		startLevel = DEFAULT_START_LEVEL;
	}
	if (windowWidth <= 0)
	{
		windowWidth = DEFAULT_WINDOW_WIDTH;
	}
	if (windowHeight <= 0)
	{
		windowHeight = DEFAULT_WINDOW_HEIGHT;
	}
	if (targetFPS <= 0)
	{
		targetFPS = DEFAULT_TARGET_FPS;
	}
	if (masterVolume < 0.0f || masterVolume > 1.0f)
	{
		masterVolume = DEFAULT_MASTER_VOLUME;
	}
}

void ProjectSettings::Load(const std::string& filePath)
{
	nlohmann::json settingsJson = Serialization::LoadJsonFile(filePath);

	if (settingsJson.contains("project"))
	{
		nlohmann::json& projectJson = settingsJson["project"];

		if (projectJson.contains("startLevel"))
		{
			startLevel = projectJson["startLevel"].get<std::string>();
		}

		if (projectJson.contains("windowResolution"))
		{
			auto& resJson = projectJson["windowResolution"];
			windowWidth = resJson.value("width", DEFAULT_WINDOW_WIDTH);
			windowHeight = resJson.value("height", DEFAULT_WINDOW_HEIGHT);
		}

		if (projectJson.contains("useVSync"))
		{
			useVSync = projectJson["useVSync"].get<bool>();
		}

		if (projectJson.contains("targetFPS"))
		{
			targetFPS = projectJson["targetFPS"].get<int>();
		}
	}

	if (settingsJson.contains("volume"))
	{
		nlohmann::json& volumeJson = settingsJson["volume"];

		if (volumeJson.contains("Master"))
		{
			masterVolume = volumeJson["Master"].get<float>();
		}

		if (volumeJson.contains("category") && volumeJson["category"].is_object())
		{
			const auto& catObj = volumeJson["category"];
			for (auto it = catObj.begin(); it != catObj.end(); ++it)
			{
				std::string catName = it.key();
				float catVol = it.value().get<float>();
				volumeCategories[catName] = catVol;
			}
		}
	}

	LoadDefaults();
}

void ProjectSettings::Save(const std::string& filePath)
{
	nlohmann::json settingsJson = Serialization::LoadJsonFile(filePath);

	if (!settingsJson.contains("project"))
	{
		settingsJson["project"] = nlohmann::json::object();
	}
	nlohmann::json& projectJson = settingsJson["project"];

	projectJson["startLevel"] = startLevel;
	projectJson["windowResolution"] =
	{
		{ "width",  windowWidth },
		{ "height", windowHeight }
	};
	projectJson["useVSync"] = useVSync;
	projectJson["targetFPS"] = targetFPS;

	if (!settingsJson.contains("volume"))
	{
		settingsJson["volume"] = nlohmann::json::object();
	}
	nlohmann::json& volJson = settingsJson["volume"];

	volJson["Master"] = masterVolume;

	volJson["category"] = nlohmann::json::object();
	for (auto& kv : volumeCategories)
	{
		volJson["category"][kv.first] = kv.second;
	}

	Serialization::SaveJsonFile(filePath, settingsJson);
}

const std::string& ProjectSettings::GetStartLevel()
{
	return startLevel;
}
void ProjectSettings::SetStartLevel(const std::string& level)
{
	startLevel = level;
}

int ProjectSettings::GetWindowWidth()
{
	return windowWidth;
}
void ProjectSettings::SetWindowWidth(int width)
{
	windowWidth = width;
}

int ProjectSettings::GetWindowHeight()
{
	return windowHeight;
}
void ProjectSettings::SetWindowHeight(int height)
{
	windowHeight = height;
}

int ProjectSettings::GetTargetFPS()
{
	return targetFPS;
}
void ProjectSettings::SetTargetFPS(int fps)
{
	targetFPS = fps;
}

bool ProjectSettings::GetUseVSync()
{
	return useVSync;
}
void ProjectSettings::SetUseVSync(bool enabled)
{
	useVSync = enabled;
}

float ProjectSettings::GetMasterVolume()
{
	return masterVolume;
}
void ProjectSettings::SetMasterVolume(float volume)
{
	masterVolume = volume;
}

float ProjectSettings::GetVolumeCategory(const std::string& category)
{
	auto it = volumeCategories.find(category);
	if (it != volumeCategories.end())
	{
		return it->second;
	}
	return 1.0f;
}

void ProjectSettings::SetVolumeCategory(const std::string& category, float volume)
{
	volumeCategories[category] = volume;
}
