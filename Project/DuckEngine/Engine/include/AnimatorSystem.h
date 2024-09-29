#pragma once
#include <unordered_map>
#include "DuckEngine.h"
#include "System.h"

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API AnimatorSystem : public System
{
public:

    void Start() override;

    void Update() override;

private:
    std::unordered_map<AnimatorComponent*, Animation*> activeAnimations;
};