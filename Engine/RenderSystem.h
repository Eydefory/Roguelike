#pragma once
#include "EngineAPI.h"
#include <SFML/Graphics.hpp>

namespace XYZEngine
{
    class XYZENGINE_API RenderSystem
    {
    public:
        static RenderSystem* Instance();

        void SetMainWindow(sf::RenderWindow* newWindow);
        sf::RenderWindow& GetMainWindow() const;

        void Render(const sf::Drawable& drawable);

    private:
        sf::RenderWindow* window = nullptr;

        RenderSystem() = default;
        ~RenderSystem() = default;

        RenderSystem(const RenderSystem&) = delete;
        RenderSystem& operator=(const RenderSystem&) = delete;
    };
}