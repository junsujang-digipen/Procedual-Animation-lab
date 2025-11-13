
#include "DDLongLegs.h"
#include <cmath>
#include <random>

// TODO: finding holdable position algorithm
// TODO: jittering
// TODO: Gravity
// TODO: holdable obstacle
// TODO: state controlling
// TODO: hunting state
// TODO: code refactoring

//? random
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> distAngle(0.f, 360.f);
std::uniform_real_distribution<float> distDist(100.f, 220.f);
std::uniform_real_distribution<float> distPercentage(0.f, 1.f);

DDLongLegs::DDLongLegs(sf::Vector2f position, int legNum):_center(position), _legs(legNum, Tentacle(10, 20))
{
	// random st point
	SearchHoldablePoints();
}



void DDLongLegs::Draw(sf::RenderWindow* window)
{
	sf::Vertex line[2]{ {},{} };
	line[0].color = sf::Color::Red;
	line[1].color = sf::Color::Red;
	line[0].position = _center;
	line[1].position = _targetPosition;
	window->draw(line, 2, sf::PrimitiveType::Lines);

	constexpr float size = 20.f;
	sf::CircleShape body(size);
	body.setOrigin(sf::Vector2f{ size,size });
	body.setPosition(_center);
	window->draw(body);

	constexpr float jointSize = 5.f;
	sf::CircleShape joint(jointSize);
	joint.setOrigin(sf::Vector2f{ jointSize,jointSize });
	for (int i = 0;i < _legs.size(); ++i) {
		line[0].position = _legs[i].GetToeSegment().getPosition();
		line[1].position = _legs[i].GetTarget();
		window->draw(line, 2, sf::PrimitiveType::Lines);
		for (auto& l : _legs[i].GetSegments()) {
			joint.setPosition(l.getPosition());
			window->draw(joint);
		}
		if (_legs[i].IsReached()) joint.setFillColor(sf::Color::Green);
		else joint.setFillColor(sf::Color::Red);
		window->draw(joint);
		joint.setFillColor(sf::Color::White);
	}
}

void DDLongLegs::Update(float dt)
{
	// try catching holdable point
	auto direction = (_targetPosition - _center).normalized();
	sf::Vector2f pulling{};
	for (int i = 0;i < _legs.size(); ++i) {
		auto isAnimationEnd = _legs[i].TryCatching(tentacleSpeed, dt);
		if (isAnimationEnd == false) continue;
		if (_legs[i].IsReached()) {
			auto totoe = _legs[i].GetToeSegment().getPosition() - _center;
			auto dotRes = direction.dot(totoe);
			if (dotRes == 0) continue;
			pulling += dotRes > 0 ? totoe.normalized() * pullingPower : -totoe.normalized() * pushingPower;

			// reset holding point
			auto probability = distPercentage(gen);
			if (probability < probabilityResetTarget) {
				SearchHoldablePointForALeg(i);
			}
		}
		else{
			SearchHoldablePointForALeg(i);
		}
	}
	// pulling & pushing body
	auto movePower = pulling * bodySpeed * dt;
	auto MovePowerLeng = movePower.lengthSquared();
	if (MovePowerLeng > 0 && (_center - _targetPosition).lengthSquared() > MovePowerLeng) {
		_center = _center + movePower;
		for (int i = 0;i < _legs.size(); ++i) {
			_legs[i].Following(_center);
		}
	}
}

void DDLongLegs::SearchHoldablePoints()
{
	for (int i = 0;i < _legs.size(); ++i) {
		SearchHoldablePointForALeg(i);
	}
}

void DDLongLegs::SearchHoldablePointForALeg(int i)
{
	auto angle = sf::radians(distAngle(gen)).asRadians();
	float x = std::cosf(angle);
	float y = std::sinf(angle);
	_legs[i].SetTarget(_center + sf::Vector2f{ x,y } *distDist(gen));
}

