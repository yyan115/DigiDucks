/******************************************************************************/
/*!
\file       SnapshotManager.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       November 30 2024
\brief      Declares the SnapshotManager class, which provides functionality
            for managing undo and redo operations in the game editor. This
            includes taking and applying scene snapshots, managing undo/redo
            stacks, and clearing history.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <stack>
#include "Serialization.h"

class SnapshotManager
{
public:
    /**************************************************************************
    * @brief Saves the current scene state to the undo stack.
    * @details Clears the redo stack when a new undo state is saved.
    **************************************************************************/
    static void SaveUndoState();   

    /**************************************************************************
    * @brief Reverts the scene to the previous state in the undo stack.
    * @details Pushes the current scene state to the redo stack before applying
    *          the previous state.
    **************************************************************************/
    static void Undo();              

    /**************************************************************************
    * @brief Reapplies the most recently undone state from the redo stack.
    * @details Pushes the current scene state to the undo stack before applying
    *          the next state.
    **************************************************************************/
    static void Redo();           

    /**************************************************************************
    * @brief Clears the undo and redo history stacks.
    **************************************************************************/
    static void ClearHistory();     

    /**************************************************************************
    * @brief Removes the most recent state from the undo stack.
    **************************************************************************/
    static void RemoveLatestUndoState();

private:
    static std::stack<nlohmann::json> undoStack;
    static std::stack<nlohmann::json> redoStack;
    static const size_t MAX_UNDO_STEPS = 30;

    /**************************************************************************
    * @brief Captures the current scene state as a JSON object.
    * @return A JSON object representing the current scene state.
    **************************************************************************/
    static nlohmann::json TakeSceneSnapshot();

    /**************************************************************************
    * @brief Applies a scene snapshot to restore a specific state.
    * @param snapshot The JSON object representing the scene state to apply.
    **************************************************************************/
    static void ApplySceneSnapshot(const nlohmann::json& snapshot); 

};
