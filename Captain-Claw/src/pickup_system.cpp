#include "pickup_system.h"
#include "animation.h"
#include "resource_manager.h"
#include "asset_constants.h"
#include "sound_sys.h"
#include "ecs.h"
#include "renderer.h"

sf::Sprite glitter;
Animator glitterAnimator;

void PickupUpdate(unsigned long long playerID, std::unordered_set<unsigned long long>& entityIDs, ECS* ecs)
{
    entity_t* claw = ecs->entitiesRegistry[playerID];

    for (auto& eID: entityIDs) {
        c_pickup_t* pickup = (c_pickup_t*) ECSGet(ecs, eID, C_PICKUP);
        c_render_t* render = (c_render_t*) ECSGet(ecs, eID, C_RENDER);

        if (!claw->render.sprite.getGlobalBounds().intersects(render->sprite.getGlobalBounds())) {
            continue;
        }

        switch (pickup->type) {
            case PICKUP_TREASURE:PickupAddGold(*claw, *pickup);
                break;
            case PICKUP_HEALTH:PickupAddHealth(*claw, *pickup);
                break;
            case PICKUP_AMMO:PickupAddAmmo(*claw, *pickup);
                break;
            case PICKUP_LIVE:PickupAddLives(*claw, *pickup);
                break;
            case PICKUP_NONE:break;
        }

        AnimAnimatorInit(&glitterAnimator, &glitter);
        Animation animation_store = AnimAnimationCreate(&ResSpriteSheetGet(VFX_GLITTER), false);
        AnimPlay(&glitterAnimator, &animation_store);

        sf::Vector2f pos = render->sprite.getPosition();
        sf::IntRect texRect = render->sprite.getTextureRect();
        // Center glitter
        glitter.setPosition(pos.x + texRect.width / 2.0f, pos.y + texRect.height / 2.0f);

        ECSEntityDealloc(ecs, eID);
        break;
    }

    if (glitterAnimator.state == ANIMATOR_STATE_PLAYING) {
        rWindow->draw(glitter);
    }
}

void PickupAddGold(entity_t& captain_claw, c_pickup_t& gold)
{
    captain_claw.gold_counter += gold.value;
    const sf::SoundBuffer& sound_gold = ResSoundBuffGet(WAV_GAME_COIN);
    SoundPlay(&sound_gold);
}

void PickupAddHealth(entity_t& captain_claw, c_pickup_t& health)
{
    captain_claw.health += health.value;
    const sf::SoundBuffer& sound_health = ResSoundBuffGet(WAV_GAME_FOODITEM);
    SoundPlay(&sound_health);
}

void PickupAddAmmo(entity_t& captain_claw, c_pickup_t& ammo)
{
    captain_claw.ammo_pistol += ammo.value;
    const sf::SoundBuffer& sound_ammo = ResSoundBuffGet(WAV_GAME_AMMUNITION);
    SoundPlay(&sound_ammo);
}

void PickupAddLives(entity_t& captain_claw, c_pickup_t& live)
{
    captain_claw.lives += live.value;
    const sf::SoundBuffer& sound_live = ResSoundBuffGet(WAV_GAME_EXTRALIFE);
    SoundPlay(&sound_live);
}