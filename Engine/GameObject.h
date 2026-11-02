#pragma once

#include "EngineAPI.h"
#include "Component.h"
#include "TransformComponent.h"
#include "Loger.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <cassert>
#include <stdexcept>

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

            assert(!name.empty());

            if constexpr (std::is_same<T, TransformComponent>::value)
            {
                if (GetComponent<TransformComponent>() != nullptr)
                {
                    LOG_ERROR("Cannot add duplicate TransformComponent");

                    throw std::runtime_error(
                        "GameObject already has TransformComponent");
                }
            }

            try
            {
                T* newComponent = new T(this);

                if (newComponent == nullptr)
                {
                    LOG_ERROR("Component allocation failed");
                    throw std::runtime_error("Component allocation failed");
                }

                components.push_back(newComponent);

                LOG_INFO("Component added to GameObject");

                return newComponent;
            }
            catch (const std::exception& exception)
            {
                LOG_ERROR(std::string("AddComponent failed: ") +
                    exception.what());

                throw;
            }
        }

        void RemoveComponent(Component* component)
        {
            if (component == nullptr)
            {
                LOG_WARN("Cannot remove null component");
                return;
            }

            auto it = std::find(
                components.begin(),
                components.end(),
                component);

            if (it != components.end())
            {
                components.erase(it);
                delete component;

                LOG_INFO("Component removed");
            }
            else
            {
                LOG_WARN("Component not found");
            }
        }

        template <typename T>
        T* GetComponent() const
        {
            for (const auto& component : components)
            {
                if (auto casted = dynamic_cast<T*>(component))
                    return casted;
            }

            return nullptr;
        }

        template <typename T>
        T* GetComponentInChildren() const
        {
            T* component = GetComponent<T>();

            if (component != nullptr || children.empty())
                return component;

            for (const auto& child : children)
            {
                T* childComponent =
                    child->GetComponentInChildren<T>();

                if (childComponent != nullptr)
                    return childComponent;
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
                    result.push_back(casted);
            }

            return result;
        }

        template <typename T>
        std::vector<T*> GetComponentsInChildren() const
        {
            std::vector<T*> result;

            for (const auto& component : GetComponents<T>())
                result.push_back(component);

            for (const auto& child : children)
            {
                for (const auto& childComponent :
                    child->GetComponentsInChildren<T>())
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