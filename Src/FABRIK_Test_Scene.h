#pragma once
#include "Scene.h"
#include <vector>
#include "SFML/Graphics.hpp"

class FABRIK_Test_Scene : public Scene {
	// constants
	float POINT_RADIUS{};
	float LINE_LENGTH{};
	int JOINT_COUNT{};
	float SPEED{};
	// data
	sf::RenderWindow* _window{};
	const sf::Vector2f centerPosition{};
	std::vector<float> distances{};
	std::vector<sf::CircleShape> joints{};
	struct SimpleLine {
		sf::Vertex position[2];
	};
	std::vector<SimpleLine> lines{};
	sf::CircleShape endPoint{};
	//interactions
	int selectedIdx{};
private:
	const sf::Vector2f OFFSET() const { return { POINT_RADIUS, POINT_RADIUS }; };
	void FABRIK_Step(int num);
public:
	FABRIK_Test_Scene(sf::RenderWindow* window);
	~FABRIK_Test_Scene() override;
	void UpdateImGui() override;
	void Draw() override;
	void Update(const float dt)override;
};