#pragma once
#include "Component.h"
#include <glm.hpp>
class TransformComponent : public Component {
public:
    glm::vec3 position{ 0.0f };
    glm::vec3 rotation{ 0.0f };
    glm::vec3 scale{ 1.0f };

    TransformComponent() {}
    TransformComponent(glm::vec3 position) : position(position) {}
    TransformComponent(glm::vec3 position, glm::vec3 rotation) : position(position), rotation(rotation) {}
    TransformComponent(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : position(position), rotation(rotation), scale(scale) {}
};

