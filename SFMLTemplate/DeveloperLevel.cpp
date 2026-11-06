#include "DeveloperLevel.h"

#include "AudioManager.h"
#include "Constants.h"
#include "CreeperSpawner.h"
#include "GameWorld.h"
#include "Loger.h"
#include "MazeGenerator.h"
#include "ResourceSystem.h"
#include "TransformComponent.h"

#include <algorithm>

namespace XYZRoguelike
{
    void DeveloperLevel::Start()
    {
        LOG_INFO("DeveloperLevel initialization started");

        auto resources = XYZEngine::ResourceSystem::Instance();

        AudioManager::Instance().PlayMusic("Resources/Music/music.ogg");

        resources->LoadTexture("player", "Resources/Textures/player.png");

        resources->LoadTexture("enemy", "Resources/Textures/enemy.png");

        resources->LoadTexture("floor", "Resources/Textures/floor.png");

        resources->LoadTexture("wall_top", "Resources/Textures/wall_top.png");

        resources->LoadTexture("wall_top_alt", "Resources/Textures/wall_top_alt.png");

        resources->LoadTexture("wall_bottom", "Resources/Textures/wall_bottom.png");

        resources->LoadTexture("wall_bottom_alt", "Resources/Textures/wall_bottom_alt.png");

        resources->LoadTexture("wall_left", "Resources/Textures/wall_left.png");

        resources->LoadTexture("wall_left_alt", "Resources/Textures/wall_left_alt.png");

        resources->LoadTexture("wall_right", "Resources/Textures/wall_right.png");

        resources->LoadTexture("wall_right_alt", "Resources/Textures/wall_right_alt.png");

        resources->LoadTexture("wall_top_left", "Resources/Textures/wall_top_left.png");

        resources->LoadTexture("wall_top_right", "Resources/Textures/wall_top_right.png");

        resources->LoadTexture("wall_bottom_left", "Resources/Textures/wall_bottom_left.png");

        resources->LoadTexture("wall_bottom_right", "Resources/Textures/wall_bottom_right.png");

        floorContainer = std::make_unique<FloorContainer>();

        wallContainer = std::make_unique<WallContainer>();

        player = std::make_shared<Player>();

        enemyContainer = std::make_unique<EnemyContainer>();

        player->SetEnemyContainer(enemyContainer.get());

        MazeGenerator maze(ROOM_COLUMNS, ROOM_ROWS, this);

        maze.Generate();

        auto walkablePositions = maze.GetWalkablePositions();
        auto* playerTransform =
            player->GetGameObject()->GetComponent<XYZEngine::TransformComponent>();

        if (playerTransform != nullptr && !walkablePositions.empty())
        {
            const auto playerPosition = playerTransform->GetWorldPosition();
            const auto nearestWalkablePosition = std::min_element(
                walkablePositions.begin(), walkablePositions.end(),
                [playerPosition](const auto& left, const auto& right)
                {
                    const float leftX = left.first - playerPosition.x;
                    const float leftY = left.second - playerPosition.y;
                    const float rightX = right.first - playerPosition.x;
                    const float rightY = right.second - playerPosition.y;
                    return leftX * leftX + leftY * leftY < rightX * rightX + rightY * rightY;
                });

            playerTransform->SetWorldPosition(nearestWalkablePosition->first,
                                              nearestWalkablePosition->second);
        }

        // Spawn only after generation so the spawner can use the finished maze.
        CreeperSpawner creeperSpawner;
        creeperSpawner.SpawnCreepers(*enemyContainer, player->GetGameObject(), walkablePositions);

        LOG_INFO("DeveloperLevel initialization completed");
    }

    void DeveloperLevel::Restart()
    {
        LOG_INFO("DeveloperLevel restarting");

        Stop();
        Start();
    }

    void DeveloperLevel::Stop()
    {
        LOG_INFO("DeveloperLevel stopping");

        AudioManager::Instance().StopMusic();

        XYZEngine::GameWorld::Instance()->Clear();

        enemyContainer.reset();
        wallContainer.reset();
        floorContainer.reset();
        player.reset();

        LOG_INFO("DeveloperLevel stopped");
    }
} // namespace XYZRoguelike
