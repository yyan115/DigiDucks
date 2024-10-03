/******************************************************************************/
/*!
\file       Component.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Defines the Component class, serving as the base class for all
            game components. Each component must implement a Clone method
            to allow copying of components.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#pragma once

#include <memory>

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

/************************************************************************
@brief Component class serves as the base class for all components in the
       game engine. It provides a virtual destructor and a Clone method
       that must be overridden by derived components to allow deep copying.
*************************************************************************/
class DUCKENGINE_API Component
{
public:
    /************************************************************************
    @brief Destructor for the Component class. Ensures that derived classes
           can clean up resources when a component is destroyed.
    *************************************************************************/
    virtual ~Component() = default;

    /************************************************************************
    @brief Pure virtual method that must be implemented by all derived
           components. It allows creating a copy of the component.
    @return A shared pointer to the cloned component.
    *************************************************************************/
    virtual std::shared_ptr<Component> Clone() const = 0;
};
