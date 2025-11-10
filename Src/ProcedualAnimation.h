#pragma once
#include "SFML/System.hpp"

void FABRIK(const sf::Vector2f targetPoint, std::vector<sf::Vector2f>& points, const std::vector<float>& distances, const int maxIteration = 100, const float availableDist = 0.1f);

inline const sf::Vector2f DistanceConstraint(const sf::Vector2f& FixedPoints, const sf::Vector2f& direction, const float distance)
{
	return FixedPoints + direction.normalized() * distance;
}