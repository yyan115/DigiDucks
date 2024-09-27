#pragma once
#include "Component.h"
#include "string"
#include "Texture.h"
#include "ft2build.h"

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

//typedef unsigned int Text;

class DUCKENGINE_API TextRendererComponent : public Component
{
public:
    std::string text = "test";
    std::string fontPath = "../Resources/Roboto-Black.ttf";
    int fontSize = 50;
    std::string texture = "";  // store rendered glyphs as textures
    TextRendererComponent() {};
    TextRendererComponent( std::string& text, std::string& fontPath, int fontSize = 100) // for eg
        : text(text), fontPath(fontPath), fontSize(fontSize) {}

    std::shared_ptr<Component> Clone() const override
    {
        return std::make_shared<TextRendererComponent>(*this); 
    }
};


