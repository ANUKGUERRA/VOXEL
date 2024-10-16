#pragma once
#include "GameObject.h"
class GameObject;

class Component {
public:
    Component() : gameObject(nullptr) {}
    virtual ~Component() = default;

    void setGameObject(GameObject* gameObject) { gameObject = gameObject; }
    GameObject* getGameObject() const { return gameObject; }

protected:
    GameObject* gameObject;
};

