#pragma once

#include <stack>
#include "Serialization.h"

class SnapshotManager
{
public:
    static void SaveUndoState();           
    static void Undo();                       
    static void Redo();                       
    static void ClearHistory();             
    static void RemoveLatestUndoState();

private:
    static nlohmann::json TakeSceneSnapshot();
    static void ApplySceneSnapshot(const nlohmann::json& snapshot); 

    static std::stack<nlohmann::json> undoStack; 
    static std::stack<nlohmann::json> redoStack; 
    static const size_t MAX_UNDO_STEPS = 20;   
};
