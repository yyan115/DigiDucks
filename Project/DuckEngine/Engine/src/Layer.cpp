#include "Layer.h"
#include "Entity.h"

void Layer::AddEntity(Entity* entity)
{
    entities.push_back(entity);
}

const std::vector<Entity*>& Layer::GetEntities() const
{
    return entities;
}
