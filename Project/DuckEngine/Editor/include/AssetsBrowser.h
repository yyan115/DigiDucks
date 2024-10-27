#pragma once
#include <string>

enum class AssetCategory {
    Scene,
    GameObject,
    Audio
};

class AssetsBrowser {
public:
    static void ShowAssets();
private:
    static void RenderDirectory(const std::string& path, AssetCategory category);
};

