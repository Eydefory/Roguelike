#include "DoorComponent.h"

#include "Constants.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "InventoryComponent.h"
#include "InventoryHudComponent.h"
#include "TransformComponent.h"

#include <SFML/Window/Keyboard.hpp>

namespace XYZRoguelike
{
    DoorComponent::DoorComponent(XYZEngine::GameObject* gameObject, XYZEngine::GameObject* player,
                                 std::function<void()> onOpened)
        : Component(gameObject), player(player), onOpened(std::move(onOpened))
    {
    }

    void DoorComponent::Update(float deltaTime)
    {
        const bool interactKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
        const bool pressedThisFrame = interactKeyPressed && !interactKeyWasPressed;
        interactKeyWasPressed = interactKeyPressed;

        if (!pressedThisFrame || player == nullptr)
        {
            return;
        }

        auto* hud = player->GetComponent<InventoryHudComponent>();
        if (hud != nullptr && hud->IsOpen())
        {
            return;
        }

        auto* doorTransform = gameObject->GetComponent<XYZEngine::TransformComponent>();
        auto* playerTransform = player->GetComponent<XYZEngine::TransformComponent>();
        auto* inventory = player->GetComponent<InventoryComponent>();
        if (doorTransform == nullptr || playerTransform == nullptr || inventory == nullptr)
        {
            return;
        }

        const auto doorPosition = doorTransform->GetWorldPosition();
        const auto playerPosition = playerTransform->GetWorldPosition();
        const float dx = doorPosition.x - playerPosition.x;
        const float dy = doorPosition.y - playerPosition.y;
        if (dx * dx + dy * dy > DOOR_INTERACTION_RANGE * DOOR_INTERACTION_RANGE)
        {
            return;
        }

        if (inventory->GetSelectedItem() != ItemType::Key)
        {
            if (hud != nullptr)
            {
                hud->ShowFeedback(inventory->HasItem(ItemType::Key)
                                      ? "Select the key slot to hold the key."
                                      : "You cannot open this door without a key.",
                                  sf::Color(255, 142, 120));
            }
            return;
        }

        if (!inventory->ConsumeSelectedKey())
        {
            return;
        }

        if (hud != nullptr)
        {
            hud->ShowFeedback("The key fits. The door opens...", sf::Color(147, 235, 178));
        }

        if (onOpened)
        {
            XYZEngine::GameWorld::Instance()->ScheduleAfterUpdate(onOpened);
        }
    }

    void DoorComponent::Render() {}
} // namespace XYZRoguelike
