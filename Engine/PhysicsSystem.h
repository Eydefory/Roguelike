#pragma once

#include "ColliderComponent.h"
#include "EngineAPI.h"
#include "RigidbodyComponent.h"
#include "Vector.h"
#include <iostream>
#include <map>

namespace XYZEngine
{
    class XYZENGINE_API PhysicsSystem
    {
    public:
        static PhysicsSystem* Instance();

        void Update();
        void MoveCollider(ColliderComponent* collider, float offsetX, float offsetY);

        float GetFixedDeltaTime() const;
        void Subscribe(ColliderComponent* collider);
        void Unsubscribe(ColliderComponent* collider);

    private:
        PhysicsSystem() {}
        ~PhysicsSystem() {}

        PhysicsSystem(PhysicsSystem const&) = delete;
        PhysicsSystem& operator=(PhysicsSystem const&) = delete;

        std::vector<ColliderComponent*> colliders;
        std::map<ColliderComponent*, ColliderComponent*> triggersEnteredPair;

        float fixedDeltaTime = 0.02f;
    };
} // namespace XYZEngine
