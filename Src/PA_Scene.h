#pragma once
#include "Scene.h"
#include "SFML/Graphics.hpp"
#include <memory>

class DDLongLegs;
class PA_Scene : public Scene {
	sf::RenderWindow* _window{};
	std::unique_ptr<DDLongLegs> obj{};
public:
	PA_Scene(sf::RenderWindow* window);
	~PA_Scene() override;
	void UpdateImGui() override;
	void Draw() override;
	void Update(const float dt) override;
};