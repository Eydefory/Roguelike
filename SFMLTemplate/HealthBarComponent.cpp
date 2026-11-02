#include "HealthBarComponent.h"
#include "RenderSystem.h"
#include "GameObject.h"
namespace XYZRoguelike
{
    HealthBarComponent::HealthBarComponent(
        XYZEngine::GameObject* gameObject)
        : Component(gameObject),
        healthComponent(nullptr)
    {
        background.setSize(sf::Vector2f(300.f, 24.f));
        background.setFillColor(sf::Color(40, 40, 40));
        background.setPosition(20.f, 20.f);

        healthBar.setSize(sf::Vector2f(300.f, 24.f));
        healthBar.setFillColor(sf::Color(190, 40, 40));
        healthBar.setPosition(20.f, 20.f);

        armorBar.setSize(sf::Vector2f(300.f, 5.f));
        armorBar.setFillColor(sf::Color(60, 150, 230));
        armorBar.setPosition(20.f, 45.f);

        healthComponent =
            gameObject->GetComponent<HealthComponent>();
    }

    void HealthBarComponent::Update(float deltaTime)
    {
        UpdateBars();
    }

    void HealthBarComponent::Render()
    {
        auto& window =
            XYZEngine::RenderSystem::Instance()->GetMainWindow();

        window.draw(background);
        window.draw(healthBar);
        window.draw(armorBar);
    }

    void HealthBarComponent::UpdateBars()
    {
        if (healthComponent == nullptr)
            return;

        float healthPercent =
            static_cast<float>(healthComponent->GetHealth()) /
            healthComponent->GetMaxHealth();

        healthPercent = std::max(0.f, std::min(1.f, healthPercent));

        healthBar.setSize(
            sf::Vector2f(300.f * healthPercent, 24.f));

        float armorPercent =
            static_cast<float>(healthComponent->GetArmor()) / 25.f;

        armorPercent = std::max(0.f, std::min(1.f, armorPercent));

        armorBar.setSize(
            sf::Vector2f(300.f * armorPercent, 5.f));
    }
}