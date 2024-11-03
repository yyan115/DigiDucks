#include "Layer.h"
#include "Entity.h"

void Layer::AddEntity(Entity* entity)
{
    // Check if the entity is already in the list
    if (std::find(entities.begin(), entities.end(), entity) == entities.end())
    {
        entities.push_back(entity);
    }
}


const std::vector<Entity*>& Layer::GetEntities() const
{
    return entities;
}

int Layer::GetOrder() const
{
    return order;
}

void Layer::SetOrder(int _order)
{
    this->order = _order;
}

bool Layer::IsVisible() const
{
    return visible;
}

void Layer::SetVisible(bool _visible)
{
    this->visible = _visible;
}
