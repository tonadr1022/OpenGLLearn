#include "entity_manager.h"

unsigned int EntityManager::createEntity()
{
    int entityId;
    if (freeList.empty())
    {
        entityId = nextEntityId++;
        entities.push_back(entityId);
    }
    else
    {
        int index = freeList.back();
        freeList.pop_back();
        entityId = entities[index];
    }

    sparseArray[entityId] = entities.size() - 1;
    return entityId;
}

void EntityManager::destroyEntity(unsigned int entityId)
{
    if (sparseArray.find(entityId) != sparseArray.end())
    {
        int index = sparseArray[entityId];
        int lastEntityId = entities.back();

        // swap last element of sparse array
        entities[index] = lastEntityId;
        sparseArray[lastEntityId] = index;

        // add index of sparse array to free list
        freeList.push_back(index);

        // remove entity from sparse array and dense array
        sparseArray.erase(entityId);
        entities.pop_back();
    }
}