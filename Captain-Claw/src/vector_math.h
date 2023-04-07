#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace sf;

Vector2f VectorCalculateDirection(Vector2f vec1, Vector2f vec2);
double VectorCalculateAngleRad(Vector2f vec1, Vector2f vec2);
double VectorMagnitude(Vector2f vec);
Vector2f VectorNormalize(Vector2f vec);
double VectorCalculateSqrdDistance(Vector2f vec1, Vector2f vec2);
double VectorCalculateDistance(Vector2f vec1, Vector2f vec2);