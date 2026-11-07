#include "PlayerAnimationComponent.h"

#include "Constants.h"
#include "GameObject.h"
#include "ResourceSystem.h"
#include "SpriteRendererComponent.h"

#include <algorithm>
#include <cmath>

namespace XYZRoguelike
{
    PlayerAnimationComponent::PlayerAnimationComponent(XYZEngine::GameObject* gameObject)
        : Component(gameObject)
    {
        auto* resources = XYZEngine::ResourceSystem::Instance();
        frames[static_cast<std::size_t>(Facing::Front)] = {
            resources->GetTextureShared("player_walk_1"),
            resources->GetTextureShared("player_walk_2"),
            resources->GetTextureShared("player_walk_3"),
            resources->GetTextureShared("player_idle")};
        frames[static_cast<std::size_t>(Facing::Back)] = {
            resources->GetTextureShared("player_back_walk_01"),
            resources->GetTextureShared("player_back_walk_02"),
            resources->GetTextureShared("player_back_walk_03"),
            resources->GetTextureShared("player_back_idle")};
        frames[static_cast<std::size_t>(Facing::Left)] = {
            resources->GetTextureShared("player_left_walk_01"),
            resources->GetTextureShared("player_left_walk_02"),
            resources->GetTextureShared("player_left_walk_03"),
            resources->GetTextureShared("player_left_idle")};
        frames[static_cast<std::size_t>(Facing::Right)] = {
            resources->GetTextureShared("player_right_walk_01"),
            resources->GetTextureShared("player_right_walk_02"),
            resources->GetTextureShared("player_right_walk_03"),
            resources->GetTextureShared("player_right_idle")};
        renderer = gameObject->GetComponent<XYZEngine::SpriteRendererComponent>();
        ApplyFrame(3);
    }

    void PlayerAnimationComponent::Update(float deltaTime)
    {
        if (!std::isfinite(deltaTime) || deltaTime <= 0.f)
        {
            return;
        }

        if (!isMoving)
        {
            animationTimer = 0.f;
            ApplyFrame(3);
            return;
        }

        animationTimer += deltaTime;
        constexpr float FRAME_DURATION = 0.12f;
        const auto frameIndex = static_cast<std::size_t>(animationTimer / FRAME_DURATION) % 3;
        ApplyFrame(frameIndex);
    }

    void PlayerAnimationComponent::Render() {}

    void PlayerAnimationComponent::SetMovementState(float horizontal, float vertical, bool moved)
    {
        FaceToward(horizontal, vertical);
        isMoving = moved;
    }

    void PlayerAnimationComponent::FaceToward(float horizontal, float vertical)
    {
        const float length = std::sqrt(horizontal * horizontal + vertical * vertical);
        if (length > 0.f)
        {
            const Facing previousFacing = facing;
            if (std::abs(horizontal) > std::abs(vertical))
            {
                facing = horizontal < 0.f ? Facing::Left : Facing::Right;
                facingDirection = {horizontal < 0.f ? -1.f : 1.f, 0.f};
            }
            else
            {
                facing = vertical < 0.f ? Facing::Back : Facing::Front;
                facingDirection = {0.f, vertical < 0.f ? -1.f : 1.f};
            }

            if (facing != previousFacing)
            {
                ApplyFrame(currentFrame < 3 ? currentFrame : 3);
            }
        }
    }

    sf::Vector2f PlayerAnimationComponent::GetFacingDirection() const
    {
        return facingDirection;
    }

    void PlayerAnimationComponent::ApplyFrame(std::size_t frameIndex)
    {
        const auto& directionFrames = frames[static_cast<std::size_t>(facing)];
        if (renderer == nullptr || frameIndex >= directionFrames.size() ||
            directionFrames[frameIndex] == nullptr ||
            (currentFrame == frameIndex && currentFacing == facing))
        {
            return;
        }

        renderer->SetTexture(*directionFrames[frameIndex]);
        renderer->SetPixelSize(static_cast<int>(PLAYER_WIDTH), static_cast<int>(PLAYER_HEIGHT));
        currentFrame = frameIndex;
        currentFacing = facing;
    }
} // namespace XYZRoguelike
