#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "GameLogic.h"
#include "GameLogicComponent.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class GameLogicManager
{
public:
    static DUCKENGINE_API void AddLogic(const std::string& name, std::shared_ptr<GameLogic> logic);
    static DUCKENGINE_API std::shared_ptr<GameLogic> GetLogic(const std::string& name);
    static void StartAll();
    static void UpdateAll();

private:
    static std::unordered_map<std::string, std::shared_ptr<GameLogic>> logicMap;
};
