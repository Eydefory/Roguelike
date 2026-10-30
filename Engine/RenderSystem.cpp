#include "pch.h"
#include "RenderSystem.h"

#include <stdexcept>

namespace XYZEngine
{
    RenderSystem* RenderSystem::Instance()
    {
        static RenderSystem render;
        return &render;
    }

    void RenderSystem::SetMainWindow(sf::RenderWindow* newWindow)
    {
        window = newWindow;
    }

    sf::RenderWindow& RenderSystem::GetMainWindow() const
    {
        if (window == nullptr)
        {
            throw std::runtime_error("RenderSystem: main window is not set.");
        }

        return *window;
    }

    void RenderSystem::Render(const sf::Drawable& drawable)
    {
        GetMainWindow().draw(drawable);
    }
}