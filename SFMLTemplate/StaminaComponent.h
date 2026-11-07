#pragma once

#include "Component.h"

namespace XYZRoguelike
{
    class StaminaComponent : public XYZEngine::Component
    {
    public:
        explicit StaminaComponent(XYZEngine::GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override;

        bool Consume(float amount);
        bool ApplyStaminaPotion();
        float GetCurrentStamina() const;
        float GetMaxStamina() const;
        float GetNoDrainTimeRemaining() const;

    private:
        float currentStamina;
        float regenerationDelayRemaining = 0.f;
        float noDrainTimeRemaining = 0.f;
    };
} // namespace XYZRoguelike
