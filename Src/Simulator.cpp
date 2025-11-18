#include "Simulator.h"
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

Simulator::Simulator(unsigned int width, unsigned int height): _window(std::make_shared<sf::RenderWindow>(sf::VideoMode({ width,height }), "SFML Window"))
{
	ImGui::SFML::Init(*_window.get());
	//setup mouse cursor
	sf::Cursor cursor(sf::Cursor::Type::Arrow);
	_window->setMouseCursor(cursor);
}

void Simulator::Run()
{
}
