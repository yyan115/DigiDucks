/******************************************************************************/
/*!
\file       GameLogic.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       November 30 2024
\brief      Defines the GameLogic class, an abstract base class for game logic
            components. This class provides an interface for initialization,
            updating, and cloning game logic, while coordinating with the
            GameLogicComponent to manage entity-level behavior.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <memory>
#include "GameLogicComponent.h"

class GameLogic
{
public:
    /************************************************************************
    * @brief Constructor to initialize the game logic with a component
    * @param component Pointer to the associated GameLogicComponent
    *************************************************************************/
    GameLogic(GameLogicComponent* component) : component(component) {}
    /************************************************************************
    * @brief Virtual destructor for safe cleanup
    *************************************************************************/
    virtual ~GameLogic() = default;

    /************************************************************************
    * @brief Pure virtual method for game logic initialization
    *************************************************************************/
    virtual void Start() = 0;

    /************************************************************************
    * @brief Pure virtual method for game logic updates
    *************************************************************************/
    virtual void Update() = 0;

    virtual void FixedUpdate() {}

    /************************************************************************
    * @brief Sets the associated GameLogicComponent
    * @param newComponent Pointer to the new GameLogicComponent
    *************************************************************************/
    void SetComponent(GameLogicComponent* newComponent) 
    {
        component = newComponent;
    }

    /************************************************************************
    * @brief Retrieves the ID of the associated component
    * @return Entity ID of the component, or -1 if no component is set
    *************************************************************************/
    int GetComponentID() const
    {
        return component ? component->GetEntityID() : -1;
    }

    virtual std::shared_ptr<GameLogic> Clone() const = 0;


protected:
    GameLogicComponent* component;
};
