/******************************************************************************/
/*
\file:      LevelManager.cpp
\authors:	Tan Yan Kai, yankai.tan, 2301312 (50%)
			Lucas Yee, l.yee, 2301212 (50%)
\par:	    yankai.tan@digipen.edu
\par:	    l.yee@digipen.edu


\brief:     Contains the definitions that are used to load the level

 Copyright (C) 2024 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior
 written consent of DigiPen Institute of Technology is prohibited.
* /
/******************************************************************************/

#include "LevelManager.h"
#include "Serialization.h"
#include "DuckEngine.h"
#include "Prefab.h"
#include "LevelManager.h"
#include "PrefabManager.h"
#include "ComponentFactory.h"
#include <windows.h>
#include <commdlg.h>
#include <string>
#include <filesystem>

/************************************************************************
@brief Loads a level in the editor from a JSON file.
@param levelFile The path to the JSON file containing level data.
*************************************************************************/
void LevelManager::LoadLevelEditor(const std::string& levelFile)
{
	json levelData = Serialization::LoadJsonFile(levelFile.c_str());

	if (!levelData.empty() && levelData.contains("gameObjects"))
	{
		std::cout << "Successfully loaded level: " << levelFile << std::endl;

		size_t lastSlash = levelFile.find_last_of("\\/");
		std::string sceneName = (lastSlash != std::string::npos)
			? levelFile.substr(lastSlash + 1)
			: levelFile;

		size_t lastDot = sceneName.find_last_of('.');
		if (lastDot != std::string::npos)
		{
			sceneName = sceneName.substr(0, lastDot);
		}


		DuckEngine::DUCKENGINE_SceneManager.SetActiveScene(sceneName);
		if (DuckEngine::DUCKENGINE_SceneManager.GetActiveSceneName() != sceneName)
		{
			LoadLevel(levelFile);
		}
	}
}

/************************************************************************
@brief Loads a level in the game from a JSON file.
@param levelFile The path to the JSON file containing level data.
*************************************************************************/
void LevelManager::LoadLevelGame(const std::string& levelFile)
{
	json levelData = Serialization::LoadJsonFile(levelFile.c_str());

	if (!levelData.empty() && levelData.contains("gameObjects"))
	{
		std::cout << "Successfully loaded level: " << levelFile << std::endl;

		size_t lastSlash = levelFile.find_last_of("\\/");
		std::string sceneName = (lastSlash != std::string::npos)
			? levelFile.substr(lastSlash + 1)
			: levelFile;

		size_t lastDot = sceneName.find_last_of('.');
		if (lastDot != std::string::npos)
		{
			sceneName = sceneName.substr(0, lastDot);
		}

		LoadLevel(levelFile);
	}

}

/************************************************************************
@brief Loads a level from a JSON file and initializes its entities and layers.
@param levelFile The path to the JSON file containing level data.
*************************************************************************/
void LevelManager::LoadLevel(const std::string& levelName) 
{
	nlohmann::json levelData = AssetManager::GetLevelData(levelName);
	if (levelData.empty()) 
	{
		std::cerr << "Failed to load level: " << levelName << std::endl;
		return;
	}

	auto* activeScene = DuckEngine::DUCKENGINE_SceneManager.GetActiveScene();

	// Add layers to the scene
	if (levelData.contains("layers")) 
	{
		for (auto& [layerName, layerData] : levelData["layers"].items()) {
			Layer layer;
			layer.SetOrder(layerData.value("order", 0));
			layer.SetVisible(layerData.value("visible", true));
			activeScene->AddLayer(layerName, layer);
		}
	}

	// Add game objects to the scene
	if (levelData.contains("gameObjects")) 
	{
		for (auto& [gameObjectName, gameObjectData] : levelData["gameObjects"].items()) 
		{
			std::string prefabName = gameObjectData.value("prefab", "");
			std::string layerName = gameObjectData.value("layer", "Gameplay");

			Entity* entity = nullptr;
			if (!prefabName.empty()) 
			{
				auto prefab = PrefabManager::GetPrefab(prefabName.c_str());
				if (prefab) {
					entity = &DuckEngine::DUCKENGINE_EntityManager.CreateEntity();
					entity->name = gameObjectName;
					entity->prefabName = prefabName;
					entity->layerName = layerName;

					ComponentFactory::AddComponentsToEntity(entity, prefab->componentsData);
				}
			}

			else if (gameObjectData.contains("components")) 
			{
				entity = &DuckEngine::DUCKENGINE_EntityManager.CreateEntity();
				entity->name = gameObjectName;
				entity->layerName = layerName;

				ComponentFactory::AddComponentsToEntity(entity, gameObjectData["components"]);
			}

			if (entity) 
			{
				auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity->entityID);
				if (transform && gameObjectData.contains("position")) 
				{
					transform->SetPosition(Serialization::GetVec2(gameObjectData, "position", { 0.f, 0.f }));
				}

				auto* layer = activeScene->GetLayer(layerName);
				if (layer) 
				{
					layer->AddEntity(entity);
				}
			}
		}
	}
}



/****************************************************************
* @brief Load level using data from json file
*
* @param levelFile - json file containing data for the level
*
***************************************************************/
std::string LevelManager::OpenFileDialog(const std::string& fileType) {
	wchar_t fileName[260] = L"";
	wchar_t defExt[10];

	// Set up the OPENFILENAME structure
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	// Select filter and default extension based on fileType
	if (fileType == "scene") {
		ofn.lpstrFilter = L"JSON Files\0*.json\0All Files\0*.*\0";
		wcscpy_s(defExt, L"json");
	}
	else if (fileType == "texture") {
		ofn.lpstrFilter = L"Image Files (.png;*.jpg;*.jpeg)\0*.png;*.jpg;*.jpeg\0All Files\0*.*\0";
		wcscpy_s(defExt, L"png");
	}
	else if (fileType == "audio") {
		ofn.lpstrFilter = L"Audio Files (*.ogg;*.mp3;*.wav)\0*.ogg;*.mp3;*.wav\0All Files\0*.*\0";
		wcscpy_s(defExt, L"mp3");
	}
	else {
		// Default to all files if fileType is unknown
		ofn.lpstrFilter = L"All Files\0*.*\0";
	}
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = 260;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
	ofn.lpstrDefExt = defExt;                  // Use the passed-in default extension

	if (GetOpenFileName(&ofn)) {
		// Convert wide char to narrow char string
		char narrowFileName[260];
		size_t convertedChars = 0;
		wcstombs_s(&convertedChars, narrowFileName, sizeof(narrowFileName), fileName, _TRUNCATE);
		return std::string(narrowFileName);
	}
	return "";
}

/****************************************************************
* @brief Open Level Dialog
*
***************************************************************/
void LevelManager::OpenLevelDialog()
{
	std::string levelFile = OpenFileDialog("scene");

	std::filesystem::path absolutePath = std::filesystem::absolute(levelFile);
	std::cout << "Open File Dialog Path: " << absolutePath << std::endl;

	json levelData = Serialization::LoadJsonFile(levelFile.c_str());
	size_t lastSlash = levelFile.find_last_of("\\/");
	
	std::string sceneName = (lastSlash != std::string::npos)
		? levelFile.substr(lastSlash + 1)
		: levelFile;

	size_t lastDot = sceneName.find_last_of('.');
	if (lastDot != std::string::npos)
	{
		sceneName = sceneName.substr(0, lastDot);
	}

	if (sceneName != DuckEngine::DUCKENGINE_SceneManager.GetActiveSceneName())
	{
		LoadLevelEditor(levelFile);
	}

}

/************************************************************************
@brief Saves any changes made to entities in the active scene.
@param sceneName The name of the scene to save.
*************************************************************************/
void LevelManager::SaveSceneChanges(const std::string& sceneName)
{
	std::string finalPath = "Resources/Scenes/" + sceneName + ".json";
	json sceneData = Serialization::LoadJsonFile(finalPath);

	std::filesystem::path asd = std::filesystem::absolute(finalPath);
	std::cout << "final path: " << asd << std::endl;
	
	// Clear existing gameObjects to avoid duplicate entries
	sceneData["gameObjects"].clear();

	auto& entities = DuckEngine::DUCKENGINE_EntityManager.GetEntities();

	for (auto& entity : entities)
	{
		std::string entityName = entity.name.empty() ? "Entity_" + std::to_string(entity.entityID) : entity.name;
		json& gameObjectData = sceneData["gameObjects"][entityName];

		SaveEntityToJson(&entity, gameObjectData);
	}

	Serialization::SaveJsonFile(finalPath, sceneData);
	std::cout << "Scene changes saved to: " << finalPath << std::endl;
}

/************************************************************************
@brief Saves any changes made to a specific entity in the scene.
@param entityID The ID of the entity.
@param sceneName The name of the scene the entity belongs to.
*************************************************************************/
void LevelManager::SaveEntityChanges(int entityID, std::string& sceneName)
{
	Entity* entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(entityID);
	if (!entity) return;

	std::string finalPath = "Resources/Scenes/" + sceneName + ".json";
	
	json sceneData = Serialization::LoadJsonFile(finalPath);

	std::string entityName = entity->name.empty() ? "Entity_" + std::to_string(entityID) : entity->name;
	json& gameObjectData = sceneData["gameObjects"][entityName];

	SaveEntityToJson(entity, gameObjectData);

	Serialization::SaveJsonFile(finalPath, sceneData);
	std::cout << "Entity changes saved for: " << entityName << " in scene: " << sceneName << std::endl;
}

/************************************************************************
@brief Saves an entity's data to a JSON object.
@param entity The entity to save.
@param gameObjectData The JSON object where the entity's data will be saved.
*************************************************************************/
void LevelManager::SaveEntityToJson(Entity* entity, json& gameObjectData)
{
	if (!entity)
	{
		std::cerr << "Error: Attempted to save a null entity." << std::endl;
		return;
	}

	gameObjectData["layer"] = entity->layerName;

	if (!entity->prefabName.empty())
	{
		gameObjectData["prefab"] = entity->prefabName;

		if (auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entity->entityID))
		{
			gameObjectData["position"]["x"] = transform->GetPosition().x;
			gameObjectData["position"]["y"] = transform->GetPosition().y;
		}

		std::cout << "Saved prefab entity: " << entity->prefabName << " with position ("
			<< gameObjectData["position"]["x"] << ", " << gameObjectData["position"]["y"] << ")" << std::endl;
	}
	else
	{
		ComponentFactory::SaveComponentsToJson(entity->entityID, gameObjectData["components"]);
		std::cout << "Saved non-prefab entity: " << (entity->name.empty() ? "Unnamed Entity" : entity->name) << std::endl;
	}
}


/************************************************************************
@brief Overwrites an existing prefab file with new component values for an entity.
@param entityID The ID of the entity whose prefab data should be updated.
*************************************************************************/
void LevelManager::OverwritePrefab(int entityID)
{
	Entity* entity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(entityID);
	if (!entity)
	{
		std::cerr << "Error: Entity not found with ID " << entityID << std::endl;
		return;
	}

	std::string prefabName = entity->prefabName;
	std::string prefabPath = "Resources/Prefabs/" + prefabName + ".json";

	json prefabData = Serialization::LoadJsonFile(prefabPath);

	if (prefabData.empty())
	{
		std::cerr << "Error: Prefab not found at path " << prefabPath << std::endl;
		return;
	}

	ComponentFactory::SaveComponentsToJson(entityID, prefabData["components"]);

	Serialization::SaveJsonFile(prefabPath, prefabData);

	std::cout << "Prefab '" << prefabName << "' has been updated successfully." << std::endl;
}



bool LevelManager::CreateNewScene(const std::string& sceneName) {
	// Paths for the new files
	std::string jsonPath = "Resources/Scenes/" + sceneName + ".json";

	// Check if JSON file already exists
	if (std::filesystem::exists(jsonPath)) {
		std::cerr << "Error: Scene already exists: " << sceneName << std::endl;
		return false;
	}

	// Create the JSON file
	json sceneData = {
		{ "gameObjects", json::object() },
		{ "layers", { { "Gameplay", { { "order", 1 }, { "visible", true } } } } }
	};
	Serialization::SaveJsonFile(jsonPath, sceneData);

	//TODO: Create a GenericScnee, make it a sharePTr to all newly created scenes
	std::cout << "Successfully created scene JSON: " << sceneName << std::endl;

	return true;
}

nlohmann::json LevelManager::SerializeScene(const std::string& sceneName)
{
	std::string scenePath = "Resources/Scenes/" + sceneName + ".json";
	nlohmann::json sceneData = Serialization::LoadJsonFile(scenePath);

	if (sceneData.empty())
	{
		std::cerr << "Error: Failed to serialize scene: " << sceneName << std::endl;
	}

	return sceneData;
}

void LevelManager::DeserializeScene(const std::string& sceneName, const nlohmann::json& sceneData)
{
	std::string scenePath = "Resources/Scenes/" + sceneName + ".json";
	Serialization::SaveJsonFile(scenePath, sceneData);

	LoadLevelEditor(scenePath);
}

