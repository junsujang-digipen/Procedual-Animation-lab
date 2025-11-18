#pragma once

#include <memory>

namespace sf {
	class RenderWindow;
}
class Simulator
{
	std::shared_ptr<sf::RenderWindow> _window{};
public:
	Simulator(unsigned int width = 800, unsigned int height = 600);
	void Run();
};

