#pragma once

#include <stack>
#include "Serialization.h"

class SnapshotManager
{
public:
    static void SaveUndoState();               // Save the current state of the scene
    static void Undo();                        // Undo the last action
    static void Redo();                        // Redo the previously undone action
    static void ClearHistory();                // Clear all undo/redo states

private:
    static nlohmann::json TakeSceneSnapshot(); // Capture the current scene state
    static void ApplySceneSnapshot(const nlohmann::json& snapshot); // Restore scene state

    static std::stack<nlohmann::json> undoStack; // Stores undo states
    static std::stack<nlohmann::json> redoStack; // Stores redo states
    static const size_t MAX_UNDO_STEPS = 20;     // Limit number of undo steps
};
