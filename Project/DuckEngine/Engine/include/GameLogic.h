#pragma once
#include <memory>
#include "GameLogicComponent.h"

class GameLogic
{
public:
    GameLogic(GameLogicComponent* component) : component(component) {}
    virtual ~GameLogic() = default;

    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void FixedUpdate() {}

    void SetComponent(GameLogicComponent* newComponent) 
    {
        component = newComponent;
    }

protected:
    GameLogicComponent* component;
};
