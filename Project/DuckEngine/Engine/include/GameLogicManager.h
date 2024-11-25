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
private:
    static DUCKENGINE_API std::unordered_map<std::string, std::shared_ptr<GameLogic>> logicMap;
    static DUCKENGINE_API std::unordered_map<int, std::vector<std::shared_ptr<GameLogic>>> entityLogicMap;

public:
    static DUCKENGINE_API void AddLogic(const std::string& name, std::shared_ptr<GameLogic> logic);
    static DUCKENGINE_API void AddLogicToEntity(int entityID, std::shared_ptr<GameLogic> logic);

    static DUCKENGINE_API std::shared_ptr<GameLogic> GetLogic(const std::string& name);

    static DUCKENGINE_API std::vector<std::shared_ptr<GameLogic>> GetAllLogicsForEntity(int entityID);

    template <typename T>
    static std::shared_ptr<T> GetLogicForEntity(int entityID)
    {
        static_assert(std::is_base_of<GameLogic, T>::value, "T must inherit from GameLogic");

        if (entityLogicMap.find(entityID) != entityLogicMap.end())
        {
            for (const auto& logic : entityLogicMap[entityID])
            {
                auto typedLogic = std::dynamic_pointer_cast<T>(logic);
                if (typedLogic)
                {
                    return typedLogic;
                }
            }
        }
        return nullptr;
    }

    static DUCKENGINE_API void RemoveLogicsForEntity(int entityID);

    static void StartAll();
    static void UpdateAll();


};
