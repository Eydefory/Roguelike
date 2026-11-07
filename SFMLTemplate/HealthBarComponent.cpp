#include "HealthBarComponent.h"

#include "Constants.h"
#include "GameObject.h"
#include "Loger.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"

#include <algorithm>
#include <cmath>
#include <string>

namespace XYZRoguelike
{
    namespace
    {
        constexpr float BAR_WIDTH = 320.f;
        constexpr float BAR_HEIGHT = 70.f;
        constexpr float BAR_FILL_WIDTH = 240.f;
        constexpr float BAR_FILL_HEIGHT = 29.f;
        constexpr float STAMINA_Y = 168.f;
        constexpr float BAR_LEFT = 24.f;
        constexpr float BAR_TOP = 32.f;
        constexpr float BAR_SCALE = BAR_WIDTH / 512.f;
        constexpr float HEALTH_FILL_LEFT = BAR_LEFT + 100.f * BAR_SCALE;
        constexpr float HEALTH_FILL_TOP = BAR_TOP + 32.f * BAR_SCALE;

        void SetTextTopLeft(sf::Text& text, const sf::Vector2f& topLeft)
        {
            const auto bounds = text.getLocalBounds();
            text.setPosition(topLeft.x - bounds.left, topLeft.y - bounds.top);
        }
    } // namespace

    HealthBarComponent::HealthBarComponent(XYZEngine::GameObject* gameObject)
        : Component(gameObject), healthComponent(nullptr), staminaComponent(nullptr)
    {
        auto* resources = XYZEngine::ResourceSystem::Instance();
        if (const auto* frame = resources->GetTextureShared("health_bar_frame"))
        {
            healthBarFrame.setTexture(*frame);
            healthBarFrame.setScale(BAR_WIDTH / frame->getSize().x, BAR_HEIGHT / frame->getSize().y);
            healthBarFrame.setPosition(24.f, 32.f);
        }
        if (const auto* fill = resources->GetTextureShared("health_fill_red"))
        {
            healthFill.setTexture(*fill);
            healthFill.setScale(BAR_FILL_WIDTH / fill->getSize().x,
                                BAR_FILL_HEIGHT / fill->getSize().y);
            healthFill.setPosition(HEALTH_FILL_LEFT, HEALTH_FILL_TOP);
        }
        if (const auto* icon = resources->GetTextureShared("shield_icon"))
        {
            for (std::size_t i = 0; i < shieldIcons.size(); ++i)
            {
                shieldIcons[i].setTexture(*icon);
                shieldIcons[i].setScale(0.8f, 0.8f);
                shieldIcons[i].setPosition(75.f + i * 44.f, 108.f);
            }
        }

        for (std::size_t index = 0; index < meterFrames.size(); ++index)
        {
            meterFrames[index].setSize(sf::Vector2f(BAR_WIDTH, BAR_HEIGHT));
            meterFrames[index].setPosition(24.f, 32.f + index * 75.f);
            meterFrames[index].setFillColor(sf::Color::Transparent);
            meterFrames[index].setOutlineThickness(0.f);
            meterFills[index].setSize(sf::Vector2f(BAR_WIDTH - 28.f, BAR_FILL_HEIGHT));
            meterFills[index].setPosition(37.f, index == 2 ? STAMINA_Y : 56.f);
        }

        meterFills[0].setFillColor(sf::Color::White);
        meterFills[1].setFillColor(sf::Color(68, 160, 208));
        meterFills[2].setFillColor(sf::Color(83, 189, 132));

        healthComponent = gameObject->GetComponent<HealthComponent>();
        staminaComponent = gameObject->GetComponent<StaminaComponent>();

        fontLoaded = font.loadFromFile("Resources/Fonts/Saltblackcombat-Regular.ttf");
        if (!fontLoaded)
        {
            LOG_ERROR("Player HUD font could not be loaded");
        }
        else
        {
            title.setFont(font);
            title.setCharacterSize(20);
            title.setFillColor(sf::Color(242, 235, 253));
            SetTextTopLeft(title, sf::Vector2f(32.f, 22.f));

            for (std::size_t index = 0; index < meterLabels.size(); ++index)
            {
                auto& label = meterLabels[index];
                label.setFont(font);
                label.setCharacterSize(17);
                label.setFillColor(sf::Color(250, 248, 255));
            }
        }

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

        window.draw(healthBarFrame);
        window.draw(healthFill);
        const int shieldCount = healthComponent != nullptr ? healthComponent->GetArmor() : 0;
        for (int i = 0; i < shieldCount && i < static_cast<int>(shieldIcons.size()); ++i)
        {
            window.draw(shieldIcons[static_cast<std::size_t>(i)]);
        }
        if (fontLoaded)
        {
            window.draw(title);
        }
        for (std::size_t index = 0; index < meterFrames.size(); ++index)
        {
            if (fontLoaded && index != 1)
            {
                window.draw(meterLabels[index]);
            }
        }

        window.setView(previousView);
    }

    void HealthBarComponent::SetLevelNumber(int levelNumber)
    {
        currentLevel = std::clamp(levelNumber, 1, TOTAL_LEVEL_COUNT);
        UpdateBars();
    }

    void HealthBarComponent::UpdateBars()
    {
        const float healthPercent =
            healthComponent != nullptr && healthComponent->GetMaxHealth() > 0
                ? std::clamp(static_cast<float>(healthComponent->GetHealth()) /
                                 healthComponent->GetMaxHealth(),
                             0.f, 1.f)
                : 0.f;
        const float shieldPercent =
            healthComponent != nullptr && healthComponent->GetMaxArmor() > 0
                ? std::clamp(static_cast<float>(healthComponent->GetArmor()) /
                                 healthComponent->GetMaxArmor(),
                             0.f, 1.f)
                : 0.f;
        const float maxStamina =
            staminaComponent != nullptr ? staminaComponent->GetMaxStamina() : 0.f;
        const float staminaPercent =
            maxStamina > 0.f
                ? std::clamp(staminaComponent->GetCurrentStamina() / maxStamina, 0.f, 1.f)
                : 0.f;

        const auto* fillTexture = healthFill.getTexture();
        healthFill.setTextureRect(
            sf::IntRect(0, 0, std::max(0, static_cast<int>(512.f * healthPercent)),
                        fillTexture != nullptr ? static_cast<int>(fillTexture->getSize().y) : 0));
        meterFills[0].setSize(sf::Vector2f((BAR_WIDTH - 28.f) * healthPercent, BAR_FILL_HEIGHT));
        meterFills[1].setSize(sf::Vector2f((BAR_WIDTH - 28.f) * shieldPercent, BAR_FILL_HEIGHT));
        meterFills[2].setSize(sf::Vector2f((BAR_WIDTH - 28.f) * staminaPercent, BAR_FILL_HEIGHT));


        if (!fontLoaded)
        {
            return;
        }
        SetTextTopLeft(title, sf::Vector2f(32.f, 22.f));




        for (std::size_t index = 0; index < meterLabels.size(); ++index)
        {
            auto& label = meterLabels[index];
            label.setCharacterSize(17);
            const auto bounds = label.getLocalBounds();
            const float maxTextWidth = BAR_WIDTH - 34.f;
            if (bounds.width > maxTextWidth && bounds.width > 0.f)
            {
                const auto fittedSize = static_cast<unsigned int>(
                    std::max(10.f, std::floor(label.getCharacterSize() * maxTextWidth / bounds.width)));
                label.setCharacterSize(fittedSize);
            }
            const auto fittedBounds = label.getLocalBounds();
            const float labelTop = (index == 2 ? STAMINA_Y : HEALTH_FILL_TOP) +
                                   (BAR_FILL_HEIGHT - fittedBounds.height) * 0.5f;
            SetTextTopLeft(label, sf::Vector2f(index == 2 ? 38.f : 96.f, labelTop));
        }
    }
} // namespace XYZRoguelike
