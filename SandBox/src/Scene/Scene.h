#pragma once
#include "../Core/config.h"

class Scene
{
public:
	Scene();
	~Scene();

	void update(float deltaTime);
	void initialize();
private:
	entt::registry m_Registry;
	void setUpMainMenu();
	void setUpPauseMenu();
	void SetUpGameUI();
};