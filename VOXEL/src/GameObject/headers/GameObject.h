#pragma once


#include <memory>
#include <typeindex>
#include <unordered_map>

#include "Component.h"


class GameObject {
public:
    template<typename T, typename... Args>
    T* addComponent(Args&&... args) {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T* componentPtr = component.get();
        m_components[std::type_index(typeid(T))] = std::move(component);
        return componentPtr;
    }

    template<typename T>
    T* getComponent() {
        auto it = m_components.find(std::type_index(typeid(T)));
        if (it != m_components.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    template<typename T>
    void removeComponent() {
        m_components.erase(std::type_index(typeid(T)));
    }

    virtual void update(float deltaTime) {
    }

private:
    std::unordered_map<std::type_index, std::unique_ptr<Component>> m_components;
};



//class Enemy : public GameObject {
//public:
//    Enemy() {
//        addComponent<TransformComponent>();
//        addComponent<ColliderComponent>();
//    }
//
//    void update(float deltaTime) override {
//    }
//};