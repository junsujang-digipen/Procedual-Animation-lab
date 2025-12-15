
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "FABRIK_Test_Scene.h"
#include "PA_Scene.h"

int main() {
	// create window
	sf::RenderWindow window(sf::VideoMode({ 800,600 }), "SFML Window");
	ImGui::SFML::Init(window);
	//setup mouse cursor
	sf::Cursor cursor(sf::Cursor::Type::Arrow);
	window.setMouseCursor(cursor);
	// scene setup
	std::shared_ptr<Scene> currentScene = std::make_shared<PA_Scene>(&window);
	// setup FPS counter, deltaTime
	sf::Clock clock;
	clock.start();
	float timeChecker = 0.f;
	int frameCount = 0;
	auto dt = 0.f;
	// update loop
	while (window.isOpen()) {
		auto frameStartTime = clock.getElapsedTime();
		timeChecker += dt;
		if (timeChecker >= 1.f) {
			std::cout << std::format("[Elapsed time] {}", clock.getElapsedTime().asSeconds()) << std::endl;
			std::cout << std::format("FPS: {}, Avg.deltaTime: {}", frameCount, 1.f / frameCount) << std::endl;
			timeChecker = 0.f;
			frameCount = 0;
		}
		// Event processing
		while (auto w_event = window.pollEvent()) {
			ImGui::SFML::ProcessEvent(window, *w_event);
			if (w_event->is<sf::Event::Closed>()) {
				window.close();
			}
		}
		// update logic
		currentScene->Update(dt);
		//? Physics

		// rendering
		window.clear(sf::Color::Black);
		currentScene->Draw();
		ImGui::SFML::Render(window);
		window.display();
		// ImGui update
		ImGui::SFML::Update(window, sf::seconds(dt));
		ImGui::BeginMainMenuBar();
		if (ImGui::BeginMenu("Scene")) {
			if (ImGui::MenuItem("FABRIK test scene")) { currentScene = std::make_shared<FABRIK_Test_Scene>(&window); }
			if (ImGui::MenuItem("Tentacle creature scene")) { currentScene = std::make_shared<PA_Scene>(&window); }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
		currentScene->UpdateImGui();
		// update delta time
		dt = (clock.getElapsedTime() - frameStartTime).asSeconds();
		++frameCount;
	}
	ImGui::SFML::Shutdown();
	return 0;
}