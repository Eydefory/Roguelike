#pragma once

#include "Component.h"

namespace XYZRoguelike
{
    class EnemyAttackComponent : public XYZEngine::Component
    {
    public:
        EnemyAttackComponent(XYZEngine::GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override;

        void SetTarget(XYZEngine::GameObject* target);
        void SetAttackSettings(int damage, float cooldown, float attackDistance);

    private:
        XYZEngine::GameObject* target;

        float attackCooldown;
        float attackTimer;
        float attackDistance;
        int attackDamage;
    };
} // namespace XYZRoguelike
