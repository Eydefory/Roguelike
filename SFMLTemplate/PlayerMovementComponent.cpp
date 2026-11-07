#include "PlayerMovementComponent.h"
#include "Constants.h"
#include "InventoryHudComponent.h"
#include "PhysicsSystem.h"
#include "PlayerAnimationComponent.h"
#include "SpriteColliderComponent.h"
#include "StaminaComponent.h"

#include <SFML/Window/Keyboard.hpp>
#include <cmath>

namespace XYZRoguelike
{
    PlayerMovementComponent::PlayerMovementComponent(XYZEngine::GameObject* gameObject)
        : Component(gameObject), movementSpeed(PLAYER_SPEED)
    {
    }

    void PlayerMovementComponent::Update(float deltaTime)
    {
        auto inventoryHud = GetGameObject()->GetComponent<InventoryHudComponent>();
        if (inventoryHud != nullptr && inventoryHud->IsOpen())
        {
            auto* animation = GetGameObject()->GetComponent<PlayerAnimationComponent>();
            if (animation != nullptr)
            {
                animation->SetMovementState(0.f, 0.f, false);
            }
            return;
        }

        auto input = GetGameObject()->GetComponent<XYZEngine::InputComponent>();

        auto transform = GetGameObject()->GetComponent<XYZEngine::TransformComponent>();

        if (input == nullptr || transform == nullptr)
            return;

        float horizontal = input->GetHorizontalAxis();
        float vertical = input->GetVerticalAxis();

        float length = std::sqrt(horizontal * horizontal + vertical * vertical);

        if (length > 0.f)
        {
            horizontal /= length;
            vertical /= length;
        }

        float speed = movementSpeed;
        const bool sprintKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                                      sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
        if (length > 0.f && sprintKeyPressed)
        {
            auto stamina = GetGameObject()->GetComponent<StaminaComponent>();
            if (stamina != nullptr && stamina->Consume(PLAYER_STAMINA_DRAIN_PER_SECOND * deltaTime))
            {
                speed *= PLAYER_SPRINT_SPEED_MULTIPLIER;
            }
        }

        float moveX = horizontal * speed * deltaTime;
        float moveY = vertical * speed * deltaTime;

        const auto previousPosition = transform->GetWorldPosition();
        auto collider = GetGameObject()->GetComponent<XYZEngine::SpriteColliderComponent>();
        if (collider != nullptr)
        {
            XYZEngine::PhysicsSystem::Instance()->MoveCollider(collider, moveX, moveY);
        }
        else
        {
            transform->MoveBy(moveX, moveY);
        }

        const auto currentPosition = transform->GetWorldPosition();
        const float movedX = currentPosition.x - previousPosition.x;
        const float movedY = currentPosition.y - previousPosition.y;
        auto* animation = GetGameObject()->GetComponent<PlayerAnimationComponent>();
        if (animation != nullptr)
        {
            animation->SetMovementState(horizontal, vertical,
                                        movedX * movedX + movedY * movedY > 0.01f);
        }
    }

    void PlayerMovementComponent::Render() {}
} // namespace XYZRoguelike
