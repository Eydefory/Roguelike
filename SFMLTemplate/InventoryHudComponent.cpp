#include "InventoryHudComponent.h"

#include "Constants.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "Loger.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <algorithm>
#include <cmath>
#include <string>

namespace XYZRoguelike
{
    namespace
    {
        constexpr float HOTBAR_SLOT_SIZE = 56.f;
        constexpr float HOTBAR_SLOT_GAP = 5.f;
        constexpr float INVENTORY_SLOT_SIZE = 92.f;
        constexpr float INVENTORY_SLOT_STEP_X = 123.f;
        constexpr float INVENTORY_GRID_START_X =
            (1024.f -
             (6.f * INVENTORY_SLOT_SIZE + 5.f * (INVENTORY_SLOT_STEP_X - INVENTORY_SLOT_SIZE))) *
            0.5f;
        constexpr float INVENTORY_GRID_START_Y = 376.f;
        constexpr float POPUP_LIFETIME = 3.2f;
        constexpr float POPUP_WIDTH = 290.f;
        constexpr float POPUP_HEIGHT = 145.f;

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

        std::string GetSlotKeyLabel(std::size_t index)
        {
            if (index < 9)
            {
                return std::to_string(index + 1);
            }
            if (index == 9)
            {
                return "0";
            }
            return index == 10 ? "-" : "=";
        }

        std::string WrapPopupText(const sf::Font& font, const std::string& value,
                                  unsigned int characterSize, float maxWidth)
        {
            std::string wrapped;
            std::string line;
            std::size_t wordStart = 0;
            while (wordStart < value.size())
            {
                const std::size_t wordEnd = value.find(' ', wordStart);
                const std::string word =
                    value.substr(wordStart, wordEnd == std::string::npos ? std::string::npos
                                                                         : wordEnd - wordStart);
                const std::string candidate = line.empty() ? word : line + " " + word;
                sf::Text measure(candidate, font, characterSize);
                if (!line.empty() && measure.getLocalBounds().width > maxWidth)
                {
                    wrapped += line + "\n";
                    line = word;
                }
                else
                {
                    line = candidate;
                }
                if (wordEnd == std::string::npos)
                {
                    break;
                }
                wordStart = wordEnd + 1;
            }
            return wrapped + line;
        }
    } // namespace

    InventoryHudComponent::InventoryHudComponent(XYZEngine::GameObject* gameObject)
        : Component(gameObject), inventory(gameObject->GetComponent<InventoryComponent>())
    {
        auto* resources = XYZEngine::ResourceSystem::Instance();
        if (const auto* texture = resources->GetTextureShared("inventory_panel"))
        {
            inventoryBackground.setTexture(*texture);
            constexpr float panelWidth = 900.f;
            const auto textureSize = texture->getSize();
            const float scale = panelWidth / static_cast<float>(textureSize.x);
            panelLayoutScale = panelWidth / 1024.f;
            inventoryBackground.setScale(scale, scale);
            inventoryBackground.setPosition((1280.f - panelWidth) * 0.5f,
                                            (720.f - textureSize.y * scale) * 0.5f);
        }

        if (const auto* texture = resources->GetTextureShared("player_idle"))
        {
            playerPortrait.setTexture(*texture);
            const float portraitSize = 168.f * panelLayoutScale;
            const auto textureSize = texture->getSize();
            const float scale =
                portraitSize / static_cast<float>(std::max(textureSize.x, textureSize.y));
            playerPortrait.setScale(scale, scale);
            playerPortrait.setPosition(inventoryBackground.getPosition() +
                                       sf::Vector2f(217.f, 202.f) * panelLayoutScale -
                                       sf::Vector2f(portraitSize * 0.5f, portraitSize * 0.5f));
        }

        const float hotbarWidth =
            INVENTORY_SLOT_COUNT * HOTBAR_SLOT_SIZE + (INVENTORY_SLOT_COUNT - 1) * HOTBAR_SLOT_GAP;
        const float hotbarStartX = (1280.f - hotbarWidth) * 0.5f;
        for (std::size_t index = 0; index < INVENTORY_SLOT_COUNT; ++index)
        {
            const sf::Vector2f position(hotbarStartX + index * (HOTBAR_SLOT_SIZE + HOTBAR_SLOT_GAP),
                                        650.f);
            hotbarSlots[index].setSize(sf::Vector2f(HOTBAR_SLOT_SIZE, HOTBAR_SLOT_SIZE));
            hotbarSlots[index].setPosition(position);

            const std::size_t column = index % 6;
            const std::size_t row = index / 6;
            const float slotSize = INVENTORY_SLOT_SIZE * panelLayoutScale;
            inventorySlots[index].setSize(sf::Vector2f(slotSize, slotSize));
            inventorySlots[index].setPosition(
                inventoryBackground.getPosition() +
                sf::Vector2f(INVENTORY_GRID_START_X + column * INVENTORY_SLOT_STEP_X,
                             INVENTORY_GRID_START_Y + row * 120.f) *
                    panelLayoutScale);
        }

        if (const auto* texture = resources->GetTextureShared("slot_empty"))
        {
            for (auto& slot : hotbarSlots)
            {
                slot.setTexture(texture);
            }
            for (auto& slot : inventorySlots)
            {
                slot.setTexture(texture);
            }
        }

        if (const auto* texture = resources->GetTextureShared("popup_message"))
        {
            popupBackground.setTexture(*texture);
            const float scale = POPUP_WIDTH / static_cast<float>(texture->getSize().x);
            popupBackground.setScale(scale, scale);
        }

        fontLoaded = font.loadFromFile("Resources/Fonts/Saltblackcombat-Regular.ttf");
        if (!fontLoaded)
        {
            LOG_ERROR("Inventory HUD font could not be loaded");
        }
    }

    void InventoryHudComponent::Update(float deltaTime)
    {
        const bool inputEnabled = !XYZEngine::GameWorld::Instance()->IsPaused();

        const bool inventoryKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::I);
        if (inputEnabled && inventoryKeyPressed && !inventoryKeyWasPressed)
        {
            panelOpen = !panelOpen;
        }
        inventoryKeyWasPressed = inventoryKeyPressed;

        const bool closeKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
        if (inputEnabled && closeKeyPressed && !closeKeyWasPressed && panelOpen)
        {
            panelOpen = false;
        }
        closeKeyWasPressed = closeKeyPressed;

        static constexpr std::array<sf::Keyboard::Key, INVENTORY_SLOT_COUNT> numberKeys = {
            sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3,   sf::Keyboard::Num4,
            sf::Keyboard::Num5, sf::Keyboard::Num6, sf::Keyboard::Num7,   sf::Keyboard::Num8,
            sf::Keyboard::Num9, sf::Keyboard::Num0, sf::Keyboard::Hyphen, sf::Keyboard::Equal};
        static constexpr std::array<sf::Keyboard::Key, INVENTORY_SLOT_COUNT> numpadKeys = {
            sf::Keyboard::Numpad1, sf::Keyboard::Numpad2,  sf::Keyboard::Numpad3,
            sf::Keyboard::Numpad4, sf::Keyboard::Numpad5,  sf::Keyboard::Numpad6,
            sf::Keyboard::Numpad7, sf::Keyboard::Numpad8,  sf::Keyboard::Numpad9,
            sf::Keyboard::Numpad0, sf::Keyboard::Subtract, sf::Keyboard::Add};
        for (std::size_t index = 0; index < INVENTORY_SLOT_COUNT; ++index)
        {
            const bool pressed = sf::Keyboard::isKeyPressed(numberKeys[index]) ||
                                 sf::Keyboard::isKeyPressed(numpadKeys[index]);
            if (inputEnabled && pressed && !numberKeyWasPressed[index])
            {
                SelectSlot(index);
            }
            numberKeyWasPressed[index] = pressed;
        }

        auto& window = XYZEngine::RenderSystem::Instance()->GetMainWindow();
        const bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        if (inputEnabled && mousePressed && !mouseWasPressed)
        {
            const auto mousePixel = sf::Mouse::getPosition(window);
            const auto mousePosition = window.mapPixelToCoords(mousePixel, window.getDefaultView());
            if (panelTransition <= 0.001f)
            {
                for (std::size_t index = 0; index < INVENTORY_SLOT_COUNT; ++index)
                {
                    if (hotbarSlots[index].getGlobalBounds().contains(mousePosition))
                    {
                        SelectSlot(index);
                        break;
                    }
                }
            }
            else
            {
                const float transition = std::clamp(panelTransition, 0.f, 1.f);
                const float inverseTransition = 1.f - transition;
                const float panelOffset =
                    inverseTransition * inverseTransition * inverseTransition * SCREEN_WIDTH;
                for (std::size_t index = 0; index < INVENTORY_SLOT_COUNT; ++index)
                {
                    auto slotBounds = inventorySlots[index].getGlobalBounds();
                    slotBounds.left += panelOffset;
                    if (slotBounds.contains(mousePosition))
                    {
                        SelectSlot(index);
                        break;
                    }
                }
            }
        }
        mouseWasPressed = mousePressed;

        const bool useKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::F);
        if (inputEnabled && useKeyPressed && !useKeyWasPressed)
        {
            UseSelectedItem();
        }
        useKeyWasPressed = useKeyPressed;

        if (std::isfinite(deltaTime) && deltaTime > 0.f)
        {
            for (auto& popup : popups)
            {
                popup.age += deltaTime;
            }
            while (!popups.empty() && popups.front().age >= popups.front().lifetime)
            {
                popups.pop_front();
            }
        }

        if (std::isfinite(deltaTime) && deltaTime > 0.f)
        {
            constexpr float PANEL_ANIMATION_SECONDS = 0.32f;
            const float target = panelOpen ? 1.f : 0.f;
            const float step = deltaTime / PANEL_ANIMATION_SECONDS;
            if (panelTransition < target)
            {
                panelTransition = std::min(target, panelTransition + step);
            }
            else if (panelTransition > target)
            {
                panelTransition = std::max(target, panelTransition - step);
            }
        }
    }

    void InventoryHudComponent::Render()
    {
        auto& window = XYZEngine::RenderSystem::Instance()->GetMainWindow();
        const auto previousView = window.getView();
        window.setView(window.getDefaultView());

        if (panelTransition <= 0.001f)
        {
            for (std::size_t index = 0; index < INVENTORY_SLOT_COUNT; ++index)
            {
                DrawSlot(window, index, hotbarSlots[index], false, sf::RenderStates::Default);
            }
        }

        float animation = std::clamp(panelTransition, 0.f, 1.f);
        const float inverseAnimation = 1.f - animation;
        animation = 1.f - inverseAnimation * inverseAnimation * inverseAnimation;
        if (animation > 0.f)
        {
            sf::RenderStates panelRenderStates;
            panelRenderStates.transform.translate((1.f - animation) * SCREEN_WIDTH, 0.f);

            window.draw(inventoryBackground, panelRenderStates);
            if (playerPortrait.getTexture() != nullptr)
            {
                window.draw(playerPortrait, panelRenderStates);
            }

            if (fontLoaded)
            {
                sf::Text title("INVENTORY", font, 30);
                title.setFillColor(sf::Color(245, 241, 255));
                SetTextTopLeft(title, inventoryBackground.getPosition() +
                                          sf::Vector2f(550.f, 110.f) * panelLayoutScale);
                window.draw(title, panelRenderStates);

                sf::Text level("LEVEL " + std::to_string(currentLevel) + " / " +
                                   std::to_string(TOTAL_LEVEL_COUNT),
                               font, 18);
                level.setFillColor(sf::Color(211, 204, 231));
                const auto levelBounds = level.getLocalBounds();
                const auto panelPosition = inventoryBackground.getPosition();
                const float levelRight = 870.f * panelLayoutScale;
                SetTextTopLeft(level, sf::Vector2f(panelPosition.x + levelRight - levelBounds.width,
                                                   panelPosition.y + 100.f * panelLayoutScale));
                window.draw(level, panelRenderStates);

                sf::Text portraitCaption("HERO", font, 18);
                portraitCaption.setFillColor(sf::Color(208, 224, 174));
                SetTextCentered(portraitCaption, panelPosition.x + 217.f * panelLayoutScale,
                                panelPosition.y + 297.f * panelLayoutScale);
                window.draw(portraitCaption, panelRenderStates);

                const ItemType selectedItem =
                    inventory != nullptr ? inventory->GetSelectedItem() : ItemType::Empty;
                sf::Text handLabel("IN HAND", font, 17);
                handLabel.setFillColor(sf::Color(208, 224, 174));
                SetTextTopLeft(handLabel,
                               panelPosition + sf::Vector2f(365.f, 192.f) * panelLayoutScale);
                window.draw(handLabel, panelRenderStates);

                sf::Text handItem(GetItemDisplayName(selectedItem), font, 25);
                handItem.setFillColor(sf::Color(245, 241, 255));
                SetTextTopLeft(handItem,
                               panelPosition + sf::Vector2f(365.f, 220.f) * panelLayoutScale);
                window.draw(handItem, panelRenderStates);

                sf::Text controls("1-9, 0, - or = select   F use item   I close", font, 25.5);
                controls.setFillColor(sf::Color(183, 180, 203));
                SetTextTopLeft(controls,
                               panelPosition + sf::Vector2f(365.f, 255.f) * panelLayoutScale);
                window.draw(controls, panelRenderStates);

                sf::Text combatControls("SPACE attack   E unlock door", font, 20);
                combatControls.setFillColor(sf::Color(183, 180, 203));
                SetTextTopLeft(combatControls,
                               panelPosition + sf::Vector2f(365.f, 289.f) * panelLayoutScale);
                window.draw(combatControls, panelRenderStates);

                for (std::size_t index = 0; index < INVENTORY_SLOT_COUNT; ++index)
                {
                    DrawSlot(window, index, inventorySlots[index], true, panelRenderStates);
                }
            }
        }

        for (std::size_t index = 0; index < popups.size(); ++index)
        {
            const auto& popup = popups[index];
            const float fadeIn = std::clamp(popup.age / 0.32f, 0.f, 1.f);
            const float fadeOut = std::clamp((popup.lifetime - popup.age) / 0.42f, 0.f, 1.f);
            const float smoothFadeIn = fadeIn * fadeIn * (3.f - 2.f * fadeIn);
            const float opacity = std::min(smoothFadeIn, fadeOut);
            const float targetY = 225.f + index * 136.f;
            const sf::Vector2f position(SCREEN_WIDTH - POPUP_WIDTH - 22.f,
                                        targetY + (1.f - smoothFadeIn) * 18.f);

            if (popupBackground.getTexture() != nullptr)
            {
                popupBackground.setPosition(position);
                popupBackground.setColor(
                    sf::Color(255, 255, 255, static_cast<sf::Uint8>(255.f * opacity)));
                window.draw(popupBackground);
            }

            if (fontLoaded)
            {
                constexpr unsigned int POPUP_TEXT_SIZE = 18;
                sf::Text message(popup.text, font, POPUP_TEXT_SIZE);
                sf::Color messageColor = popup.color;
                messageColor.a = static_cast<sf::Uint8>(255.f * opacity);
                message.setFillColor(messageColor);
                const auto bounds = message.getLocalBounds();
                message.setPosition(position.x + (POPUP_WIDTH - bounds.width) * 0.5f - bounds.left,
                                    position.y + (POPUP_HEIGHT - bounds.height) * 0.5f -
                                        bounds.top);
                window.draw(message);
            }
        }

        window.setView(previousView);
    }

    bool InventoryHudComponent::IsOpen() const
    {
        return panelOpen || panelTransition > 0.001f;
    }

    void InventoryHudComponent::ShowFeedback(const std::string& message, sf::Color color)
    {
        AddPopup(message, color, POPUP_LIFETIME);
    }

    void InventoryHudComponent::AddPopup(const std::string& message, sf::Color color,
                                         float lifetime)
    {
        const std::string displayedMessage =
            fontLoaded ? WrapPopupText(font, message, 27.5f, 350.f) : message;
        popups.push_back({displayedMessage, color, 0.f, lifetime});
        while (popups.size() > 3)
        {
            popups.pop_front();
        }
    }

    void InventoryHudComponent::ShowItemDescription(ItemType type)
    {
        std::string description;
        switch (type)
        {
        case ItemType::Sword:
            description = "Melee weapon. Press SPACE near an enemy to attack.";
            break;
        case ItemType::Bow:
            description = "Ranged weapon. Press SPACE to fire toward the direction you face.";
            break;
        case ItemType::HealingPotion:
            description = "Restores 40 health points.";
            break;
        case ItemType::ShieldPotion:
            description = "Restores lost shield charges.";
            break;
        case ItemType::StaminaPotion:
            description = "Restores stamina and prevents drain for 5 seconds.";
            break;
        case ItemType::Key:
            description = "Use this key at the exit door to unlock the next level.";
            break;
        case ItemType::Empty:
        default:
            return;
        }
        AddPopup(description, sf::Color(235, 230, 250), 7.5f);
    }

    void InventoryHudComponent::SetLevelNumber(int levelNumber)
    {
        currentLevel = std::clamp(levelNumber, 1, TOTAL_LEVEL_COUNT);
    }

    void InventoryHudComponent::ShowRunComplete()
    {
        panelOpen = false;
        panelTransition = 0.f;
    }

    void InventoryHudComponent::SelectSlot(std::size_t slotIndex)
    {
        if (inventory == nullptr || !inventory->SelectSlot(slotIndex))
        {
            return;
        }

        const ItemType selectedItem = inventory->GetSelectedItem();
        if (selectedItem == ItemType::Empty)
        {
            ShowFeedback("Hands are empty.");
            return;
        }
        ShowItemDescription(selectedItem);
    }

    void InventoryHudComponent::UseSelectedItem()
    {
        if (inventory == nullptr)
        {
            return;
        }

        const ItemType selectedItem = inventory->GetSelectedItem();
        if (inventory->UseSelectedItem())
        {
            switch (selectedItem)
            {
            case ItemType::HealingPotion:
                ShowFeedback("Health restored by 40.");
                break;
            case ItemType::ShieldPotion:
                ShowFeedback("Shield restored.");
                break;
            case ItemType::StaminaPotion:
                ShowFeedback("Stamina restored. Sprint is free for 5 seconds.");
                break;
            default:
                break;
            }
            return;
        }

        switch (selectedItem)
        {
        case ItemType::HealingPotion:
            ShowFeedback("Health is already full.", sf::Color(255, 206, 135));
            break;
        case ItemType::ShieldPotion:
            ShowFeedback("Shield is already full.", sf::Color(255, 206, 135));
            break;
        case ItemType::StaminaPotion:
            ShowFeedback("Stamina boost is already active.", sf::Color(255, 206, 135));
            break;
        case ItemType::Sword:
        case ItemType::Bow:
            ShowFeedback("Use SPACE to attack with the selected weapon.", sf::Color(221, 219, 234));
            break;
        case ItemType::Key:
            ShowFeedback("Use the selected key at a door with E.", sf::Color(221, 219, 234));
            break;
        case ItemType::Empty:
        default:
            ShowFeedback("This slot is empty.", sf::Color(255, 206, 135));
            break;
        }
    }

    const sf::Texture* InventoryHudComponent::GetItemTexture(ItemType type) const
    {
        const char* name = GetItemTextureName(type);
        return name[0] != '\0' ? XYZEngine::ResourceSystem::Instance()->GetTextureShared(name)
                               : nullptr;
    }

    void InventoryHudComponent::DrawSlot(sf::RenderWindow& window, std::size_t index,
                                         sf::RectangleShape& slot, bool showLabel,
                                         const sf::RenderStates& renderStates)
    {
        if (inventory == nullptr || index >= INVENTORY_SLOT_COUNT)
        {
            return;
        }

        const auto* selectedTexture = XYZEngine::ResourceSystem::Instance()->GetTextureShared(
            index == inventory->GetSelectedSlot() ? "slot_selected" : "slot_empty");
        if (selectedTexture != nullptr)
        {
            slot.setTexture(selectedTexture);
        }
        window.draw(slot, renderStates);

        const auto& item = inventory->GetItems()[index];
        if (item.quantity > 0 && item.type != ItemType::Empty)
        {
            const sf::Texture* texture = GetItemTexture(item.type);
            if (texture != nullptr)
            {
                sf::Sprite icon(*texture);
                const auto textureSize = texture->getSize();
                const float targetSize = showLabel ? slot.getSize().x * 0.48f : 36.f;
                const float scale =
                    targetSize / static_cast<float>(std::max(textureSize.x, textureSize.y));
                icon.setScale(scale, scale);
                const auto bounds = icon.getLocalBounds();
                icon.setPosition(
                    slot.getPosition().x + (slot.getSize().x - bounds.width * scale) * 0.5f,
                    slot.getPosition().y + (slot.getSize().y - bounds.height * scale) * 0.5f);
                window.draw(icon, renderStates);
            }

            if (fontLoaded && (item.quantity > 1 || item.type == ItemType::Key))
            {
                sf::Text quantity("x" + std::to_string(item.quantity), font, showLabel ? 11 : 10);
                quantity.setFillColor(sf::Color(246, 239, 255));
                const auto bounds = quantity.getLocalBounds();
                SetTextTopLeft(
                    quantity,
                    sf::Vector2f(slot.getPosition().x + slot.getSize().x - bounds.width - 6.f,
                                 slot.getPosition().y + slot.getSize().y - bounds.height - 5.f));
                window.draw(quantity, renderStates);
            }
        }

        if (fontLoaded)
        {
            sf::Text number(GetSlotKeyLabel(index), font, showLabel ? 11 : 10);
            number.setFillColor(sf::Color(212, 204, 232));
            SetTextTopLeft(number, slot.getPosition() + sf::Vector2f(7.f, 3.f));
            window.draw(number, renderStates);
        }
    }
} // namespace XYZRoguelike
