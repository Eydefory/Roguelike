#include "PlayerMovementComponent.h"
#include "Constants.h"

#include <cmath>

namespace XYZRoguelike
{
    PlayerMovementComponent::PlayerMovementComponent(
        XYZEngine::GameObject* gameObject)
        : Component(gameObject),
        movementSpeed(PLAYER_SPEED)
    {
    }

    void PlayerMovementComponent::Update(float deltaTime)
    {
        auto input =
            GetGameObject()->GetComponent<XYZEngine::InputComponent>();

        auto transform =
            GetGameObject()->GetComponent<XYZEngine::TransformComponent>();

        if (input == nullptr || transform == nullptr)
            return;

        float horizontal = input->GetHorizontalAxis();
        float vertical = input->GetVerticalAxis();

        
        float length = std::sqrt(
            horizontal * horizontal +
            vertical * vertical
        );

        if (length > 0.f)
        {
            horizontal /= length;
            vertical /= length;
        }

        float moveX = horizontal * movementSpeed * deltaTime;
        float moveY = vertical * movementSpeed * deltaTime;

        transform->MoveBy(moveX, moveY);
    }

    void PlayerMovementComponent::Render()
    {
    }
}