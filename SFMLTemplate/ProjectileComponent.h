#pragma once

#include "Component.h"

namespace XYZRoguelike
{
    class EnemyContainer;

    class ProjectileComponent : public XYZEngine::Component
    {
    public:
        ProjectileComponent(XYZEngine::GameObject* gameObject, XYZEngine::GameObject* player,
                            EnemyContainer* enemies, float directionX, float directionY);

        void Update(float deltaTime) override;
        void Render() override;

    private:
        XYZEngine::GameObject* player = nullptr;
        EnemyContainer* enemies = nullptr;
        float directionX = 0.f;
        float directionY = 1.f;
        float distanceTravelled = 0.f;
    };
} // namespace XYZRoguelike
