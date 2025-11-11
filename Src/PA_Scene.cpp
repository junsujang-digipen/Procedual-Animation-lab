#include "PA_Scene.h"
#include "DDLongLegs.h"

PA_Scene::PA_Scene(sf::RenderWindow* window) : _window(window), obj(std::make_unique<DDLongLegs>(sf::Vector2f{},6))
{
	obj->SetTarget(static_cast<sf::Vector2f>(_window->getSize())/2.f);
}

PA_Scene::~PA_Scene()
{

}

void PA_Scene::UpdateImGui()
{

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
