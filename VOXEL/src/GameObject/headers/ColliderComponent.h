#pragma once
#include <string>
#include <vec3.hpp>
#include "Component.h"
#include "GameObject.h"

class ColliderComponent : public Component
{
public:
    ColliderComponent();
    ColliderComponent(glm::vec3 min, glm::vec3 max);

    bool intersects(const ColliderComponent& other) const;
    void setColliderPosition(glm::vec3 min, glm::vec3 max);
    void checkCollisions();

    std::string Tag;
    glm::vec3 min;
    glm::vec3 max;

private:
    void handleCollision(const ColliderComponent& other);
};
