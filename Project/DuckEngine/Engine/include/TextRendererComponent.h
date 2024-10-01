//#pragma once
//
//// Export/Import macro for DLL
//#ifdef DUCKENGINE_EXPORTS
//#define DUCKENGINE_API __declspec(dllexport)
//#else
//#define DUCKENGINE_API __declspec(dllimport)
//#endif
//
//#include <glm/glm.hpp>
//#include "Component.h"
//#include <string>
//
//// Ensure you have included the GLM library in your project
//// and set up the include directories accordingly.
//
//class DUCKENGINE_API TextRendererComponent : public Component {
//public:
//    std::string text;       // The text to render
//    float positionX;     // Position on the screen
//    float positionY;
//    float scale;     // Scale factor for the text size
//    float colorR;        // Color of the text (RGB)
//    float colorG;        // Color of the text (RGB)
//    float colorB;        // Color of the text (RGB)
//
//    // Constructor with default values
//    TextRendererComponent(const std::string& txt = "",
//        float posX = 0.0f, float posY = 0.0f,
//        float scl = 1.0f,
//        float clrR = 1.0f, float clrG = 1.0f, float clrB = 1.0f)
//        : text(txt), positionX(posX), positionY(posY), scale(scl),
//        colorR(clrR), colorG(clrG), colorB(clrB) {}
//
//    std::shared_ptr<Component> Clone() const override
//    {
//        return std::make_shared<TextRendererComponent>(*this);
//    }
//
//    // Destructor
//    ~TextRendererComponent() {}
//};
//
//
////#pragma once
////#include "Component.h"
////#include "string"
////#include "Texture.h"
////#include "ft2build.h"
////
////// Export/Import macro
////#ifdef DUCKENGINE_EXPORTS
////#define DUCKENGINE_API __declspec(dllexport)
////#else
////#define DUCKENGINE_API __declspec(dllimport)
////#endif
////
//////typedef unsigned int Text;
////
////class DUCKENGINE_API TextRendererComponent : public Component
////{
////public:
////    const char* text = "test";
////    const char* fontPath = "../Resources/Roboto-Black.ttf";
////    int fontSize = 50;
////    const char* texture = "";  // store rendered glyphs as textures
////    TextRendererComponent() {};
////    TextRendererComponent(const char*& text, const char*& fontPath, int fontSize = 100) // for eg
////        : text(text), fontPath(fontPath), fontSize(fontSize) {}
////
////    std::shared_ptr<Component> Clone() const override
////    {
////        return std::make_shared<TextRendererComponent>(*this); 
////    }
////};
////
////
