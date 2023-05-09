#include "transform_utils.h"
#include "vector_math.h"

sf::Vector2f TransformForward(const sf::Transformable& transform)
{
    sf::Transform t;
    t.rotate(transform.getRotation());
    t.scale(transform.getScale());
    return VectorNormalize(t.transformPoint(sf::Vector2f(1, 0)));
}

sf::Vector2f TransformUp(const sf::Transformable& transform)
{
    sf::Transform t;
    t.rotate(transform.getRotation());
    t.scale(transform.getScale());
    return VectorNormalize(t.transformPoint(sf::Vector2f(0, -1)));
}