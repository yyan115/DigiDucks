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

protected:
    GameLogicComponent* component;
};
