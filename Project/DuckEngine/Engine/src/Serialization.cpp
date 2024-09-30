#include "Serialization.h"

json Serialization::jsonData;
WindowInit Serialization::windowInit;

void Serialization::InitJson(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Could not open the file!" << std::endl;
        return;
    }
    file >> jsonData;
    file.close();

    // Extract window initialization data
    windowInit.title = jsonData.value("title", "Untitled Game");
    windowInit.width = jsonData.value("width", 800);
    windowInit.height = jsonData.value("height", 600);
}

json Serialization::LoadJsonFile(const std::string& filePath)
{
    json data;
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Could not open the file: " << filePath << std::endl;
    }
    else
    {
        file >> data;
        file.close();
    }
    return data;
}

Vec2 Serialization::GetVec2(const json& j, const std::string& key, const Vec2& defaultValue)
{
    if (j.contains(key))
    {
        float x = j[key]["x"].get<float>();
        float y = j[key]["y"].get<float>();
        return Vec2(x, y);
    }
    return defaultValue;
}

WindowInit Serialization::GetWindowInit()
{
    return windowInit;
}
