#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Tentacle.h"


class DDLongLegs {
	std::vector<Tentacle> _legs{};
	sf::Vector2f _center{};
	sf::Vector2f _targetPosition{};
public:
	DDLongLegs(sf::Vector2f position, int legNum);

	//? temporary draw function
	void Draw(sf::RenderWindow* window);
	void Update(float dt);
	const sf::Vector2f& GetCenter() { return _center; }
	void SetTarget(const sf::Vector2f targetPosition) { _targetPosition = targetPosition; }

public: // constants for ImGui
	float pullingPower{ 1.f };
	float pushingPower{ 0.1f };
	float bodySpeed{ 100.f };
	float tentacleSpeed{ 300.f };
	float probabilityResetTarget{0.005f};
};