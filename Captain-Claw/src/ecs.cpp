#include "ecs.h"
#include "entity.h"
#include "entity_components.h"

bool IsValidEntityID(ECS* self, unsigned long long id);

bool IsValidEntityID(ECS* self, unsigned long long id)
{
    if (!self->entitiesRegistry.count(id)) {
        printf("[ERROR][ECS]: Entity ID(%llu) is not registered.\n", id);
        return false;
    }

    return true;
}

entity_t* ECSEntityAlloc(ECS* self)
{
    entity_t* e = EntityAlloc();
    e->ID = self->nextEntityID++;
    self->entitiesRegistry[e->ID] = e;
    return e;
}

void ECSAdd(ECS* self, unsigned long long id, entity_components_t t, const void* v)
{
    if (!IsValidEntityID(self, id)) {
        return;
    }

    self->componentList[t].systemType = t;
    self->componentList[t].entityIDs.insert(id);
    EntitySet(self->entitiesRegistry[id], t, v);
}

void ECSDealloc(ECS* self)
{
    for (auto& component: self->componentList) {
        component.second.entityIDs.clear();
    }
    self->componentList.clear();

    for (auto& entity: self->entitiesRegistry) {
        EntityDealloc(&entity.second);
    }
    self->entitiesRegistry.clear();
}

void ECSRemove(ECS* self, unsigned long long id, entity_components_t t)
{
    if (!IsValidEntityID(self, id)) {
        return;
    }

    self->componentList[t].entityIDs.erase(id);
}

bool ECSHas(ECS* self, unsigned long long id, entity_components_t t)
{
    if (!IsValidEntityID(self, id)) {
        return false;
    }

    return (self->componentList[t].entityIDs.count(id));
}

void* ECSGet(ECS* self, unsigned long long id, entity_components_t t)
{
    if (!IsValidEntityID(self, id)) {
        return nullptr;
    }

    entity_t* e = self->entitiesRegistry.at(id);
    switch (t) {
        case C_NONE:return nullptr;
        case C_TILE:return &e->tile;
        case C_PICKUP:return &e->pickup;
        case C_CHECKPOINT:return &e->checkpoint;
        case C_ENEMY:return &e->enemy;
        case C_PLATFORM:return &e->platform;
        case C_SOUND:return &e->sound;
        case C_RENDER:return &e->render;
        case C_DAMAGEABLE:return &e->damageable;
        case C_INVENTORY:return &e->inventory;
    }

    return nullptr;
}

void ECSEntityDealloc(ECS* self, unsigned long long id)
{
    if (!IsValidEntityID(self, id)) {
        return;
    }

    for (auto& component: self->componentList) {
        component.second.entityIDs.erase(id);
    }

    entity_t* e = self->entitiesRegistry.at(id);
    EntityDealloc(&e);
    self->entitiesRegistry.erase(id);
}