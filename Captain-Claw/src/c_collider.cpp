#include "c_collider.h"
#include "renderer.h"

void ColliderSync(std::unordered_set<unsigned long long>& entityIDS, ECS* ecs)
{
    for (auto& eID: entityIDS) {
        c_collider_t* collider = (c_collider_t*) ECSGet(ecs, eID, C_COLLIDER);
        sf::Transformable* transform = (sf::Transformable*) ECSGet(ecs, eID, C_TRANSFORM);

        // NOTE(Tony): Find better solution
        if (ECSHas(ecs, eID, C_DAMAGEABLE)) {
            c_damageable_t* damageable = (c_damageable_t*) ECSGet(ecs, eID, C_DAMAGEABLE);
            damageable->swordCollider.transform = *transform;
            damageable->swordCollider.transform.move(damageable->swordCollider.offset);
        }

        collider->transform = *transform;
        collider->transform.move(collider->offset);
    }
}

bool CheckCollision(c_collider_t& a, c_collider_t& b, sf::Vector2i* hitNormal)
{
    sf::Vector2i normal = sf::Vector2i(0, 0);
    if (hitNormal) {
        *hitNormal = normal;
    }

    sf::FloatRect aBounds = ColliderGetGlobalBounds(a);
    sf::FloatRect bBounds = ColliderGetGlobalBounds(b);

    sf::Vector2f aHalfExtent = sf::Vector2f(aBounds.width / 2.0f, aBounds.height / 2.0f);
    sf::Vector2f bHalfExtent = sf::Vector2f(bBounds.width / 2.0f, bBounds.height / 2.0f);

    sf::Vector2f aPos = sf::Vector2f(aBounds.left, aBounds.top);
    sf::Vector2f bPos = sf::Vector2f(bBounds.left, bBounds.top);

    float dX = bPos.x - aPos.x;
    float dY = bPos.y - aPos.y;
    float intersectX = abs((int) dX) - (bHalfExtent.x + aHalfExtent.x);
    float intersectY = abs((int) dY) - (bHalfExtent.y + aHalfExtent.y);

    if (intersectX < 0.0f && intersectY < 0.0f) {
//        inertia = std::clamp(inertia, 0.0f, 1.0f);

        if (intersectX > intersectY) {
            if (dX > 0.0f) {
                normal = sf::Vector2i(-1, 0);
//                a.move(intersectX * (1.0f - inertia), 0.0f);
//                b.move(-intersectX * inertia, 0.0f);
            } else {
                normal = sf::Vector2i(1, 0);
//                a.move(-intersectX * (1.0f - inertia), 0.0f);
//                b.move(intersectX * inertia, 0.0f);
            }
        } else {
            if (dY > 0.0f) {
                normal = sf::Vector2i(0, -1);
//                a.move(0.0f, intersectY * (1.0f - inertia));
//                b.move(0.0f, -intersectY * inertia);
            } else {
                normal = sf::Vector2i(0, 1);
//                a.move(0.0f, -intersectY * (1.0f - inertia));
//                b.move(0.0f, intersectY * inertia);
            }
        }

        if (hitNormal) {
            *hitNormal = normal;
        }

        return true;
    }

    return false;
}

bool CheckCollision(c_collider_t& a, c_collider_t& b, sf::Vector2i* hitNormal,
                    sf::Transformable* tA, sf::Transformable* tB, float inertia)
{
    sf::Vector2i normal = sf::Vector2i(0, 0);
    if (hitNormal) {
        *hitNormal = normal;
    }

    sf::FloatRect aBounds = ColliderGetGlobalBounds(a);
    sf::FloatRect bBounds = ColliderGetGlobalBounds(b);

    sf::Vector2f aHalfExtent = sf::Vector2f(aBounds.width / 2.0f, aBounds.height / 2.0f);
    sf::Vector2f bHalfExtent = sf::Vector2f(bBounds.width / 2.0f, bBounds.height / 2.0f);

    sf::Vector2f aPos = sf::Vector2f(aBounds.left, aBounds.top);
    sf::Vector2f bPos = sf::Vector2f(bBounds.left, bBounds.top);

    float dX = bPos.x - aPos.x;
    float dY = bPos.y - aPos.y;
    float intersectX = abs((int) dX) - (bHalfExtent.x + aHalfExtent.x);
    float intersectY = abs((int) dY) - (bHalfExtent.y + aHalfExtent.y);

    if (intersectX < 0.0f && intersectY < 0.0f) {
        inertia = std::clamp(inertia, 0.0f, 1.0f);

        if (!a.isTrigger && !b.isTrigger) {
            if (intersectX <= intersectY) {
                if (dY > 0.0f) {
                    normal = sf::Vector2i(0, -1);
                    tA->move(0.0f, intersectY * (1.0f - inertia));
                    tB->move(0.0f, -intersectY * inertia);
                } else {
                    normal = sf::Vector2i(0, 1);
                    tA->move(0.0f, -intersectY * (1.0f - inertia));
                    tB->move(0.0f, intersectY * inertia);
                }
            } else {
                if (dX > 0.0f) {
                    normal = sf::Vector2i(-1, 0);
                    tA->move(intersectX * (1.0f - inertia), 0.0f);
                    tB->move(-intersectX * inertia, 0.0f);
                } else {
                    normal = sf::Vector2i(1, 0);
                    tA->move(-intersectX * (1.0f - inertia), 0.0f);
                    tB->move(intersectX * inertia, 0.0f);
                }
            }
        }

        if (hitNormal) {
            *hitNormal = normal;
        }

        return true;
    }

    return false;
}

c_collider_t ColliderCreate(const sf::Vector2f& size, const sf::Vector2f& offset, bool isTrigger)
{
    return c_collider_t{
        .offset = offset,
        .size = size,
        .isTrigger = isTrigger,
    };
}

c_collider_t ColliderCreate(const sf::Sprite& sprite, bool isTrigger)
{
    const sf::IntRect& texRect = sprite.getTextureRect();
    sf::Vector2f size((float) texRect.width, (float) texRect.height);

    return c_collider_t{
        .offset = size / 2.0f,
        .size = size,
        .isTrigger = isTrigger,
    };
}

sf::FloatRect ColliderGetLocalBounds(const c_collider_t& self)
{
    const float width = self.size.x;
    const float height = self.size.y;

    return {0.0f, 0.0f, width, height};
}

sf::FloatRect ColliderGetGlobalBounds(const c_collider_t& self)
{
    sf::FloatRect localBounds = ColliderGetLocalBounds(self);
    // NOTE(Tony): Not sure if this is the right way to translate scale, but hey it works!
    localBounds.width *= (self.transform.getScale().x < 0) ? -1.0f : 1.0f;
    localBounds.height *= (self.transform.getScale().y < 0) ? -1.0f : 1.0f;
    return self.transform.getTransform().transformRect(localBounds);
}

void DrawCollider(const c_collider_t& collider)
{
    sf::FloatRect rect = ColliderGetGlobalBounds(collider);
    DrawOutlineFloatRect(sf::FloatRect(rect.left - rect.width / 2.0f, rect.top - rect.height / 2.0f,
                                       rect.width, rect.height), sf::Color::Green);
}