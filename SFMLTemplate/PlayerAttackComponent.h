#pragma once

#include "Component.h"

namespace XYZRoguelike
{
    class EnemyContainer;

    class PlayerAttackComponent : public XYZEngine::Component
    {
    public:
        PlayerAttackComponent(XYZEngine::GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override;

        void SetEnemyContainer(EnemyContainer* container);

    private:
        EnemyContainer* enemyContainer;
        float attackCooldown;
        float attackTimer;
    };
}