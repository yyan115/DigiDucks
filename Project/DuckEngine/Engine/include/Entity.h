#pragma once
#include <bitset>
#include <iostream>

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

enum class EntityTag
{
    Undefined = 0,
    Player = 1,
    Wall = 2
};

class DUCKENGINE_API Entity
{
public:
    int entityID;
    EntityTag tag;

    Entity(int id, EntityTag toTag = EntityTag::Undefined) : entityID(id), tag(toTag){}
    
    EntityTag GetTag()
    {
        return tag;
    }

    bool IsTag(EntityTag compareTag)
    {
        return tag == compareTag;
    }


};