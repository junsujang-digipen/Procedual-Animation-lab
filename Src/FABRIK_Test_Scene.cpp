#include "FABRIK_Test_Scene.h"
#include "ProcedualAnimation.h"
#include <imgui.h>
//x TODO: delta time, FPS counter
//x TODO: speed constraint? limit end point -> target point jumping with speed limit
//x TODO: Distance constraint
// TODO: restrict angle
// TODO: transform hierarchy
// TODO: Gravity
// TODO: tentacle animation?
//x TODO: Scene
// TODO: CCD
#define TO_FLOAT(x) static_cast<float>(x)
#define TO_INT(x) static_cast<int>(x)

void FABRIK_Test_Scene::FABRIK_Step(int num)
{
}

FABRIK_Test_Scene::FABRIK_Test_Scene(sf::RenderWindow* window) :POINT_RADIUS(5.f), LINE_LENGTH(25.f), JOINT_COUNT(7), SPEED(100.f), _window(window), centerPosition(_window->getSize().x / 2.f, _window->getSize().y / 2.f), distances(JOINT_COUNT, LINE_LENGTH), joints(JOINT_COUNT, sf::CircleShape(POINT_RADIUS)), lines(JOINT_COUNT), endPoint(POINT_RADIUS), selectedIdx(-1)
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
	endPoint.setPosition(lines[JOINT_COUNT - 1].position[1].position - OFFSET());
}

FABRIK_Test_Scene::~FABRIK_Test_Scene()
{
}

void FABRIK_Test_Scene::UpdateImGui()
{
	ImGui::Begin("Hello, FABRIK!");
	ImGui::SetWindowSize({300,200});
	ImGui::Text("Look! It's FABRIK!");
	if (ImGui::Button("Generate joint")) {
		// Generate a new joint
		distances.emplace_back(LINE_LENGTH);
		joints.emplace_back(sf::CircleShape(POINT_RADIUS));
		joints.back().setFillColor(sf::Color::Green);
		joints.back().setPosition(lines.back().position[1].position - OFFSET());
		lines.emplace_back();
		lines.back().position[0].position = joints.back().getPosition() + OFFSET();
		lines.back().position[1].position = joints.back().getPosition() + sf::Vector2f{ 0.f, distances.back() } + OFFSET();
		JOINT_COUNT += 1;
		endPoint.setPosition(lines[JOINT_COUNT - 1].position[1].position - OFFSET());
	}
	if (ImGui::Button("Delete joint")) {
		// Delete the last joint
		if (JOINT_COUNT > 1) {
			joints.pop_back();
			distances.pop_back();
			lines.pop_back();
			JOINT_COUNT -= 1;
			endPoint.setPosition(lines[JOINT_COUNT - 1].position[1].position - OFFSET());
		}
	}
	ImGui::InputFloat("Following speed",&SPEED);
	ImGui::Text("Drag joint to test FABRIK!");
	ImGui::Text(std::format("Joint count: {}", JOINT_COUNT).c_str());

	ImGui::End();
}

void FABRIK_Test_Scene::Draw()
{

	if (selectedIdx > 0) {
		SimpleLine l{};
		l.position[0].position = lines[selectedIdx-1].position[1].position;
		l.position[0].color = sf::Color::Red;
		l.position[1].position = sf::Vector2f{ TO_FLOAT(sf::Mouse::getPosition(*_window).x), TO_FLOAT(sf::Mouse::getPosition(*_window).y) };
		l.position[1].color = sf::Color::Red;
		_window->draw(l.position, 2, sf::PrimitiveType::Lines);
	}
	for (int i = 0; i < lines.size(); ++i) {
		_window->draw(lines[i].position, 2, sf::PrimitiveType::Lines);
	}
	for (auto& c : joints) {
		_window->draw(c);
	}
	_window->draw(endPoint);
}

void FABRIK_Test_Scene::Update(const float dt)
{
	auto mousePosition = sf::Mouse::getPosition(*_window);
	auto targetPoint = sf::Vector2f{ TO_FLOAT(mousePosition.x), TO_FLOAT(mousePosition.y) };
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && selectedIdx == -1) {
		// collision check
		for (int i = 1; i < JOINT_COUNT; ++i) {
			if (joints[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
				// Joint is clicked
				joints[i].setFillColor(sf::Color::Red);
				selectedIdx = i;
				break;
			}
		}
		if (endPoint.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
			endPoint.setFillColor(sf::Color::Red);
			selectedIdx = JOINT_COUNT;
		}
	}
	if(selectedIdx > 0){ 
		// FABRIK parameter setup
		std::vector<sf::Vector2f> points(selectedIdx + 1);
		for (int i = 0; i < selectedIdx; ++i) {
			points[i] = joints[i].getPosition() + OFFSET();
		}
		points[selectedIdx] = lines[selectedIdx - 1].position[1].position;
		auto dir = targetPoint - points[selectedIdx];
		//x std::cout << std::format("Direction: {}, {}", dir.normalized().x, dir.normalized().y) << std::endl;
		//! Move to target point with speed limit
		if (dir.length() > SPEED * dt) {
			targetPoint = points[selectedIdx] + dir.normalized() * SPEED * dt;
		}
		// FABRIK
		FABRIK(targetPoint, points, distances);
		// Apply FABRIK result
		for (int i = 0; i < selectedIdx; ++i) {
			joints[i].setPosition(points[i] - OFFSET());
			lines[i].position[0].position = points[i];
			lines[i].position[1].position = points[i + 1];
		}
		// controlled joint and following joints
		if(selectedIdx != JOINT_COUNT){
			// Distance constraint
			joints[selectedIdx].setPosition(targetPoint-OFFSET());
			for (int i = selectedIdx+1; i < JOINT_COUNT; ++i) {
				lines[i-1].position[0].position = joints[i-1].getPosition() + OFFSET();
				DistanceConstraint(lines[i-1].position[0].position, lines[i-1].position[1].position, distances[i-1]);
				joints[i].setPosition(lines[i-1].position[1].position - OFFSET());
			}
			lines[JOINT_COUNT - 1].position[0].position = joints[JOINT_COUNT - 1].getPosition() + OFFSET();
			DistanceConstraint(lines[JOINT_COUNT - 1].position[0].position, lines[JOINT_COUNT - 1].position[1].position, distances[JOINT_COUNT - 1]);
		}
		endPoint.setPosition(lines[JOINT_COUNT - 1].position[1].position - OFFSET());
		// reset
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) == false){
			if (selectedIdx == JOINT_COUNT) endPoint.setFillColor(sf::Color::White);
			else joints[selectedIdx].setFillColor(sf::Color::Green);
			selectedIdx = -1; 
		}
	}
}

