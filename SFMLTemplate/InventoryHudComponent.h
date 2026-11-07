#pragma once

#include "Component.h"
#include "Constants.h"
#include "InventoryComponent.h"

#include <SFML/Graphics.hpp>

#include <array>
#include <deque>
#include <string>

namespace XYZRoguelike
{
    class InventoryHudComponent : public XYZEngine::Component
    {
    public:
        explicit InventoryHudComponent(XYZEngine::GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override;
        bool IsOverlay() const override
        {
            return true;
        }

        bool IsOpen() const;
        void ShowFeedback(const std::string& message, sf::Color color = sf::Color(153, 235, 181));
        void SetLevelNumber(int levelNumber);
        void ShowRunComplete();

    private:
        using SlotShapes = std::array<sf::RectangleShape, INVENTORY_SLOT_COUNT>;

        struct PopupMessage
        {
            std::string text;
            sf::Color color;
            float age = 0.f;
            float lifetime = 3.2f;
        };

        void SelectSlot(std::size_t slotIndex);
        void ShowItemDescription(ItemType type);
        void AddPopup(const std::string& message, sf::Color color, float lifetime);
        void UseSelectedItem();
        const sf::Texture* GetItemTexture(ItemType type) const;
        void DrawSlot(sf::RenderWindow& window, std::size_t index, sf::RectangleShape& slot,
                      bool showLabel, const sf::RenderStates& renderStates);

        InventoryComponent* inventory = nullptr;
        sf::Font font;
        bool fontLoaded = false;
        bool panelOpen = false;
        bool inventoryKeyWasPressed = false;
        bool closeKeyWasPressed = false;
        bool useKeyWasPressed = false;
        bool mouseWasPressed = false;
        std::array<bool, INVENTORY_SLOT_COUNT> numberKeyWasPressed{};
        float panelTransition = 0.f;
        float panelLayoutScale = 1.f;
        int currentLevel = 1;
        std::deque<PopupMessage> popups;

        sf::Sprite inventoryBackground;
        sf::Sprite playerPortrait;
        sf::Sprite popupBackground;
        SlotShapes hotbarSlots;
        SlotShapes inventorySlots;
    };
} // namespace XYZRoguelike
