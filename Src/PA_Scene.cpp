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

	// ground
	b2BodyDef groundBodyDef = b2DefaultBodyDef();
	groundBodyDef.position = b2Vec2(400.0f, 300.0f );
	ground._bodyId = b2CreateBody(_worldId, &groundBodyDef);
	b2Polygon groundBox = b2MakeBox(50.0f, 10.0f);
	b2ShapeDef groundShapeDef = b2DefaultShapeDef();
	ground._shapeId = b2CreatePolygonShape(ground._bodyId, &groundShapeDef, &groundBox);
	// ball
	b2BodyDef ballBodyDef = b2DefaultBodyDef();
	ballBodyDef.type = b2_dynamicBody;
	ballBodyDef.position = b2Vec2(400.0f, 400.0f);
	ball._bodyId = b2CreateBody(_worldId, &ballBodyDef);
	b2Circle ballBox{ {}, 10.0f };
	b2ShapeDef ballShapeDef = b2DefaultShapeDef();
	ball._shapeId = b2CreateCircleShape(ball._bodyId, &ballShapeDef, &ballBox);
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
	sf::RectangleShape groundshape{};
	groundshape.setSize({50.f,10.f});
	groundshape.setPosition({ 400.f,300.f });
	groundshape.setFillColor(sf::Color::White);
	_window->draw(groundshape);


	sf::CircleShape ballShape{};
	auto ballpos = b2Body_GetPosition(ball._bodyId);
	ballShape.setPosition({ ballpos.x, ballpos.y });
	auto ballData = b2Shape_GetCircle(ball._shapeId);
	ballShape.setRadius(ballData.radius);
	ballShape.setOrigin({ballData.radius,ballData.radius});
	_window->draw(ballShape);


	obj->Draw(_window);
}

void PA_Scene::Update(const float dt)
{
	b2World_Step(_worldId,dt,4);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		obj->SetTarget(_window->mapPixelToCoords(sf::Mouse::getPosition(*_window)));
	}
	obj->Update(dt);
}
