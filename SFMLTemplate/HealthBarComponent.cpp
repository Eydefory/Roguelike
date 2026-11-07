#include "HealthBarComponent.h"

#include "GameObject.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"

#include <algorithm>

namespace XYZRoguelike
{
    namespace
    {
        constexpr float BAR_WIDTH = 320.f;
        constexpr float BAR_HEIGHT = 70.f;
        constexpr float BAR_FILL_WIDTH = 240.f;
        constexpr float BAR_FILL_HEIGHT = 29.f;
        constexpr float BAR_LEFT = 24.f;
        constexpr float HEALTH_BAR_TOP = 32.f;
        constexpr float SPRINT_BAR_TOP = 108.f;
        constexpr float SHIELD_ROW_TOP = 184.f;
        constexpr float BAR_SCALE = BAR_WIDTH / 512.f;
        constexpr float BAR_FILL_LEFT = BAR_LEFT + 100.f * BAR_SCALE;
        constexpr float HEALTH_FILL_TOP = HEALTH_BAR_TOP + 32.f * BAR_SCALE;
        constexpr float SPRINT_FILL_TOP = SPRINT_BAR_TOP + 32.f * BAR_SCALE;

        void SetFillRect(sf::Sprite& fill, float percent)
        {
            const auto* texture = fill.getTexture();
            if (texture == nullptr)
            {
                return;
            }

            const int textureWidth = static_cast<int>(texture->getSize().x);
            const int textureHeight = static_cast<int>(texture->getSize().y);
            fill.setTextureRect(sf::IntRect(
                0, 0, std::max(0, static_cast<int>(textureWidth * percent)), textureHeight));
        }
    } // namespace

    HealthBarComponent::HealthBarComponent(XYZEngine::GameObject* gameObject)
        : Component(gameObject), healthComponent(nullptr), staminaComponent(nullptr)
    {
        auto* resources = XYZEngine::ResourceSystem::Instance();
        if (const auto* frame = resources->GetTextureShared("health_bar_frame"))
        {
            healthBarFrame.setTexture(*frame);
            healthBarFrame.setScale(BAR_WIDTH / frame->getSize().x,
                                    BAR_HEIGHT / frame->getSize().y);
            healthBarFrame.setPosition(BAR_LEFT, HEALTH_BAR_TOP);
        }
        if (const auto* fill = resources->GetTextureShared("health_fill_red"))
        {
            healthFill.setTexture(*fill);
            healthFill.setScale(BAR_FILL_WIDTH / fill->getSize().x,
                                BAR_FILL_HEIGHT / fill->getSize().y);
            healthFill.setPosition(BAR_FILL_LEFT, HEALTH_FILL_TOP);
        }
        if (const auto* frame = resources->GetTextureShared("sprint_bar_frame"))
        {
            sprintBarFrame.setTexture(*frame);
            sprintBarFrame.setScale(BAR_WIDTH / frame->getSize().x,
                                    BAR_HEIGHT / frame->getSize().y);
            sprintBarFrame.setPosition(BAR_LEFT, SPRINT_BAR_TOP);
        }
        if (const auto* fill = resources->GetTextureShared("sprint_fill_green"))
        {
            sprintFill.setTexture(*fill);
            sprintFill.setScale(BAR_FILL_WIDTH / fill->getSize().x,
                                BAR_FILL_HEIGHT / fill->getSize().y);
            sprintFill.setPosition(BAR_FILL_LEFT, SPRINT_FILL_TOP);
        }

        if (const auto* icon = resources->GetTextureShared("shield_icon"))
        {
            for (std::size_t i = 0; i < shieldIcons.size(); ++i)
            {
                shieldIcons[i].setTexture(*icon);
                shieldIcons[i].setScale(0.8f, 0.8f);
                shieldIcons[i].setPosition(75.f + i * 44.f, SHIELD_ROW_TOP);
            }
        }

        healthComponent = gameObject->GetComponent<HealthComponent>();
        staminaComponent = gameObject->GetComponent<StaminaComponent>();

        UpdateBars();
    }

    void HealthBarComponent::Update(float deltaTime)
    {
        UpdateBars();
    }

    void HealthBarComponent::Render()
    {
        auto& window = XYZEngine::RenderSystem::Instance()->GetMainWindow();
        const auto previousView = window.getView();
        window.setView(window.getDefaultView());

        // The frames contain an opaque dark track, so draw the fills over it.
        window.draw(healthBarFrame);
        window.draw(healthFill);
        window.draw(sprintBarFrame);
        window.draw(sprintFill);

        const int shieldCount = healthComponent != nullptr ? healthComponent->GetArmor() : 0;
        for (int i = 0; i < shieldCount && i < static_cast<int>(shieldIcons.size()); ++i)
        {
            window.draw(shieldIcons[static_cast<std::size_t>(i)]);
        }

        window.setView(previousView);
    }

    void HealthBarComponent::UpdateBars()
    {
        const float healthPercent =
            healthComponent != nullptr && healthComponent->GetMaxHealth() > 0
                ? std::clamp(static_cast<float>(healthComponent->GetHealth()) /
                                 healthComponent->GetMaxHealth(),
                             0.f, 1.f)
                : 0.f;
        const float maxStamina =
            staminaComponent != nullptr ? staminaComponent->GetMaxStamina() : 0.f;
        const float staminaPercent =
            maxStamina > 0.f
                ? std::clamp(staminaComponent->GetCurrentStamina() / maxStamina, 0.f, 1.f)
                : 0.f;

        SetFillRect(healthFill, healthPercent);
        SetFillRect(sprintFill, staminaPercent);
    }
} // namespace XYZRoguelike
