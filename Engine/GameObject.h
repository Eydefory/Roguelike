#pragma once

#include "EngineAPI.h"
#include "Component.h"
#include "TransformComponent.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <type_traits>

namespace XYZEngine
{
    class GameWorld;

    class XYZENGINE_API GameObject
    {
    public:
        GameObject();
        GameObject(std::string newName);

        ~GameObject();

        std::string GetName() const;
        void Print(int depth = 0) const;

        void Update(float deltaTime);
        void Render();

        template <typename T>
        T* AddComponent()
        {
            static_assert(std::is_base_of<Component, T>::value,
                "T must be derived from Component.");

            if constexpr (std::is_same<T, TransformComponent>::value)
            {
                if (GetComponent<TransformComponent>() != nullptr)
                {
                    std::cout << "Can't add Transform, because it will break the engine loop." << std::endl;
                    return nullptr;
                }
            }

            T* newComponent = new T(this);
            components.push_back(newComponent);

            return newComponent;
        }

        void RemoveComponent(Component* component)
        {
            auto it = std::find(components.begin(), components.end(), component);

            if (it != components.end())
            {
                components.erase(it);
                delete component;
                std::cout << "Deleted component" << std::endl;
            }
        }

        template <typename T>
        T* GetComponent() const
        {
            for (const auto& component : components)
            {
                if (auto casted = dynamic_cast<T*>(component))
                {
                    return casted;
                }
            }

            return nullptr;
        }

        template <typename T>
        T* GetComponentInChildren() const
        {
            T* component = GetComponent<T>();

            if (component != nullptr || children.empty())
            {
                return component;
            }

            for (const auto& child : children)
            {
                T* childComponent = child->GetComponentInChildren<T>();

                if (childComponent != nullptr)
                {
                    return childComponent;
                }
            }

            return nullptr;
        }

        template <typename T>
        std::vector<T*> GetComponents() const
        {
            std::vector<T*> result;

            for (const auto& component : components)
            {
                if (auto casted = dynamic_cast<T*>(component))
                {
                    result.push_back(casted);
                }
            }

            return result;
        }

        template <typename T>
        std::vector<T*> GetComponentsInChildren() const
        {
            std::vector<T*> result;

            for (const auto& component : GetComponents<T>())
            {
                result.push_back(component);
            }

            for (const auto& child : children)
            {
                for (const auto& childComponent : child->GetComponentsInChildren<T>())
                {
                    result.push_back(childComponent);
                }
            }

            return result;
        }

        friend class GameWorld;
        friend class TransformComponent;

    private:
        std::string name;

        std::vector<GameObject*> children = {};
        std::vector<Component*> components = {};

        void AddChild(GameObject* child);
        void RemoveChild(GameObject* child);
    };
}