#pragma once

#include "Component.h"

namespace XYZRoguelike
{
    class EnemyContainer;
    class InventoryHudComponent;

    class PlayerAttackComponent : public XYZEngine::Component
    {
    public:
        PlayerAttackComponent(XYZEngine::GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override;

        void SetEnemyContainer(EnemyContainer* container);

    private:
        void AttackWithSword(InventoryHudComponent* hud);
        void FireBow(InventoryHudComponent* hud);

        EnemyContainer* enemyContainer;
        float attackCooldown;
        float attackTimer;
    };
} // namespace XYZRoguelike
