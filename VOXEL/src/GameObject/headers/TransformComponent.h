#pragma once
#include "Component.h"
#include <glm.hpp>
class TransformComponent : public Component {
public:
    glm::vec3 position{ 0.0f };
    glm::vec3 rotation{ 0.0f };
    glm::vec3 scale{ 1.0f };
};

