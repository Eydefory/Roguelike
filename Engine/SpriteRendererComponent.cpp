#include "pch.h"
#include "SpriteRendererComponent.h"
#include "TransformComponent.h"
#include "RenderSystem.h"

namespace XYZEngine
{
    SpriteRendererComponent::SpriteRendererComponent(GameObject* gameObject)
        : Component(gameObject)
    {
        sprite = new sf::Sprite();

        scale = { 1.f, 1.f };

        sprite->setScale({ 1.f, 1.f });

        transform = gameObject->GetComponent<TransformComponent>();
    }

    SpriteRendererComponent::~SpriteRendererComponent()
    {
        delete sprite;
    }

    void SpriteRendererComponent::Update(float deltaTime)
    {
        SyncTransform();
    }

    void SpriteRendererComponent::SyncTransform()
    {
        if (sprite == nullptr || transform == nullptr)
        {
            return;
        }

        sprite->setPosition(
            Convert<sf::Vector2f, Vector2Df>(
                transform->GetWorldPosition()
            )
        );

        sprite->setRotation(transform->GetWorldRotation());

        auto transformScale =
            Convert<sf::Vector2f, Vector2Df>(
                transform->GetWorldScale()
            );

        sprite->setScale({
            scale.x * transformScale.x,
            scale.y * transformScale.y
            });
    }

    void SpriteRendererComponent::Render()
    {
        if (sprite == nullptr || transform == nullptr)
        {
            return;
        }

        SyncTransform();

        RenderSystem::Instance()->Render(*sprite);
    }

    const sf::Sprite* SpriteRendererComponent::GetSprite() const
    {
        return sprite;
    }

    void SpriteRendererComponent::SetTexture(const sf::Texture& newTexture)
    {
        sprite->setTexture(newTexture);

        auto textureSize = sprite->getTexture()->getSize();

        sprite->setOrigin({
            0.5f * textureSize.x,
            0.5f * textureSize.y
            });
    }

    void SpriteRendererComponent::SetPixelSize(int newWidth, int newHeight)
    {
        auto originalSize = sprite->getTexture()->getSize();

        if (originalSize.x == 0 || originalSize.y == 0)
        {
            return;
        }

        scale = {
            static_cast<float>(newWidth) / static_cast<float>(originalSize.x),
            static_cast<float>(newHeight) / static_cast<float>(originalSize.y)
        };
    }

    void SpriteRendererComponent::FlipX(bool flip)
    {
        if (flip != isFlipX)
        {
            scale.x = -scale.x;
            isFlipX = flip;
        }
    }

    void SpriteRendererComponent::FlipY(bool flip)
    {
        if (flip != isFlipY)
        {
            scale.y = -scale.y;
            isFlipY = flip;
        }
    }
}