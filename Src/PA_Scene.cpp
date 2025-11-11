#include "PA_Scene.h"
#include "DDLongLegs.h"

#include <imgui.h>

PA_Scene::PA_Scene(sf::RenderWindow* window) : _window(window), obj(std::make_unique<DDLongLegs>(sf::Vector2f{},6))
{
	obj->SetTarget(static_cast<sf::Vector2f>(_window->getSize())/2.f);
}

PA_Scene::~PA_Scene()
{

}

void PA_Scene::UpdateImGui()
{
	ImGui::Begin("It's Tentacle creature simulation!");
	ImGui::SetWindowSize({ 300,200 });
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
	ImGui::End();
}

void PA_Scene::Draw()
{
	obj->Draw(_window);
}

void PA_Scene::Update(const float dt)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		auto mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*_window));
		obj->SetTarget(mousePos);
	}
	obj->Update(dt);
}
