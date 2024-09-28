#pragma once
// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API System {
public:
    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual ~System() = default;
};