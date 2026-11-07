#include "pch.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"

#include <algorithm>
#include <cmath>

namespace XYZEngine
{
    CameraComponent::CameraComponent(GameObject* gameObject) : Component(gameObject)
    {
        view = new sf::View(sf::FloatRect(0.f, 0.f, 800.f, 600.f));
        transform = gameObject->GetComponent<TransformComponent>();
    }

    CameraComponent::~CameraComponent()
    {
        delete view;
    }

    void CameraComponent::Update(float deltaTime)
    {
        if (window == nullptr || transform == nullptr || view == nullptr)
        {
            return;
        }

        auto position = transform->GetWorldPosition();
        auto rotation = transform->GetWorldRotation();

        sf::Vector2f shakeOffset(0.f, 0.f);
        if (shakeTimeRemaining > 0.f && std::isfinite(deltaTime) && deltaTime > 0.f)
        {
            shakeTimeRemaining = std::max(0.f, shakeTimeRemaining - deltaTime);
            shakePhase += deltaTime;

            if (shakeTimeRemaining > 0.f && shakeDuration > 0.f)
            {
                const float fade = shakeTimeRemaining / shakeDuration;
                shakeOffset.x = std::sin(shakePhase * 95.f) * shakeMagnitude * fade;
                shakeOffset.y = std::cos(shakePhase * 113.f) * shakeMagnitude * 0.7f * fade;
            }
            else
            {
                shakeMagnitude = 0.f;
                shakeDuration = 0.f;
                shakePhase = 0.f;
            }
        }

        sf::Vector2f cameraCenter = Convert<sf::Vector2f, Vector2Df>(position) + shakeOffset;

        if (hasWorldBounds)
        {
            const float radians = rotation * 3.14159265358979323846f / 180.f;
            const sf::Vector2f viewSize = view->getSize();
            const float halfViewWidth = (std::abs(std::cos(radians)) * viewSize.x +
                                         std::abs(std::sin(radians)) * viewSize.y) *
                                        0.5f;
            const float halfViewHeight = (std::abs(std::sin(radians)) * viewSize.x +
                                          std::abs(std::cos(radians)) * viewSize.y) *
                                         0.5f;

            const auto clampCenter = [](float center, float start, float extent, float halfView)
            {
                if (extent <= halfView * 2.f)
                {
                    return start + extent * 0.5f;
                }

                return std::clamp(center, start + halfView, start + extent - halfView);
            };

            cameraCenter.x =
                clampCenter(cameraCenter.x, worldBounds.left, worldBounds.width, halfViewWidth);
            cameraCenter.y =
                clampCenter(cameraCenter.y, worldBounds.top, worldBounds.height, halfViewHeight);
        }

        view->setCenter(cameraCenter);
        view->setRotation(rotation);

        window->setView(*view);
    }

    void CameraComponent::Render()
    {
        if (window == nullptr)
        {
            std::cout << "NULL window render." << std::endl;
        }
    }

    void CameraComponent::SetBaseResolution(int width, int height)
    {
        if (view == nullptr)
        {
            return;
        }

        view->reset(sf::FloatRect(0.f, 0.f, static_cast<float>(width), static_cast<float>(height)));
    }

    void CameraComponent::SetWindow(sf::RenderWindow* newWindow)
    {
        window = newWindow;
    }

    void CameraComponent::SetWorldBounds(const sf::FloatRect& bounds)
    {
        if (!std::isfinite(bounds.left) || !std::isfinite(bounds.top) ||
            !std::isfinite(bounds.width) || !std::isfinite(bounds.height) || bounds.width <= 0.f ||
            bounds.height <= 0.f)
        {
            hasWorldBounds = false;
            return;
        }

        worldBounds = bounds;
        hasWorldBounds = true;
    }

    void CameraComponent::ZoomBy(float newZoom)
    {
        if (view == nullptr)
        {
            return;
        }

        if (newZoom <= 0.f)
        {
            std::cout << "Not allowed zoom lesser or equal than zero." << std::endl;
            return;
        }

        view->zoom(newZoom);
    }

    void CameraComponent::Shake(float duration, float magnitude)
    {
        if (!std::isfinite(duration) || !std::isfinite(magnitude) || duration <= 0.f ||
            magnitude <= 0.f)
        {
            return;
        }

        if (shakeTimeRemaining <= 0.f)
        {
            shakePhase = 0.f;
        }

        shakeTimeRemaining = std::max(shakeTimeRemaining, duration);
        shakeDuration = std::max(shakeDuration, shakeTimeRemaining);
        shakeMagnitude = std::max(shakeMagnitude, magnitude);
    }
} // namespace XYZEngine
