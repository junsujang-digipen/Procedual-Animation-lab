#include "FABRIK_Test_Scene.h"
#include "ProcedualAnimation.h"
#include <imgui.h>

#define TO_FLOAT(x) static_cast<float>(x)
#define TO_INT(x) static_cast<int>(x)

FABRIK_Test_Scene::FABRIK_Test_Scene(sf::RenderWindow* window) :POINT_RADIUS(5.f), LINE_LENGTH(25.f), JOINT_COUNT(7), SPEED(100.f), _window(window),centerPosition(_window->getSize().x / 2.f, _window->getSize().y / 2.f), distances(JOINT_COUNT, LINE_LENGTH), joints(JOINT_COUNT, sf::CircleShape(POINT_RADIUS)), lines(JOINT_COUNT)
{
	joints[0].setPosition(centerPosition - OFFSET());
	for (int i = 1; i < JOINT_COUNT; ++i) {
		joints[i].setPosition(joints[i - 1].getPosition() + sf::Vector2f{ 0.f, distances[i - 1] });
		joints[i].setFillColor(sf::Color::Green);
		lines[i - 1].position[0].position = joints[i - 1].getPosition() + OFFSET();
		lines[i - 1].position[1].position = joints[i].getPosition() + OFFSET();
	}
	lines[JOINT_COUNT - 1].position[0].position = joints[JOINT_COUNT - 1].getPosition() + OFFSET();
	lines[JOINT_COUNT - 1].position[1].position = joints[JOINT_COUNT - 1].getPosition() + sf::Vector2f{ 0.f, distances[JOINT_COUNT - 1] } + OFFSET();
}

FABRIK_Test_Scene::~FABRIK_Test_Scene()
{
}

void FABRIK_Test_Scene::UpdateImGui()
{
	ImGui::Begin("Hello, world!");
	ImGui::Button("Look at this pretty button");
	ImGui::End();
}

void FABRIK_Test_Scene::Draw()
{

	for (auto& l : lines) {
		_window->draw(l.position, 2, sf::PrimitiveType::Lines);
	}
	for (auto& c : joints) {
		_window->draw(c);
	}
}

void FABRIK_Test_Scene::Update(const float dt)
{
	// logic
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		auto mousePosition = sf::Mouse::getPosition(*_window);
		// FABRIK parameter setup
		std::vector<sf::Vector2f> points(JOINT_COUNT + 1);
		for (int i = 0; i < JOINT_COUNT; ++i) {
			points[i] = joints[i].getPosition() + OFFSET();
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
		for (int i = 0; i < JOINT_COUNT; ++i) {
			joints[i].setPosition(points[i] - OFFSET());
			lines[i].position[0].position = points[i];
			lines[i].position[1].position = points[i + 1];
		}
	}
}

