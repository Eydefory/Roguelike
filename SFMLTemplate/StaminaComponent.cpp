#include "StaminaComponent.h"

#include "Constants.h"
#include "GameObject.h"
#include "InventoryHudComponent.h"
#include "Loger.h"

#include <algorithm>
#include <cmath>

namespace XYZRoguelike
{
    StaminaComponent::StaminaComponent(XYZEngine::GameObject* gameObject)
        : Component(gameObject), currentStamina(PLAYER_MAX_STAMINA)
    {
    }

    void StaminaComponent::Update(float deltaTime)
    {
        auto* inventoryHud = gameObject->GetComponent<InventoryHudComponent>();
        if (inventoryHud != nullptr && inventoryHud->IsOpen())
        {
            return;
        }

        if (!std::isfinite(deltaTime) || deltaTime <= 0.f)
        {
            return;
        }

        noDrainTimeRemaining = std::max(0.f, noDrainTimeRemaining - deltaTime);
        if (currentStamina >= PLAYER_MAX_STAMINA)
        {
            return;
        }

        float regenerationTime = deltaTime;
        if (regenerationDelayRemaining > 0.f)
        {
            regenerationTime = std::max(0.f, deltaTime - regenerationDelayRemaining);
            regenerationDelayRemaining = std::max(0.f, regenerationDelayRemaining - deltaTime);
        }

        currentStamina =
            std::min(PLAYER_MAX_STAMINA,
                     currentStamina + PLAYER_STAMINA_REGEN_PER_SECOND * regenerationTime);
    }

    void StaminaComponent::Render() {}

    bool StaminaComponent::Consume(float amount)
    {
        if (!std::isfinite(amount) || amount <= 0.f)
        {
            return false;
        }

        if (noDrainTimeRemaining > 0.f)
        {
            return true;
        }

        if (currentStamina <= 0.f)
        {
            return false;
        }

        currentStamina = std::max(0.f, currentStamina - amount);
        regenerationDelayRemaining = PLAYER_STAMINA_REGEN_DELAY;
        return true;
    }

    float StaminaComponent::GetCurrentStamina() const
    {
        return currentStamina;
    }

    float StaminaComponent::GetMaxStamina() const
    {
        return PLAYER_MAX_STAMINA;
    }

    bool StaminaComponent::ApplyStaminaPotion()
    {
        const bool staminaRestored = currentStamina < PLAYER_MAX_STAMINA;
        const bool buffExtended = noDrainTimeRemaining < PLAYER_STAMINA_POTION_DURATION;
        if (!staminaRestored && !buffExtended)
        {
            return false;
        }

        currentStamina = PLAYER_MAX_STAMINA;
        noDrainTimeRemaining = PLAYER_STAMINA_POTION_DURATION;
        regenerationDelayRemaining = 0.f;
        LOG_INFO("Stamina potion applied");
        return true;
    }

    float StaminaComponent::GetNoDrainTimeRemaining() const
    {
        return noDrainTimeRemaining;
    }
} // namespace XYZRoguelike
