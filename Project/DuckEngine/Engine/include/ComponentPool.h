#pragma once


class ComponentPoolBase {
public:
    virtual ~ComponentPoolBase() = default;

    // Pure virtual function for removing components
    virtual void RemoveComponent(int entityID) = 0;
};

template <typename T>
class ComponentPool : public ComponentPoolBase
{
public:
    std::vector<std::shared_ptr<T>> components;

    std::shared_ptr<T> AddComponent(int entityID, const T& component)
    {
        if (entityID >= components.size())
        {
            components.resize(entityID + 1);
        }
        components[entityID] = std::make_shared<T>(component);
        return components[entityID];
    }

    std::shared_ptr<T> GetComponent(int entityID)
    {
        if (entityID < components.size())
        {
            return components[entityID];
        }
        return nullptr;
    }

    void RemoveComponent(int entityID) override
    {
        if (entityID < components.size())
        {
            components[entityID].reset();
        }
    }
};
