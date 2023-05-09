#pragma once

#include <unordered_map>
#include <set>
#include <unordered_set>

#include "entity.h"
#include "entity_components.h"

struct component_list_t {
    entity_components_t systemType;
    std::unordered_set<unsigned long long> entityIDs;
};

struct ECS {
    std::unordered_map<entity_components_t, component_list_t> componentList;
    // NOTE(Tony): Wasted memory due to bad design choice
    std::unordered_map<unsigned long long, entity_t*> entitiesRegistry;
    unsigned long long nextEntityID;
};

entity_t* ECSEntityAlloc(ECS* self);
void ECSEntityDealloc(ECS* self, unsigned long long id);
void ECSDealloc(ECS* self);
void ECSAdd(ECS* self, unsigned long long id, entity_components_t t, const void* v);
void ECSSetFlag(ECS* self, unsigned long long id, entity_components_t t);
void ECSRemove(ECS* self, unsigned long long id, entity_components_t t);
bool ECSHas(ECS* self, unsigned long long id, entity_components_t t);
void* ECSGet(ECS* self, unsigned long long id, entity_components_t t);