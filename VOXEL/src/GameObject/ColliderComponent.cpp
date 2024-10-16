#include "headers/ColliderComponent.h"

ColliderComponent::ColliderComponent() : min(0.0f), max(0.0f) {}

ColliderComponent::ColliderComponent(glm::vec3 min, glm::vec3 max) : min(min), max(max) {}

bool ColliderComponent::intersects(const ColliderComponent& other) const
{
    return (min.x <= other.max.x && max.x >= other.min.x) &&
        (min.y <= other.max.y && max.y >= other.min.y) &&
        (min.z <= other.max.z && max.z >= other.min.z);
}

void ColliderComponent::setColliderPosition(glm::vec3 min, glm::vec3 max)
{
    this->min = min;
    this->max = max;
}

void ColliderComponent::checkCollisions()
{
    for (size_t i = 0; i < GameObject::gameObjects.size(); ++i) {
        auto collider = GameObject::gameObjects[i]->getComponent<ColliderComponent>();
        if (!collider) continue;

        if (collider != this && collider->intersects(*this))
            this->handleCollision(*collider);
    }
}

void ColliderComponent::handleCollision(const ColliderComponent& other)
{
}
