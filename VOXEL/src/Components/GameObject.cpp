#include "headers/GameObject.h"
#include "headers/Component.h"
#include <algorithm>

std::vector<GameObject*> GameObject::gameObjects;

GameObject::GameObject() {
    gameObjects.push_back(this);
}

GameObject::~GameObject() {
    gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), this), gameObjects.end());
}

void GameObject::update(float deltaTime) {}
void GameObject::start() {}