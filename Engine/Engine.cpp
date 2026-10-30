#include "pch.h"
#include "Engine.h"
#include "GameWorld.h"
#include "RenderSystem.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

namespace XYZEngine
{
    Engine* Engine::Instance()
    {
        static Engine instance;
        return &instance;
    }

    Engine::Engine()
    {
        unsigned int seed = static_cast<unsigned int>(time(nullptr));
        srand(seed);
    }

    void Engine::Run()
    {
        sf::Clock gameClock;
        sf::Event event;

        while (RenderSystem::Instance()->GetMainWindow().isOpen())
        {
            sf::Time dt = gameClock.restart();
            float deltaTime = dt.asSeconds();

            while (RenderSystem::Instance()->GetMainWindow().pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    RenderSystem::Instance()->GetMainWindow().close();
                }
            }

            if (!RenderSystem::Instance()->GetMainWindow().isOpen())
            {
                break;
            }

            RenderSystem::Instance()->GetMainWindow().clear();

            GameWorld::Instance()->Update(deltaTime);
            GameWorld::Instance()->FixedUpdate(deltaTime);
            GameWorld::Instance()->Render();
            GameWorld::Instance()->LateUpdate();

            RenderSystem::Instance()->GetMainWindow().display();
        }
    }
}