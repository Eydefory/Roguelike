#include <SFML/Graphics.hpp>

#include "DeveloperLevel.h"
#include "Engine.h"
#include "GameFlowComponent.h"
#include "GameWorld.h"
#include "Loger.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#include <exception>
#include <filesystem>
#include <memory>
#include <system_error>
#include <vector>

using namespace XYZRoguelike;

namespace
{
    bool SetWorkingDirectoryToExecutable()
    {
        std::vector<wchar_t> executablePath(32768);
        const DWORD pathLength = GetModuleFileNameW(nullptr, executablePath.data(),
                                                    static_cast<DWORD>(executablePath.size()));

        if (pathLength == 0 || pathLength >= static_cast<DWORD>(executablePath.size()))
        {
            return false;
        }

        const std::filesystem::path executableDirectory =
            std::filesystem::path(std::wstring(executablePath.data(), pathLength)).parent_path();

        std::error_code error;
        std::filesystem::current_path(executableDirectory, error);
        return !error;
    }
} // namespace

int main()
{
    const bool workingDirectorySet = SetWorkingDirectoryToExecutable();

    auto logger = std::make_shared<Logger>();

    logger->addSink(std::make_shared<ConsoleSink>());

    auto fileSink = std::make_shared<FileSink>("game.log");

    if (!fileSink->IsOpen())
    {
        std::cerr << "WARNING: game.log could not be opened." << std::endl;
    }

    logger->addSink(fileSink);

    LoggerRegistry::getInstance().setDefaultLogger(logger);

    LoggerRegistry::getInstance().registerLogger("global", logger);

    if (!workingDirectorySet)
    {
        LOG_ERROR("Could not set working directory to the executable folder");
    }

    LOG_INFO("Logger initialized");

    try
    {
        sf::RenderWindow window(sf::VideoMode(1280, 720), "XYZ Roguelike");

        window.setFramerateLimit(60);

        XYZEngine::RenderSystem::Instance()->SetMainWindow(&window);

        auto level = std::make_shared<DeveloperLevel>();

        level->PrepareMenu();

        auto* gameFlowObject = XYZEngine::GameWorld::Instance()->CreateGameObject("GameFlowUI");
        gameFlowObject->AddComponent<GameFlowComponent>(level.get());
        level->SetPersistentUiRoot(gameFlowObject);

        LOG_INFO("Game started");

        XYZEngine::Engine::Instance()->Run();

        level->Stop();

        XYZEngine::ResourceSystem::Instance()->Clear();

        LOG_INFO("Game stopped");
    }
    catch (const std::exception& exception)
    {
        LOG_ERROR(std::string("Critical exception: ") + exception.what());
    }
    catch (...)
    {
        LOG_ERROR("Unknown critical exception");
    }

    return 0;
}
