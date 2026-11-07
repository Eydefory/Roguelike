#pragma once

#include "CameraComponent.h"
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
            : Component(gameObject), maxHealth(100), health(100), maxArmor(0), armor(0)
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
            maxArmor = newArmor;
            armor = newArmor;
        }

        void TakeDamage(int damage)
        {
            if (damage <= 0)
            {
                LOG_WARN("Damage must be positive");
                return;
            }

            const int absorbed = std::min(armor, damage);
            armor -= absorbed;
            damage -= absorbed;

            health = std::max(0, health - damage);

            auto* camera = gameObject->GetComponent<XYZEngine::CameraComponent>();
            if (camera != nullptr)
            {
                camera->Shake();
            }

            LOG_INFO("Entity received damage");

            if (!IsAlive())
                LOG_ERROR("Entity died");
        }

        void TakeHit(int damage)
        {
            if (damage <= 0)
            {
                LOG_WARN("Damage must be positive");
                return;
            }

            if (armor > 0)
            {
                --armor;
            }
            else
            {
                health = std::max(0, health - damage);
            }

            auto* camera = gameObject->GetComponent<XYZEngine::CameraComponent>();
            if (camera != nullptr)
            {
                camera->Shake();
            }

            LOG_INFO("Entity received damage");
            if (!IsAlive())
            {
                LOG_ERROR("Entity died");
            }
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

        int GetMaxArmor() const
        {
            return maxArmor;
        }

        int Heal(int amount)
        {
            if (amount <= 0 || !IsAlive())
            {
                return 0;
            }

            const int previousHealth = health;
            health = amount >= maxHealth - health ? maxHealth : health + amount;
            const int restored = health - previousHealth;

            if (restored > 0)
            {
                LOG_INFO("Entity healed");
            }

            return restored;
        }

        int RestoreArmor(int amount)
        {
            if (amount <= 0 || armor >= maxArmor || !IsAlive())
            {
                return 0;
            }

            const int previousArmor = armor;
            armor = amount >= maxArmor - armor ? maxArmor : armor + amount;
            const int restored = armor - previousArmor;
            if (restored > 0)
            {
                LOG_INFO("Entity shield restored");
            }

            return restored;
        }

        bool IsAlive() const
        {
            return health > 0;
        }

    private:
        int maxHealth;
        int health;
        int maxArmor;
        int armor;
    };
} // namespace XYZRoguelike
