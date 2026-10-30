#include "pch.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

namespace XYZEngine
{
    CameraComponent::CameraComponent(GameObject* gameObject)
        : Component(gameObject)
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

        view->setCenter(Convert<sf::Vector2f, Vector2Df>(position));
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

        view->reset(sf::FloatRect(
            0.f,
            0.f,
            static_cast<float>(width),
            static_cast<float>(height)
        ));
    }

    void CameraComponent::SetWindow(sf::RenderWindow* newWindow)
    {
        window = newWindow;
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
}