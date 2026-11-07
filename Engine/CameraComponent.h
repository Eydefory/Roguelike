#pragma once
#include "EngineAPI.h"
#include "TransformComponent.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace XYZEngine
{
    class XYZENGINE_API CameraComponent : public Component
    {
    public:
        CameraComponent(GameObject* gameObject);
        ~CameraComponent();

        void Update(float deltaTime) override;
        void Render() override;

        void SetWindow(sf::RenderWindow* newWindow);
        void SetWorldBounds(const sf::FloatRect& bounds);
        void ZoomBy(float newZoom);
        void Shake(float duration = 0.12f, float magnitude = 5.f);

        void SetBaseResolution(int width, int height);

    private:
        TransformComponent* transform = nullptr;
        sf::RenderWindow* window = nullptr;
        sf::View* view = nullptr;
        sf::FloatRect worldBounds{};
        bool hasWorldBounds = false;
        float shakeTimeRemaining = 0.f;
        float shakeDuration = 0.f;
        float shakeMagnitude = 0.f;
        float shakePhase = 0.f;
    };

} // namespace XYZEngine
