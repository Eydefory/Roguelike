#pragma once

#include "Component.h"
#include "TransformComponent.h"

#include <vector>

namespace XYZRoguelike
{
    class EnemySearchComponent : public XYZEngine::Component
    {
    public:
        EnemySearchComponent(XYZEngine::GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override;

        void SetTarget(XYZEngine::GameObject* target);
        void SetMovementSettings(float detectionRadius, float movementSpeed, float stopDistance);
        void SetNavigationGrid(const std::vector<std::vector<bool>>& grid);

        bool IsTargetDetected() const;

        XYZEngine::GameObject* GetTarget() const;

    private:
        XYZEngine::GameObject* target;

        float detectionRadius;
        float movementSpeed;
        float stopDistance;

        bool targetDetected;
        std::vector<std::vector<bool>> navigationGrid;
    };
} // namespace XYZRoguelike
