#include "Serialization.h"

struct Serialization::Impl 
{
    json jsonData;
    WindowInit windowInit;
};

// Initialize the static pointer to Impl
Serialization::Impl* Serialization::impl = new Impl();

Serialization::Serialization()
{
    if (impl == nullptr) impl = new Impl();
}

Serialization::~Serialization()
{
    delete impl;
}

void Serialization::InitJson(const char* filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) 
    {
        std::cerr << "Could not open the file!" << std::endl;
        return;
    }
    file >> impl->jsonData;
    file.close();

    // Extract window initialization data
    impl->windowInit.title = impl->jsonData.value("title", "Untitled Game");
    impl->windowInit.width = impl->jsonData.value("width", 800);
    impl->windowInit.height = impl->jsonData.value("height", 600);
}

json Serialization::LoadJsonFile(const char* filePath)
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

Vec2 Serialization::GetVec2(const json& j, const char* key, const Vec2& defaultValue)
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
    return impl->windowInit;
}
