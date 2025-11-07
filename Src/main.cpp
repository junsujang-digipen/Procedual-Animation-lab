#include <SFML/Graphics.hpp>
#include <iostream>
#include <format>
#include <vector>
#include <numeric>

#define TO_FLOAT(x) static_cast<float>(x)
#define TO_INT(x) static_cast<int>(x)
void FABRIK(const sf::Vector2f targetPoint, std::vector<sf::Vector2f>& points, const std::vector<float>& distances, const int maxIteration = 100, const float availableDist = 0.05f);
int main() {
	// create window
	sf::RenderWindow window(sf::VideoMode({ 800,600 }), "SFML Window");
	//setup mouse cursor
	sf::Cursor cursor(sf::Cursor::Type::Arrow);
	window.setMouseCursor(cursor);
	// constants
	constexpr float POINT_RADIUS = 5.f;
	constexpr sf::Vector2f OFFSET{ POINT_RADIUS, POINT_RADIUS };
	constexpr float LINE_LENGTH = 30.f;
	constexpr int JOINT_COUNT = 7;
	const std::vector<float> distances{ LINE_LENGTH, LINE_LENGTH - 0.f, LINE_LENGTH - 5.f, LINE_LENGTH - 5.f, LINE_LENGTH - 10.f, LINE_LENGTH - 10.f, LINE_LENGTH - 10.f };
	// initial setup
	std::vector<sf::CircleShape> joints(JOINT_COUNT, sf::CircleShape(POINT_RADIUS));
	struct SimpleLine {
		sf::Vertex position[2];
	};
	std::vector<SimpleLine> lines(JOINT_COUNT);
	sf::Vector2f centerPosition{ 400.f, 300.f };
	joints[0].setPosition(centerPosition - OFFSET);
	for (int i = 1; i < JOINT_COUNT; ++i) {
		joints[i].setPosition(joints[i-1].getPosition() + sf::Vector2f{ 0.f, distances[i - 1] });
		joints[i].setFillColor(sf::Color::Green);
		lines[i - 1].position[0].position = joints[i - 1].getPosition() + OFFSET;
		lines[i - 1].position[1].position = joints[i].getPosition() + OFFSET;
	}
	lines[JOINT_COUNT - 1].position[0].position = joints[JOINT_COUNT - 1].getPosition() + OFFSET;
	lines[JOINT_COUNT - 1].position[1].position = joints[JOINT_COUNT - 1].getPosition() + sf::Vector2f{ 0.f, distances[JOINT_COUNT - 1] } + OFFSET;

	//x TODO: delta time
	// TODO: Scene
	// TODO: Gravity
	// TODO: tentacle animation?
	// TODO: restrict angle
	// TODO: CCD  
	
	sf::Clock clock;
	// update loop
	while (window.isOpen()) {
		auto dt = clock.restart();
		//x std::cout << std::format("Delta time: {}", dt.asSeconds()) << std::endl;
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
			points[JOINT_COUNT] = points[JOINT_COUNT - 1];
			auto targetPoint = sf::Vector2f{ TO_FLOAT(mousePosition.x), TO_FLOAT(mousePosition.y) };
			auto dir = targetPoint - points[JOINT_COUNT];
			//x std::cout << std::format("Direction: {}, {}", dir.normalized().x, dir.normalized().y) << std::endl;
			//! Move to target point with speed limit
			const float SPEED = 21.f;
			if (dir.length() > SPEED) {
				targetPoint = points[JOINT_COUNT] + dir.normalized() * SPEED;
			}
			//! Move to target point with ratio
			//const float ratio = 0.2f;
			//if (dir.length() > 1.f) targetPoint = points[JOINT_COUNT] + dir * ratio;
			// FABRIK
			FABRIK(targetPoint, points, distances);
			// Apply FABRIK result
			for(int i = 0; i < JOINT_COUNT; ++i) {
				joints[i].setPosition(points[i] - OFFSET);
				lines[i].position[0].position = points[i];
				lines[i].position[1].position = points[i + 1];
			}
		}
	}
	return 0;

}

void FABRIK(const sf::Vector2f targetPoint, std::vector<sf::Vector2f>& points, const std::vector<float>& distances, const int maxIteration, const float availableDist)
{
	using Vec2 = sf::Vector2f;
	Vec2 stPoint = points.front();
	for (int i = 0; i < maxIteration; ++i) {
		// Forward reaching
		points.back() = targetPoint;
		for (int j = TO_INT(points.size()) - 2; j >= 0; --j) {
			auto dir = points[j] - points[j + 1];
			points[j] = points[j + 1] + dir.normalized() * distances[j];
		}
		// Backward reaching
		points.front() = stPoint;
		for (int j = 1; j < points.size(); ++j) {
			auto dir = points[j] - points[j - 1];
			points[j] = points[j - 1] + dir.normalized() * distances[j - 1];
		}
		// end check
		if((points.back() - targetPoint).length() <= availableDist) {
			break;
		}
	}
}
