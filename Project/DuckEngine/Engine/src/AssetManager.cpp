/******************************************************************************/
/*
\file:      AssetManager.cpp
\authors:	Tan Yan Kai, yankai.tan, 2301312 (40%)
\par:	    yankai.tan@digipen.edu
\author     Muhammad Zikry Bin Zakaria , 2201751 (60%)
\par        muhammadzikry.b@digipen.edu

\brief:     Contains the definitions that are used to manage the assets

 Copyright (C) 2024 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the prior
 written consent of DigiPen Institute of Technology is prohibited.
* /
/******************************************************************************/

#include "AssetManager.h"
#include "ImageLoader.h"
#include "Texture.h"
#include "ShaderManager.h"
#include "FontManager.h"
#include "LevelManager.h"
#include "PrefabManager.h"

#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

std::unordered_map<std::string, std::vector<std::shared_ptr<Texture>>> AssetManager::textureMap;
std::unordered_map<std::string, std::string> AssetManager::nameToFilePath;
std::unordered_map<std::string, FMOD::Sound*> AssetManager::soundMap;
FMOD::System* AssetManager::fmodSystem = nullptr;
std::vector<std::string> AssetManager::fontNames;
std::unordered_map<std::string, nlohmann::json> AssetManager::levelDataMap;

std::string NormalizePath(const std::string& path) {
	std::string normalizedPath = path;
	std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');
	return normalizedPath;
}

void AssetManager::LoadAll()
{
	// Load all textures and sounds from respective directories
	LoadAllTextures("Resources/Sprites");
	LoadAllShaders("Resources/Shaders");
	LoadAllFonts("Resources/Fonts");

	PrefabManager::LoadPrefabsFromDirectory("Resources/Prefabs");
	PreloadScenes("Resources/Scenes");
}

void AssetManager::LoadAllFonts(const std::string& directoryPath) {
	for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
		if (entry.is_regular_file() && entry.path().extension() == ".ttf") {
			std::string fontName = entry.path().stem().string();  // Gets the filename without extension
			std::string fontPath = entry.path().string();
			FontManager::LoadFont(fontName, fontPath, 48);  // Adjust font size as needed
			fontNames.push_back(fontName);
			std::cout << "loaded font: " << fontName << ", path: " << fontPath << "\n";
		}
	}
}

const std::vector<std::string>& AssetManager::GetFontNames() {
	return fontNames;
}

void AssetManager::LoadAllShaders(const std::string& directoryPath) {
	// Load all required shaders

	// Load default shader used for game objects
	ShaderManager::InsertShader("DefaultShader", directoryPath + "/gameVertShader.vert", directoryPath + "/gameFragShader.frag");

	// Load font shader
	ShaderManager::InsertShader("TextShader", directoryPath + "/fontVertShader.vert", directoryPath + "/fontFragShader.frag");

	// Load debug shaders for drawing point, line, rect and circle shapes.
	ShaderManager::InsertShader("PointShader", directoryPath + "/PointVertShader.vert", directoryPath + "/DebugFragShader.frag");
	ShaderManager::InsertShader("LineShader", directoryPath + "/LineVertShader.vert", directoryPath + "/DebugFragShader.frag");
	ShaderManager::InsertShader("RectangleShader", directoryPath + "/RectangleVertShader.vert", directoryPath + "/DebugFragShader.frag");
	ShaderManager::InsertShader("CircleShader", directoryPath + "/CircleVertShader.vert", directoryPath + "/DebugFragShader.frag");
}

void AssetManager::LoadAllTextures(const std::string& directoryPath) {
	// Iterate over texture files in the specified directory
	for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
		if (entry.is_regular_file()) {
			std::string filePath = NormalizePath(entry.path().string());
			std::string fileExtension = entry.path().extension().string();
			// Convert to lowercase for fileExtension
			for (char& c : fileExtension) {
				c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
			}

			// Check for valid image extensions
			if (fileExtension == ".png" || fileExtension == ".jpg" || fileExtension == ".jpeg") {
				if (textureMap.find(filePath) != textureMap.end()) {
					std::cerr << "All textures already loaded.\n";
					return;
				}
				else {
					LoadTexture(filePath);
				}
			}
				
		}
	}
	std::cout << "Loaded all textures in " + directoryPath << std::endl;
}


std::vector<std::shared_ptr<Texture>> AssetManager::LoadTexture(const std::string& filePath)
{
	if (textureMap.find(filePath) != textureMap.end())
	{
		return textureMap[filePath];
	}

	std::vector<std::shared_ptr<Texture>> textures;
	textures.push_back(LoadTextureFromFile(filePath));

	textureMap[filePath] = textures;
	std::string textureName = std::filesystem::path(filePath).stem().string();
	nameToFilePath[textureName] = filePath;
	return textures;
}

std::vector<std::shared_ptr<Texture>> AssetManager::LoadTexture(const std::string& filePath, int textureWidth, int textureHeight)
{
	if (textureMap.find(filePath) != textureMap.end())
	{
		return textureMap[filePath];
	}

	std::vector<std::shared_ptr<Texture>> textures = LoadTextureFromFile(filePath, textureWidth, textureHeight);

	textureMap[filePath] = textures;

	return textures;
}

// Unload a specific texture by its filename
void AssetManager::UnloadTexture(const std::string& fileName) {
	auto it = textureMap.find(fileName);
	if (it != textureMap.end()) {
		it->second.clear();  // Clear texture data
		textureMap.erase(it);  // Remove from textureMap

		for (auto nameIt = nameToFilePath.begin(); nameIt != nameToFilePath.end(); ++nameIt)
		{
			if (nameIt->second == fileName)
			{
				nameToFilePath.erase(nameIt);
				break;
			}
		}
	}
	else {
		std::cout << "Texture not found: " << fileName << std::endl;
	}
}

// Check if a texture with the specified filename is loaded
bool AssetManager::IsTextureLoaded(const std::string& fileName) {
	return textureMap.find(fileName) != textureMap.end();
}

// Reloads a texture from a file path and updates the texture map
void AssetManager::ReloadTexture(const std::string& fileName, const std::string& filePath) {
	if (!IsTextureLoaded(fileName)) {
		textureMap[fileName].clear();
		textureMap[fileName].push_back(LoadTextureFromFile(filePath));
		std::cout << "Texture reloaded: " << fileName << std::endl;
	}
}

// Get a texture by its filename if it's loaded
std::shared_ptr<Texture> AssetManager::GetTexture(const std::string& fileName) {
	if (IsTextureLoaded(fileName)) {
		return textureMap[fileName][0];
	}
	return nullptr;
}

std::shared_ptr<Texture> AssetManager::LoadTextureFromFile(const std::string& filePath)
{
	Texture texture = ImageLoader::LoadTexture(filePath);
	return std::make_shared<Texture>(texture);

}

std::vector<std::shared_ptr<Texture>> AssetManager::LoadTextureFromFile(const std::string& filePath, int textureWidth, int textureHeight)
{
	std::vector<Texture> textures = ImageLoader::LoadSpriteSheet(filePath, textureWidth, textureHeight);
	std::vector<std::shared_ptr<Texture>> texturePtrs;
	texturePtrs.reserve(textures.size());

	for (Texture& texture : textures)
	{
		texturePtrs.push_back(std::make_shared<Texture>(texture));
	}

	return texturePtrs;
}

Texture AssetManager::GetTextureByName(const std::string& textureName) 
{
	auto it = nameToFilePath.find(textureName);
	if (it != nameToFilePath.end())
	{
		const std::string& filePath = it->second;
		if (textureMap.find(filePath) != textureMap.end() && !textureMap[filePath].empty())
		{
			return *textureMap[filePath][0];
		}
	}
	std::cerr << "Error: Texture with name '" << textureName << "' not found." << std::endl;
	return Texture();
}


FMOD::System*& AssetManager::GetFMODSystem() {
	return fmodSystem;
}

// Load all sounds from a specific directory
void AssetManager::LoadAllSounds(const std::string& directoryPath) {
	if (!fmodSystem) {
		std::cerr << "FMOD System not initialized!" << std::endl;
		return;
	}

	// Iterate over sound files in the specified directory
	for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
		if (entry.is_regular_file()) {
			std::string filePath = NormalizePath(entry.path().string());
			std::string fileExtension = entry.path().extension().string();
			// Convert to lowercase for fileExtension
			for (char& c : fileExtension) {
				c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
			}

			// Load the sound if it has a valid audio extension
			if (fileExtension == ".wav" || fileExtension == ".mp3" || fileExtension == ".ogg") {
				if (soundMap.find(filePath) != soundMap.end()) {
					std::cerr << "All sounds already loaded.\n";
					return;
				}
				else {
					LoadSound(filePath, filePath);
					
				}
			}
		}
	}

	std::cout << "Loaded all sounds in " + directoryPath << std::endl;
}

void AssetManager::LoadSound(const std::string& soundID, const std::string& filePath) {
	if (!fmodSystem) {
		std::cerr << "FMOD System not initialized!" << std::endl;
		return;
	}

	if (soundMap.find(soundID) != soundMap.end()) return;  // Already loaded

	FMOD::Sound* sound = nullptr;
	FMOD_RESULT result = fmodSystem->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &sound);
	if (result != FMOD_OK) {
		std::cerr << "Error loading sound: " << filePath << std::endl;
		return;
	}
	soundMap[soundID] = sound;
}

FMOD::Sound* AssetManager::GetSounds(const std::string& soundID) {
	auto it = soundMap.find(soundID);
	return it != soundMap.end() ? it->second : nullptr;
}

void AssetManager::UnloadSound(const std::string& soundID) {
	auto it = soundMap.find(soundID);
	if (it != soundMap.end()) {
		it->second->release(); // Release the FMOD sound
		soundMap.erase(it);
	}
}

void AssetManager::ReloadSound(const std::string& soundID, const std::string& filePath) {
	UnloadSound(soundID); // Unload the existing sound
	LoadSound(soundID, filePath); // Reload the sound
}

void AssetManager::PreloadScenes(const std::string& directoryPath) 
{
	for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) 
	{
		if (entry.is_regular_file() && entry.path().extension() == ".json") 
		{
			std::string levelName = entry.path().stem().string();
			std::string filePath = entry.path().string();
			nlohmann::json levelData = Serialization::LoadJsonFile(filePath.c_str());
			levelDataMap[levelName] = levelData;
			std::cout << "Preloaded level: " << levelName << std::endl;
		}
	}
}

nlohmann::json AssetManager::GetLevelData(const std::string& levelName)
{
	PrefabManager::LoadPrefabsFromDirectory("Resources/Prefabs");

	std::string filePath = "Resources/Scenes/" + levelName + ".json";

	if (std::filesystem::exists(filePath))
	{
		nlohmann::json levelData = Serialization::LoadJsonFile(filePath.c_str());
		if (!levelData.is_null())
		{
			levelDataMap[levelName] = levelData;
			std::cout << "Reloaded level data for: " << levelName << " from file." << std::endl;
			return levelData;
		}
		else
		{
			std::cerr << "Error: Failed to load level data from file: " << filePath << std::endl;
		}
	}
	else
	{
		std::cerr << "Error: Level file not found: " << filePath << std::endl;
	}

	if (levelDataMap.find(levelName) != levelDataMap.end())
	{
		std::cerr << "Returning cached data for: " << levelName << std::endl;
		return levelDataMap[levelName];
	}

	return nlohmann::json();
}


void AssetManager::UnloadAll()
{
	textureMap.clear();
	nameToFilePath.clear();

	// sound
	for (auto& [id, sound] : soundMap) {
		sound->release();
	}
	soundMap.clear();
	if (fmodSystem) {
		fmodSystem->close();
	}
}

std::string AssetManager::GetTexturePath(Texture textureID)
{
	for (const auto& [path, textures] : textureMap)
	{
		if (!textures.empty() && *textures[0] == textureID)
		{
			return path;
		}
	}
	return "";
}

void AssetManager::LoadFont(const std::string& fontName, const std::string& filePath) {
	if (std::find(fontNames.begin(), fontNames.end(), fontName) != fontNames.end()) {
		std::cerr << "Font already loaded: " << fontName << std::endl;
		return;
	}

	FontManager::LoadFont(fontName, filePath, 48);  // Adjust font size as needed
	fontNames.push_back(fontName);
	std::cout << "Loaded font: " << fontName << " from " << filePath << std::endl;
}

void AssetManager::UnloadFont(const std::string& fontName) {
	auto it = std::find(fontNames.begin(), fontNames.end(), fontName);
	if (it != fontNames.end()) {
		fontNames.erase(it);
		std::cout << "Unloaded font: " << fontName << std::endl;
	}
	else {
		std::cerr << "Font not found: " << fontName << std::endl;
	}
}

void AssetManager::LoadShader(const std::string& shaderName, const std::string& filePath) {
	std::string extension = fs::path(filePath).extension().string();
	std::transform(
		extension.begin(),
		extension.end(),
		extension.begin(),
		[](unsigned char c) { return static_cast<char>(std::tolower(c)); }
	);

	// Check if this is a vertex or fragment shader
	std::string pairedShaderPath;
	std::string pairedExtension = (extension == ".vert") ? ".frag" : ".vert";
	std::string pairedShaderName = shaderName;

	for (const auto& entry : fs::directory_iterator(fs::path(filePath).parent_path())) {
		if (entry.path().extension() == pairedExtension) {
			pairedShaderPath = entry.path().string();
			break;
		}
	}

	// Load shader only if paired shader is found
	if (!pairedShaderPath.empty()) {
		ShaderManager::InsertShader(shaderName,
			(extension == ".vert") ? filePath : pairedShaderPath,
			(extension == ".frag") ? filePath : pairedShaderPath);
		std::cout << "Loaded Shader: " << shaderName << " (Vertex: " << (extension == ".vert" ? filePath : pairedShaderPath)
			<< ", Fragment: " << (extension == ".frag" ? filePath : pairedShaderPath) << ")\n";
	}
	else {
		std::cerr << "Error: Could not find paired shader for: " << filePath << std::endl;
	}
}

void AssetManager::UnloadShader(const std::string& shaderName) {
	if (ShaderManager::GetShader(shaderName)) {
		ShaderManager::DeleteShader(shaderName);  // Removes the shader from memory
		std::cout << "Unloaded Shader: " << shaderName << std::endl;
	}
	else {
		std::cerr << "Error: Shader not found: " << shaderName << std::endl;
	}
}


bool AssetManager::AddAsset(const std::string& sourcePath, const std::string& destinationFolder) {
	try {
		// Ensure the destination folder exists
		fs::create_directories(destinationFolder);

		// Construct the destination path
		std::string fileName = fs::path(sourcePath).filename().string();
		std::string destinationPath = destinationFolder + "/" + fileName;

		// Copy the file to the destination
		fs::copy_file(sourcePath, destinationPath, fs::copy_options::overwrite_existing);
		std::cout << "Asset added: " << destinationPath << std::endl;

		// load asset based on file type
		std::string extension = fs::path(destinationPath).extension().string();
		std::transform(
			extension.begin(),
			extension.end(),
			extension.begin(),
			[](unsigned char c) { return static_cast<char>(std::tolower(c)); }
		);


		if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") {
			LoadTexture(destinationPath);
		}
		else if (extension == ".wav" || extension == ".mp3" || extension == ".ogg") {
			LoadSound(destinationPath, destinationPath);
		}
		else if (extension == ".ttf") {
			std::string fontName = fs::path(destinationPath).stem().string();
			LoadFont(fontName, destinationPath);
		}
		else if (extension == ".vert" || extension == ".frag") {
			std::string shaderName = fs::path(destinationPath).stem().string();
			LoadShader(shaderName, destinationPath);
		}
		else {
			std::cerr << "Warning: Unsupported asset type added (" << extension << ")." << std::endl;
		}

		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error adding asset: " << e.what() << std::endl;
		return false;
	}
}


bool AssetManager::RemoveAsset(const std::string& assetPath) {
	try {
		if (!fs::exists(assetPath)) {
			std::cerr << "Error: Asset does not exist: " << assetPath << std::endl;
			return false;
		}

		// Remove asset from memory before deleting file
		std::string extension = fs::path(assetPath).extension().string();
		std::transform(
			extension.begin(),
			extension.end(),
			extension.begin(),
			[](unsigned char c) { return static_cast<char>(std::tolower(c)); }
		);


		if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") {
			UnloadTexture(assetPath);
		}
		else if (extension == ".wav" || extension == ".mp3" || extension == ".ogg") {
			UnloadSound(assetPath);
		}
		else if (extension == ".ttf") {
			std::string fontName = fs::path(assetPath).stem().string();
			UnloadFont(fontName);
		}
		else if (extension == ".vert" || extension == ".frag") {
			std::string shaderName = fs::path(assetPath).stem().string();
			UnloadShader(shaderName);
		}
		else if (extension == ".json") {
			// Assume it's a prefab
			std::string prefabName = fs::path(assetPath).stem().string();
			if (!PrefabManager::RemovePrefab(prefabName)) {
				return false;  // If prefab removal fails, return early
			}
		}

		// Delete the asset from disk
		fs::remove(assetPath);
		std::cout << "Asset removed: " << assetPath << std::endl;

		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error removing asset: " << e.what() << std::endl;
		return false;
	}
}
