#pragma once
#include <vector>
#include <SFML/system.hpp>
#include "Segment.h"

class Tentacle {
	std::vector<Segment> _segments{};
	float _length{};
	sf::Vector2f _targetPosition{};
	sf::Vector2f _lastPosition{};
public:
	Tentacle(int jointNum, float length);
	const Segment& GetBodySegment() const { return _segments.front(); }
	const Segment& GetToeSegment() const { return _segments.back(); }
	const std::vector<Segment>& GetSegments() const { return _segments; }
	const sf::Vector2f& GetTarget()const { return _targetPosition; }
	const bool IsReached()const;

	void SetTarget(const sf::Vector2f& targetPosition);
	void Following(const sf::Vector2f& bodyPosition);
	const bool TryCatching(const float speed, const float dt);
	const void Reaching(const sf::Vector2f& currStepPosition);
private:
};