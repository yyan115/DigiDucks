/******************************************************************************/
/*!
\file       System.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Defines the System class, which serves as an abstract base class
            for all systems in the game engine. Systems have Start and Update
            methods that must be implemented by derived classes.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

/************************************************************************
@brief The System class serves as an abstract base class for all game engine
       systems. It defines two key methods, Start and Update, which must be
       implemented by any derived system.
*************************************************************************/
class DUCKENGINE_API System {
public:
    /************************************************************************
    @brief Pure virtual function that is called to initialize the system.
           This must be implemented by any derived class.
    *************************************************************************/
    virtual void Start() = 0;

    /************************************************************************
    @brief Pure virtual function that is called each frame to update the system.
           This must be implemented by any derived class.
    *************************************************************************/
    virtual void Update() = 0;

    virtual void Render() {}

    /************************************************************************
    @brief Virtual destructor for the System class to ensure derived classes
           can clean up resources when destroyed.
    *************************************************************************/
    virtual ~System() = default;
};