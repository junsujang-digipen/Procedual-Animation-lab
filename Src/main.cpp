#include <SFML/Graphics.hpp>
#include <iostream>
#include <format>
#include <vector>
#include <numeric>

#define TO_FLOAT(x) static_cast<float>(x)
#define TO_INT(x) static_cast<int>(x)
std::vector<sf::Vector2f> FABRIK(const sf::Vector2f targetPoint, std::vector<sf::Vector2f>& points, const std::vector<float>& distances, const int maxIteration = 100, const float availableDist = 0.5f);
int main() {
	// create window
	sf::RenderWindow window(sf::VideoMode({ 800,600 }), "SFML Window");
	//setup mouse cursor
	sf::Cursor cursor(sf::Cursor::Type::Arrow);
	window.setMouseCursor(cursor);


	constexpr float POINT_RADIUS = 5.f;
	constexpr float LINE_LENGTH = 100.f;
	constexpr int JOINT_COUNT = 3;
	std::vector<sf::CircleShape> joints(JOINT_COUNT);
	for (auto& c : joints) {
		c.setRadius(POINT_RADIUS);
		c.setPosition({ 400.f - POINT_RADIUS, 300.f - POINT_RADIUS });
		c.setFillColor(sf::Color::Green);
	}
	struct SimpleLine {
		sf::Vertex position[2];
	};
	std::vector<SimpleLine> lines(JOINT_COUNT);
	
	// update loop
	while (window.isOpen()) {
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
			std::vector<float> distances(JOINT_COUNT, LINE_LENGTH);
			for (int i = 0; i < JOINT_COUNT; ++i) {
				points[i] = joints[i].getPosition() + sf::Vector2f{ POINT_RADIUS, POINT_RADIUS };
			}
			points[JOINT_COUNT] = points[JOINT_COUNT - 1];
			// FABRIK
			FABRIK(sf::Vector2f{ TO_FLOAT(mousePosition.x), TO_FLOAT(mousePosition.y) }, points, distances);
			// Apply FABRIK result
			for(int i = 0; i < JOINT_COUNT; ++i) {
				joints[i].setPosition(points[i] - sf::Vector2f{ POINT_RADIUS, POINT_RADIUS });
				lines[i].position[0].position = points[i];
				lines[i].position[1].position = points[i + 1];
			}
		}
	}
	return 0;

}

std::vector<sf::Vector2f> FABRIK(const sf::Vector2f targetPoint, std::vector<sf::Vector2f>& points, const std::vector<float>& distances, const int maxIteration, const float availableDist)
{
	using Vec2 = sf::Vector2f;
	Vec2 stPoint = points.front();
	// 도달 불가능 할 때 처리
	//auto totalLength = std::accumulate(distances.begin(), distances.end(), 0.f);
	//if(totalLength < )
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
		// break check
		if((points.back() - targetPoint).length() <= availableDist) {
			break;
		}
	}
	return points;
}
