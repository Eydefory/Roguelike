#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Loger.h"

#include <algorithm>
#include <stdexcept>

namespace XYZRoguelike
{
    class HealthComponent : public XYZEngine::Component
    {
    public:
        HealthComponent(XYZEngine::GameObject* gameObject)
            : Component(gameObject), maxHealth(100), health(100), armor(0)
        {
            LOG_INFO("HealthComponent initialized");
        }

        void Update(float deltaTime) override {}
        void Render() override {}

        void Initialize(int newMaxHealth, int newArmor)
        {
            if (newMaxHealth <= 0 || newArmor < 0)
            {
                LOG_ERROR("Invalid health or armor values");
                throw std::invalid_argument("Invalid health or armor");
            }

            maxHealth = newMaxHealth;
            health = newMaxHealth;
            armor = newArmor;
        }

        void TakeDamage(int damage)
        {
            if (damage <= 0)
            {
                LOG_WARN("Damage must be positive");
                return;
            }

            int absorbed = std::min(armor, damage);

            armor -= absorbed;
            damage -= absorbed;

            health = std::max(0, health - damage);

            LOG_INFO("Entity received damage");

            if (!IsAlive())
                LOG_ERROR("Entity died");
        }

        int GetHealth() const
        {
            return health;
        }

        int GetMaxHealth() const
        {
            return maxHealth;
        }

        int GetArmor() const
        {
            return armor;
        }

        bool IsAlive() const
        {
            return health > 0;
        }

    private:
        int maxHealth;
        int health;
        int armor;
    };
} // namespace XYZRoguelike