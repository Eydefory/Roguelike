#include <SFML/Graphics.hpp>

#include "Engine.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"
#include "DeveloperLevel.h"
#include "GameWorld.h"
#include "Loger.h"

#include <memory>
#include <exception>

using namespace XYZRoguelike;

int main()
{ auto logger = std::make_shared<Logger>();

    logger->addSink(std::make_shared<ConsoleSink>());

    auto fileSink = std::make_shared<FileSink>("game.log");

    if (!fileSink->IsOpen())
    {
        std::cerr << "WARNING: game.log could not be opened."
            << std::endl;
    }

    logger->addSink(fileSink);

    LoggerRegistry::getInstance()
        .setDefaultLogger(logger);

    LoggerRegistry::getInstance()
        .registerLogger("global", logger);

    LOG_INFO("Logger initialized");

    try
    {
        auto* window = new sf::RenderWindow(
            sf::VideoMode(1280, 720),
            "XYZ Roguelike");

        window->setFramerateLimit(60);

        XYZEngine::RenderSystem::Instance()
            ->SetMainWindow(window);

        auto level = std::make_shared<DeveloperLevel>();

        level->Start();

        LOG_INFO("Game started");

        XYZEngine::Engine::Instance()->Run();

        level->Stop();

        XYZEngine::ResourceSystem::Instance()->Clear();

        LOG_INFO("Game stopped");
    }
    catch (const std::exception& exception)
    {
        LOG_ERROR(std::string("Critical exception: ") +
            exception.what());
    }
    catch (...)
    {
        LOG_ERROR("Unknown critical exception");
    }

    return 0;
}