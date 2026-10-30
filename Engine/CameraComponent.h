#pragma once
#include "EngineAPI.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "TransformComponent.h"

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
        void ZoomBy(float newZoom);

        void SetBaseResolution(int width, int height);

    private:
        TransformComponent* transform = nullptr;
        sf::RenderWindow* window = nullptr;
        sf::View* view = nullptr;
    };
    

}
