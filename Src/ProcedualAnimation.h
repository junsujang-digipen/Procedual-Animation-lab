#pragma once
#include "SFML/System.hpp"
#include <vector>

class Segment;
void ForwardReaching(std::vector<Segment>& segments, sf::Vector2f targetPosition, const float distance);
void BackwardReaching(std::vector<Segment>& segments, sf::Vector2f targetPosition, const float distance);

void FABRIK(const sf::Vector2f targetPoint, std::vector<sf::Vector2f>& points, const std::vector<float>& distances, const int maxIteration = 100, const float availableDist = 0.1f);

inline const sf::Vector2f DistanceConstraintByDirection(const sf::Vector2f& FixedPoints, const sf::Vector2f& direction, const float distance)
{
	return FixedPoints + direction.normalized() * distance;
}
inline void DistanceConstraint(const sf::Vector2f& FixedPoints, sf::Vector2f& target, const float distance)
{
	target = DistanceConstraintByDirection(FixedPoints, target - FixedPoints, distance);
}
inline const sf::Vector2f GetDistanceConstraint(const sf::Vector2f& FixedPoints, const sf::Vector2f target, const float distance)
{
	return DistanceConstraintByDirection(FixedPoints, target - FixedPoints, distance);
}