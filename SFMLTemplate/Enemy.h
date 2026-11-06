#pragma once

#include "Constants.h"
#include "GameWorld.h"
#include "HealthComponent.h"

#include <string>

namespace XYZRoguelike
{
    // Shared settings let enemy subclasses reuse the same combat components.
    struct EnemyConfig
    {
        std::string objectName = "Enemy";
        std::string textureName = "enemy";
        int maxHealth = ENEMY_MAX_HEALTH;
        int armor = ENEMY_ARMOR;
        int attackDamage = ENEMY_DAMAGE;
        float movementSpeed = ENEMY_SPEED;
        float detectionRadius = ENEMY_DETECTION_RADIUS;
        float stopDistance = ENEMY_STOP_DISTANCE;
        float attackCooldown = ENEMY_ATTACK_COOLDOWN;
        float attackDistance = ENEMY_ATTACK_DISTANCE;
    };

    // Shared game-object and combat setup for enemy archetypes.
    class Enemy
    {
    public:
        virtual ~Enemy() = default;

        XYZEngine::GameObject* GetGameObject() const;
        HealthComponent* GetHealthComponent() const;

    protected:
        Enemy(XYZEngine::GameObject* player, const EnemyConfig& config, float worldX, float worldY);

    private:
        XYZEngine::GameObject* gameObject = nullptr;
    };
} // namespace XYZRoguelike
