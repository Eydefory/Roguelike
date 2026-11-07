#pragma once

#include "Component.h"

#include <SFML/System/Vector2.hpp>

#include <array>
#include <cstddef>

namespace sf
{
    class Texture;
}

namespace XYZEngine
{
    class SpriteRendererComponent;
}

namespace XYZRoguelike
{
    class PlayerAnimationComponent : public XYZEngine::Component
    {
    public:
        explicit PlayerAnimationComponent(XYZEngine::GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override;

        void SetMovementState(float horizontal, float vertical, bool moved);
        void FaceToward(float horizontal, float vertical);
        sf::Vector2f GetFacingDirection() const;

    private:
        enum class Facing
        {
            Front,
            Back,
            Left,
            Right
        };
        void ApplyFrame(std::size_t frameIndex);

        std::array<std::array<const sf::Texture*, 4>, 4> frames{};
        XYZEngine::SpriteRendererComponent* renderer = nullptr;
        sf::Vector2f facingDirection{0.f, 1.f};
        float animationTimer = 0.f;
        std::size_t currentFrame = static_cast<std::size_t>(-1);
        Facing currentFacing = Facing::Front;
        bool isMoving = false;
        Facing facing = Facing::Front;
    };
} // namespace XYZRoguelike
