#include "Tentacle.h"
#include "ProcedualAnimation.h"
#include <iostream>



constexpr float error = 0.1f;
Tentacle::Tentacle(int jointNum, float length) : _segments(jointNum), _length(length)
{
	if(jointNum < 2)
		std::cerr << "Tentacle has joints less than 2" << std::endl;
	for (int i = 0;i < jointNum;++i) {
		_segments[i].setPosition(sf::Vector2f{0.f,length*i});
	}
}

void Tentacle::SetTarget(const sf::Vector2f& targetPosition)
{
	_targetPosition = targetPosition;
}

void Tentacle::Following(const sf::Vector2f& bodyPosition)
{
	BackwardReaching(_segments, bodyPosition, _length);
}

const bool Tentacle::TryCatching(const float speed, const float dt)
{
	auto curr = _segments.back().getPosition();
	auto direction = (_targetPosition - curr);
	if (direction.length() < speed*dt) {
		curr = _targetPosition;
	}
	else {
		curr += direction.normalized() * speed * dt;
	}
	return Reaching(curr) && curr == _targetPosition;
}

const bool Tentacle::Reaching(const sf::Vector2f& currStepPosition)
{
	//FABRIK
	sf::Vector2f fixedPoint{_segments.front().getPosition()};
	for (int i = 0; i < 100; ++i) {
		ForwardReaching(_segments, currStepPosition, _length);
		BackwardReaching(_segments, fixedPoint, _length);
		// end check
		if ((_segments.back().getPosition() - currStepPosition).length() <= error) {
			return true;
		}
	}
	return false;
}
