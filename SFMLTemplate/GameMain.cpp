#include <SFML/Graphics.hpp>

#include "Engine.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"
#include "DeveloperLevel.h"
#include "GameWorld.h"

using namespace XYZRoguelike;

int main()
{
    auto window = new sf::RenderWindow(
        sf::VideoMode(1280, 720),
        "XYZ Roguelike"
    );

    window->setFramerateLimit(60);

    XYZEngine::RenderSystem::Instance()->SetMainWindow(window);

    auto level = std::make_shared<DeveloperLevel>();

    level->Start();

    XYZEngine::Engine::Instance()->Run();

    level->Stop();

    XYZEngine::ResourceSystem::Instance()->Clear();

    return 0;
}