#pragma once

#include "Component.h"
#include "TransformComponent.h"

namespace XYZRoguelike
{
    class EnemySearchComponent : public XYZEngine::Component
    {
    public:
        EnemySearchComponent(XYZEngine::GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override;

        void SetTarget(XYZEngine::GameObject* target);

        bool IsTargetDetected() const;

        XYZEngine::GameObject* GetTarget() const;

    private:
        XYZEngine::GameObject* target;

        float detectionRadius;
        float movementSpeed;
        float stopDistance;

        bool targetDetected;
    };
}