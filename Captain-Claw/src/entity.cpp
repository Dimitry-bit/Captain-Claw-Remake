#include "entity.h"
#include "renderer.h"
#include "resource_manager.h"

static void EntityInitRender(entity_t* self, const render_types_t& rType, const string& graphicID = "");

entity_t* EntityAlloc()
{
    printf("[INFO][Entity]: Entity Created.\n");
    entity_t* entity = new entity_t;
    return entity;
}

void EntityDealloc(entity_t** entity)
{
//    if (entity && (*entity)->animator.state != ANIMATOR_STATE_STOPPED) {
//        AnimStop(&(*entity)->animator);
//    }

    delete *entity;
    *entity = nullptr;
}

void EntityInit(entity_t* self, const string& logic, const render_types_t& rType, const string& graphicID)
{
    EntityInitRender(self, rType, graphicID);
    self->logic = logic;
    AnimAnimatorInit(&self->animator, &self->render.sprite);
}

void EntitySet(entity_t* self, const entity_components_t& cType, const void* cValue)
{
    self->type |= cType;

    switch (cType) {
        case C_NONE:break;
        case C_PICKUP: {
            self->pickup = *(const c_pickup_t*) (cValue);
        }
            break;
        case C_CHECKPOINT: {
            self->checkpoint = *(const c_checkpoint_t*) (cValue);
        }
            break;
        case C_ENEMY: {
            self->enemy = *(const c_enemy_t*) (cValue);
        }
            break;
        case C_PLATFORM: {
            self->platform = *(const c_platform_t*) (cValue);
        }
            break;
        case C_SOUND: {
            self->sound = *(const c_sound_t*) (cValue);
        }
            break;
        case C_TILE: {
            self->tile = *(const c_tile_t*) (cValue);
        }
            break;
        case C_RENDER: {
            // TODO(Tony): Move render init in here
        }
            break;
        case C_DAMAGEABLE: {
            self->damageable = *(const c_damageable_t*) (cValue);
        }
            break;
        case C_INVENTORY: {
            self->inventory = *(const c_inventory_t*) (cValue);
        }
        case C_ANIMATOR: {
        }
            break;
        case C_PLAYER: {
            self->player = *(const c_player_t*) (cValue);
        }
            break;
        case C_PHYSICS: {
            self->physics = *(const c_physics_t*) (cValue);
        }
            break;
        case C_COLLIDER: {
            self->collider = *(const c_collider_t*) (cValue);
        }
            break;
    }
}

bool EntityHas(entity_t* self, const entity_components_t& cType)
{
    return (self->type & cType);
}

static void EntityInitRender(entity_t* self, const render_types_t& rType, const string& graphicID)
{
    self->render.type = rType;
    self->render.graphicsID = graphicID;
    if (self->render.type == RENDER_SPRITE && !self->render.graphicsID.empty()) {
        const asset_slot_t* slot = ResGetAssetSlot(ASSET_TEXTURE, self->render.graphicsID.c_str());
        const sf::Texture* tex = slot->texture;
        if (slot->assetTags & ASSET_TAG_ANIMATION) {
            const spriteSheet_t& spriteSheet = ResSpriteSheetGet(slot->header.fileName.c_str());
            tex = &spriteSheet.texture;
            self->render.sprite.setTextureRect(spriteSheet.frames[0].area);
            self->render.sprite.setOrigin(spriteSheet.frames[0].pivot);
            self->render.graphicsID = slot->header.fileName;
        }

        self->render.sprite.setTexture(*tex);
    }
}

void EntityUpdate(entity_t* self, const entity_t* to)
{
    *self = *to;
    EntityInit(self, to->logic, to->render.type, to->render.graphicsID);
}

void EntitySetPos(entity_t* self, float x, float y)
{
    self->render.sprite.setPosition(x, y);
    self->render.rectangleShape.setPosition(x, y);
    self->render.circleShape.setPosition(x, y);
}

void EntitySetPos(entity_t* self, const sf::Vector2f& pos)
{
    EntitySetPos(self, pos.x, pos.y);
}

void EntitySetOrigin(entity_t* self, float x, float y)
{
    self->render.sprite.setOrigin(x, y);
    self->render.rectangleShape.setOrigin(x, y);
    self->render.circleShape.setOrigin(x, y);
}

void EntitySetOrigin(entity_t* self, const sf::Vector2f& origin)
{
    EntitySetOrigin(self, origin.x, origin.y);
}

entity_transform EntityGetTransform(const entity_t* self)
{
    entity_transform transform = {};
    if (self->render.type == RENDER_SPRITE) {
        transform.bounds = self->render.sprite.getGlobalBounds();
        transform.position = self->render.sprite.getPosition();
        transform.origin = self->render.sprite.getOrigin();
    } else if (self->render.type == RENDER_RECTANGLE) {
        transform.bounds = self->render.rectangleShape.getGlobalBounds();
        transform.position = self->render.rectangleShape.getPosition();
        transform.origin = self->render.rectangleShape.getOrigin();
    } else if (self->render.type == RENDER_CIRCLE) {
        transform.bounds = self->render.circleShape.getGlobalBounds();
        transform.position = self->render.circleShape.getPosition();
        transform.origin = self->render.circleShape.getOrigin();
    }

    return transform;
}