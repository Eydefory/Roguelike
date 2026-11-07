#include "QuestHudComponent.h"

#include "Constants.h"
#include "EnemyContainer.h"
#include "GameObject.h"
#include "Loger.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"

#include <algorithm>
#include <string>

namespace XYZRoguelike
{
    namespace
    {
        constexpr float PANEL_WIDTH = 312.f;
        constexpr float PANEL_X = SCREEN_WIDTH - PANEL_WIDTH - 14.f;
        constexpr float PANEL_Y = 10.f;
        constexpr float ROW_START_Y = 75.f;
        constexpr float ROW_SPACING = 34.f;

        void SetTextTopLeft(sf::Text& text, const sf::Vector2f& topLeft)
        {
            const auto bounds = text.getLocalBounds();
            text.setPosition(topLeft.x - bounds.left, topLeft.y - bounds.top);
        }

        void SetTextCentered(sf::Text& text, float centerX, float topY)
        {
            const auto bounds = text.getLocalBounds();
            text.setPosition(centerX - bounds.left - bounds.width * 0.5f, topY - bounds.top);
        }

        void SetTextRightAligned(sf::Text& text, float rightX, float topY)
        {
            const auto bounds = text.getLocalBounds();
            text.setPosition(rightX - bounds.left - bounds.width, topY - bounds.top);
        }
    } // namespace

    QuestHudComponent::QuestHudComponent(XYZEngine::GameObject* gameObject)
        : Component(gameObject), inventory(gameObject->GetComponent<InventoryComponent>())
    {
        if (const auto* texture =
                XYZEngine::ResourceSystem::Instance()->GetTextureShared("task_table"))
        {
            panel.setTexture(*texture);
            const auto textureSize = texture->getSize();
            const float scale = PANEL_WIDTH / static_cast<float>(textureSize.x);
            panel.setScale(scale, scale);
            panel.setPosition(PANEL_X, PANEL_Y);
        }
        else
        {
            LOG_ERROR("Quest HUD task table texture could not be loaded");
        }

        fontLoaded = font.loadFromFile("Resources/Fonts/Saltblackcombat-Regular.ttf");
        if (!fontLoaded)
        {
            LOG_ERROR("Quest HUD font could not be loaded");
            return;
        }

        title.setFont(font);
        title.setCharacterSize(16);
        title.setFillColor(sf::Color(244, 239, 255));

        const std::array<const char*, 3> labels = {"DEFEAT ENEMIES", "FIND THE KEY",
                                                   "OPEN THE EXIT"};
        for (std::size_t index = 0; index < objectives.size(); ++index)
        {
            objectives[index].setFont(font);
            objectives[index].setCharacterSize(25);
            objectives[index].setFillColor(sf::Color(225, 222, 238));

            statuses[index].setFont(font);
            statuses[index].setCharacterSize(25);
            statuses[index].setFillColor(sf::Color(198, 237, 185));
            objectives[index].setString(labels[index]);
        }

        RefreshText();
    }

    void QuestHudComponent::Update(float deltaTime)
    {
        RefreshText();
    }

    void QuestHudComponent::Render()
    {
        auto& window = XYZEngine::RenderSystem::Instance()->GetMainWindow();
        const auto previousView = window.getView();
        window.setView(window.getDefaultView());

        if (panel.getTexture() != nullptr)
        {
            window.draw(panel);
        }
        if (fontLoaded)
        {
            window.draw(title);
            for (std::size_t index = 0; index < objectives.size(); ++index)
            {
                window.draw(objectives[index]);
                window.draw(statuses[index]);
            }
        }

        window.setView(previousView);
    }

    void QuestHudComponent::SetEnemyContainer(EnemyContainer* container)
    {
        enemies = container;
        RefreshText();
    }

    void QuestHudComponent::SetLevelNumber(int levelNumber)
    {
        currentLevel = std::clamp(levelNumber, 1, TOTAL_LEVEL_COUNT);
        RefreshText();
    }

    void QuestHudComponent::RefreshText()
    {
        if (!fontLoaded)
        {
            return;
        }

        const std::size_t remaining = enemies != nullptr ? enemies->GetEnemyCount() : 0;
        const std::size_t total = enemies != nullptr ? enemies->GetTotalEnemyCount() : 0;
        const std::size_t defeated = total >= remaining ? total - remaining : 0;
        const bool hasKey = inventory != nullptr && inventory->HasItem(ItemType::Key);

        title.setString("LEVEL " + std::to_string(currentLevel) + " - TASKS");
        SetTextCentered(title, PANEL_X + PANEL_WIDTH * 0.48f, PANEL_Y + 25.f);
        statuses[0].setString(std::to_string(defeated) + "/" + std::to_string(total));
        statuses[0].setFillColor(remaining == 0 ? sf::Color(173, 234, 172)
                                                : sf::Color(232, 221, 177));
        statuses[1].setString(hasKey ? "DONE" : (remaining == 0 ? "READY" : "WAIT"));
        statuses[1].setFillColor(hasKey ? sf::Color(173, 234, 172) : sf::Color(232, 221, 177));
        statuses[2].setString(hasKey ? "READY" : "LOCKED");
        statuses[2].setFillColor(hasKey ? sf::Color(173, 234, 172) : sf::Color(177, 174, 192));

        for (std::size_t index = 0; index < objectives.size(); ++index)
        {
            const float rowTop = PANEL_Y + ROW_START_Y + index * ROW_SPACING;
            SetTextTopLeft(objectives[index], sf::Vector2f(PANEL_X + 30.f, rowTop));
            SetTextRightAligned(statuses[index], PANEL_X + PANEL_WIDTH - 30.f, rowTop);
        }
    }
} // namespace XYZRoguelike
