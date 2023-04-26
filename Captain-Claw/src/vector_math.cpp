#include <vector_math.h>
#include <SFML/Graphics.hpp>
#include <cmath>

using namespace sf;

Vector2f VectorCalculateDirection(Vector2f vec1, Vector2f vec2)
{
    Vector2f dir = vec2 - vec1;
    return dir;
}

double VectorCalculateAngleRad(Vector2f vec1, Vector2f vec2)
{
    float x = vec2.x - vec1.x, y = vec2.y - vec1.y;
    return atan2f(y, x);
}

double VectorMagnitude(Vector2f vec)
{
    return sqrt(pow(vec.x, 2) + pow(vec.y, 2));
}

Vector2f VectorNormalize(Vector2f vec)
{
    if (VectorMagnitude(vec) == 0.0f) {
        return vec;
    }

    Vector2f normalizedVector;
    normalizedVector.x = vec.x / VectorMagnitude(vec);
    normalizedVector.y = vec.y / VectorMagnitude(vec);

    return normalizedVector;
}

double VectorCalculateSqrdDistance(Vector2f vec1, Vector2f vec2)
{
    return pow(vec1.x - vec2.x, 2) + pow(vec1.y - vec2.y, 2);
}

double VectorCalculateDistance(Vector2f vec1, Vector2f vec2)
{
    double rootDist = VectorCalculateSqrdDistance(vec1, vec2);
    return rootDist;
}