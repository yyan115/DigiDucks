#pragma once
#include <string>

enum class AssetCategory {
    Scene,
    GameObject,
    Audio,
    Material,
    Texture,
    Shader,
};

class AssetsBrowser {
public:
    static void ShowAssets();
private:
    static void RenderDirectoryTree();
    static void RenderAssetGrid(const std::string& path);
    static void RenderPrefabsGrid();

    static std::string selectedFolderPath;
    static std::string selectedFolderName;
};

