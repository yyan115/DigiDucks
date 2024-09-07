#ifndef DUCKENGINE_H
#define DUCKENGINE_H

#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

#include <iostream>

class DUCKENGINE_API DuckEngine
{
public:
    void Start();

    void Initialize();
    void Update();
    void Draw();
    void Exit();

    bool Running();
};

#endif // DUCKENGINE_H
