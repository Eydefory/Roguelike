#include "pch.h"
#include "SpriteColliderComponent.h"

namespace XYZEngine
{
    SpriteColliderComponent::SpriteColliderComponent(GameObject* gameObject)
        : ColliderComponent(gameObject)
    {
        auto spriteRenderer =
            gameObject->GetComponent<SpriteRendererComponent>();

        if (spriteRenderer == nullptr)
        {
            std::cout << "SpriteRenderer required to SpriteCollider."
                << std::endl;

            gameObject->RemoveComponent(this);
            return;
        }

        sprite = spriteRenderer->GetSprite();

        PhysicsSystem::Instance()->Subscribe(this);
    }

    SpriteColliderComponent::~SpriteColliderComponent()
    {
        PhysicsSystem::Instance()->Unsubscribe(this);
    }

    void SpriteColliderComponent::Update(float deltaTime)
    {
        auto spriteRenderer =
            gameObject->GetComponent<SpriteRendererComponent>();

        if (spriteRenderer == nullptr || sprite == nullptr)
        {
            return;
        }

        spriteRenderer->SyncTransform();

        bounds = sprite->getGlobalBounds();
    }

    void SpriteColliderComponent::Render()
    {
        sf::RectangleShape rectangle(
            sf::Vector2f(bounds.width, bounds.height)
        );

        rectangle.setPosition(bounds.left, bounds.top);
        rectangle.setFillColor(sf::Color::Transparent);
        rectangle.setOutlineColor(sf::Color::White);
        rectangle.setOutlineThickness(4.f);

        RenderSystem::Instance()->Render(rectangle);
    }
}