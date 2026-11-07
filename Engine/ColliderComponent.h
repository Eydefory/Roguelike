#pragma once

#include "Collision.h"
#include "Component.h"
#include "PhysicsSystem.h"
#include "Trigger.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

namespace XYZEngine
{
    class ColliderComponent : public Component
    {
    public:
        ColliderComponent(GameObject* gameObject);

        virtual void Update(float deltaTime) = 0;
        virtual void Render() = 0;

        void SetTrigger(bool newIsTrigger)
        {
            isTrigger = newIsTrigger;
        }

        void SubscribeCollision(std::function<void(Collision)> onCollisionAction);
        void UnsubscribeCollision(std::function<void(Collision)> onCollisionAction);

        void SubscribeTriggerEnter(std::function<void(Trigger)> onTriggerEnterAction);
        void UnsubscribeTriggerEnter(std::function<void(Trigger)> onTriggerEnterAction);

        void SubscribeTriggerExit(std::function<void(Trigger)> onTriggerExitAction);
        void UnsubscribeTriggerExit(std::function<void(Trigger)> onTriggerExitAction);

        friend class PhysicsSystem;

    protected:
        sf::FloatRect bounds;
        bool isTrigger = false;

        void OnCollision(Collision collision);
        void OnTriggerEnter(Trigger trigger);
        void OnTriggerExit(Trigger trigger);

        std::vector<std::function<void(Collision)>> onCollisionActions;
        std::vector<std::function<void(Trigger)>> onTriggerEnterActions;
        std::vector<std::function<void(Trigger)>> onTriggerExitActions;
    };
} // namespace XYZEngine
