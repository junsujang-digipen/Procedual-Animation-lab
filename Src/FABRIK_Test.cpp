#include <SFML/Graphics.hpp>
#include <iostream>
#include <format>
#include <vector>
#include <numeric>
#include "ProcedualAnimation.h"

#define TO_FLOAT(x) static_cast<float>(x)
#define TO_INT(x) static_cast<int>(x)
int main() {
	// create window
	sf::RenderWindow window(sf::VideoMode({ 800,600 }), "SFML Window");
	
	// constants
	constexpr float POINT_RADIUS = 5.f;
	constexpr sf::Vector2f OFFSET{ POINT_RADIUS, POINT_RADIUS };
	constexpr float LINE_LENGTH = 30.f;
	constexpr int JOINT_COUNT = 7;
	constexpr float SPEED = 100.f;
	const std::vector<float> distances{ LINE_LENGTH, LINE_LENGTH - 0.f, LINE_LENGTH - 5.f, LINE_LENGTH - 5.f, LINE_LENGTH - 10.f, LINE_LENGTH - 10.f, LINE_LENGTH - 10.f };
	const sf::Vector2f centerPosition{ window.getSize().x/2.f, window.getSize().y/2.f};
	// initial setup
	std::vector<sf::CircleShape> joints(JOINT_COUNT, sf::CircleShape(POINT_RADIUS));
	struct SimpleLine {
		sf::Vertex position[2];
	};
	std::vector<SimpleLine> lines(JOINT_COUNT);
	joints[0].setPosition(centerPosition - OFFSET);
	for (int i = 1; i < JOINT_COUNT; ++i) {
		joints[i].setPosition(joints[i-1].getPosition() + sf::Vector2f{ 0.f, distances[i - 1] });
		joints[i].setFillColor(sf::Color::Green);
		lines[i - 1].position[0].position = joints[i - 1].getPosition() + OFFSET;
		lines[i - 1].position[1].position = joints[i].getPosition() + OFFSET;
	}
	lines[JOINT_COUNT - 1].position[0].position = joints[JOINT_COUNT - 1].getPosition() + OFFSET;
	lines[JOINT_COUNT - 1].position[1].position = joints[JOINT_COUNT - 1].getPosition() + sf::Vector2f{ 0.f, distances[JOINT_COUNT - 1] } + OFFSET;

	//x TODO: delta time, FPS counter
	//x TODO: speed constraint? limit end point -> target point jumping with speed limit
	//x TODO: Distance constraint
	// TODO: restrict angle
	// TODO: Gravity
	// TODO: tentacle animation?
	//x TODO: Scene
	// TODO: CCD  
	
	sf::Clock clock;
	clock.start();
	float timeChecker = 0.f;
	int frameCount = 0;
	auto dt = 0.f;
	// update loop
	while (window.isOpen()) {
		auto frameStartTime = clock.getElapsedTime();
		timeChecker += dt;
		if (timeChecker >= 1.f) {
			std::cout << std::format("[Elapsed time] {}", clock.getElapsedTime().asSeconds()) << std::endl;
			std::cout << std::format("FPS: {}, Avg.deltaTime: {}", frameCount,  1.f / frameCount) << std::endl;
			timeChecker = 0.f;
			frameCount = 0;
		}
		// Event processing
		while (auto w_event = window.pollEvent()) {
			if (w_event->is<sf::Event::Closed>()) {
				window.close();
			}
		}
		// rendering
		window.clear(sf::Color::Black);

		for (auto& l : lines) {
			window.draw(l.position, 2, sf::PrimitiveType::Lines);
		}
		for (auto& c : joints) {
			window.draw(c);
		}

		window.display();

		// logic
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			auto mousePosition = sf::Mouse::getPosition(window);
			// FABRIK parameter setup
			std::vector<sf::Vector2f> points(JOINT_COUNT+1);
			for (int i = 0; i < JOINT_COUNT; ++i) {
				points[i] = joints[i].getPosition() + OFFSET;
			}
			points[JOINT_COUNT] = lines[JOINT_COUNT - 1].position[1].position;
			auto targetPoint = sf::Vector2f{ TO_FLOAT(mousePosition.x), TO_FLOAT(mousePosition.y) };
			auto dir = targetPoint - points[JOINT_COUNT];
			//x std::cout << std::format("Direction: {}, {}", dir.normalized().x, dir.normalized().y) << std::endl;
			//! Move to target point with speed limit
			if (dir.length() > SPEED * dt) {
				targetPoint = points[JOINT_COUNT] + dir.normalized() * SPEED * dt;
			}
			// FABRIK
			FABRIK(targetPoint, points, distances);
			// Apply FABRIK result
			for(int i = 0; i < JOINT_COUNT; ++i) {
				joints[i].setPosition(points[i] - OFFSET);
				lines[i].position[0].position = points[i];
				lines[i].position[1].position = points[i + 1];
			}
		}
		dt = (clock.getElapsedTime() - frameStartTime).asSeconds();
		++frameCount;
	}
	return 0;

}