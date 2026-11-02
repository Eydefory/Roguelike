#include "pch.h"
#include "Engine.h"
#include "GameWorld.h"
#include "RenderSystem.h"
#include "Loger.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>

namespace XYZEngine
{
    Engine* Engine::Instance()
    {
        static Engine instance;
        return &instance;
    }

    Engine::Engine()
    {
        unsigned int seed =
            static_cast<unsigned int>(time(nullptr));

        srand(seed);

        LOG_INFO("Engine initialized");
    }

    void Engine::Run()
    {
        LOG_INFO("Engine loop started");

        sf::Clock gameClock;
        sf::Event event;

        while (RenderSystem::Instance()
            ->GetMainWindow().isOpen())
        {
            sf::Time dt = gameClock.restart();
            float deltaTime = dt.asSeconds();

            assert(deltaTime >= 0.f);

            while (RenderSystem::Instance()
                ->GetMainWindow().pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    LOG_INFO("Window close event");
                    RenderSystem::Instance()
                        ->GetMainWindow().close();
                }
            }

            if (!RenderSystem::Instance()
                ->GetMainWindow().isOpen())
            {
                break;
            }

            try
            {
                RenderSystem::Instance()
                    ->GetMainWindow().clear();

                GameWorld::Instance()->Update(deltaTime);
                GameWorld::Instance()->FixedUpdate(deltaTime);
                GameWorld::Instance()->Render();
                GameWorld::Instance()->LateUpdate();

                RenderSystem::Instance()
                    ->GetMainWindow().display();
            }
            catch (const std::exception& exception)
            {
                LOG_ERROR(std::string("Engine frame exception: ") +
                    exception.what());
            }
            catch (...)
            {
                LOG_ERROR("Unknown Engine frame exception");
            }
        }

        LOG_INFO("Engine loop stopped");
    }
}