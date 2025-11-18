#include "PA_Scene.h"
#include "DDLongLegs.h"

#include <imgui.h>
#include <box2d/box2d.h>

PA_Scene::PA_Scene(sf::RenderWindow* window) : _window(window), obj(std::make_unique<DDLongLegs>(sf::Vector2f{},6))
{
	auto winSize{ static_cast<sf::Vector2f>(_window->getSize()) };
	obj->SetTarget(winSize/2.f);

	// set coordinate origin position to bottom-left
	sf::View view{ winSize / 2.f,{winSize.x, -winSize.y} };
	_window->setView(view);

	b2WorldDef world = b2DefaultWorldDef();
	world.gravity = b2Vec2(0.f,-9.8f);
	_worldId = b2CreateWorld(&world);
	b2BodyDef groundBodyDef = b2DefaultBodyDef();
	groundBodyDef.position = b2Vec2(0.0f, -10.0f );
	b2BodyId groundId = b2CreateBody(_worldId, &groundBodyDef);
	b2Polygon groundBox = b2MakeBox(50.0f, 10.0f);
	b2ShapeDef groundShapeDef = b2DefaultShapeDef();
	b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);
}

PA_Scene::~PA_Scene()
{
	_window->setView(_window->getDefaultView());
}

void PA_Scene::UpdateImGui()
{
	ImGui::Begin("It's Tentacle creature simulation!");
	ImGui::SetWindowSize({ 300,250 });
	ImGui::Text("Tentacle creature!!");
	if (ImGui::Button("Searching holdable point")) {
		// reset holdable points
		obj->SearchHoldablePoints();
	}
	ImGui::SliderFloat("Body speed", &obj->bodySpeed, 0.f, 1000.f);
	ImGui::SliderFloat("Tentacle speed", &obj->tentacleSpeed,0.f, 1000.f);
	ImGui::SliderFloat("Pulling power", &obj->pullingPower,0.f,5.f);
	ImGui::SliderFloat("Pushing power", &obj->pushingPower, 0.f, 5.f);
	ImGui::InputFloat("Probability of re-searching holdable points", &obj->probabilityResetTarget);
	ImGui::Text("Click to set target position!");
	auto mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*_window));
	ImGui::Text(std::format("Mouse Position: ({}, {})", mousePos.x, mousePos.y).c_str());
	auto mousePosToCoords = _window->mapPixelToCoords(sf::Mouse::getPosition(*_window));
	ImGui::Text(std::format("Mouse Position in Coords: ({}, {})", mousePosToCoords.x, mousePosToCoords.y).c_str());
	ImGui::End();
}

void PA_Scene::Draw()
{
	sf::RectangleShape ground{};
	ground.setSize({50.f,10.f});
	ground.setPosition({ 400.f,300.f });
	ground.setFillColor(sf::Color::White);
	_window->draw(ground);

	obj->Draw(_window);
}

void PA_Scene::Update(const float dt)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		obj->SetTarget(_window->mapPixelToCoords(sf::Mouse::getPosition(*_window)));
	}
	obj->Update(dt);
}
