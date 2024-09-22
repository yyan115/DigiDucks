#pragma once

#include <memory>

// Export/Import macro
#ifdef DUCKENGINE_EXPORTS
#define DUCKENGINE_API __declspec(dllexport)
#else
#define DUCKENGINE_API __declspec(dllimport)
#endif

class DUCKENGINE_API Component
{
public:
    virtual ~Component() = default;
    virtual std::shared_ptr<Component> Clone() const = 0;
};
