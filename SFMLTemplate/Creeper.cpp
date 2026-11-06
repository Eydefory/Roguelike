#include "Creeper.h"

#include "Constants.h"

namespace XYZRoguelike
{
    namespace
    {
        EnemyConfig MakeCreeperConfig()
        {
            EnemyConfig config;
            config.objectName = "Creeper";
            config.textureName = "enemy";
            config.maxHealth = CREEPER_MAX_HEALTH;
            config.armor = CREEPER_ARMOR;
            config.attackDamage = CREEPER_DAMAGE;
            config.movementSpeed = CREEPER_SPEED;
            config.detectionRadius = CREEPER_DETECTION_RADIUS;
            config.stopDistance = CREEPER_STOP_DISTANCE;
            config.attackCooldown = CREEPER_ATTACK_COOLDOWN;
            config.attackDistance = CREEPER_ATTACK_DISTANCE;
            return config;
        }
    } // namespace

    Creeper::Creeper(XYZEngine::GameObject* player, float worldX, float worldY)
        : Enemy(player, MakeCreeperConfig(), worldX, worldY)
    {
    }
} // namespace XYZRoguelike
