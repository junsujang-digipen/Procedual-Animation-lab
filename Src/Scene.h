#pragma once
class Scene {
public:
	virtual ~Scene() = default;
	virtual void UpdateImGui() {}
	virtual void Draw() = 0;
	virtual void Update(const float dt) = 0;
};