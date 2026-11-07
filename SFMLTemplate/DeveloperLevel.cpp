#include "DeveloperLevel.h"

#include "AudioManager.h"
#include "Constants.h"
#include "CreeperSpawner.h"
#include "DoorComponent.h"
#include "GameWorld.h"
#include "HealthBarComponent.h"
#include "InventoryHudComponent.h"
#include "ItemPickupComponent.h"
#include "Loger.h"
#include "MazeGenerator.h"
#include "PotionSpawnerComponent.h"
#include "QuestHudComponent.h"
#include "ResourceSystem.h"
#include "RigidbodyComponent.h"
#include "SpriteColliderComponent.h"
#include "SpriteRendererComponent.h"
#include "StaminaComponent.h"
#include "TransformComponent.h"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <array>
#include <random>
#include <vector>

namespace XYZRoguelike
{
    void DeveloperLevel::PrepareMenu()
    {
        LOG_INFO("Preparing the main menu");
        LoadMenuResources();
        runComplete = false;
    }

    void DeveloperLevel::Start()
    {
        LOG_INFO("DeveloperLevel initialization started");
        LoadResources();
        AudioManager::Instance().PlayMusic("Resources/Music/music.ogg");
        levelManager.StartRun();
        runComplete = false;
        BuildLevel(false);
        LOG_INFO("DeveloperLevel initialization completed");
    }

    void DeveloperLevel::Restart()
    {
        LOG_INFO("DeveloperLevel restarting");
        Stop();
        XYZEngine::ResourceSystem::Instance()->Clear();
        resourcesLoaded = false;
        menuResourcesLoaded = false;
        Start();
    }

    void DeveloperLevel::RestartKeeping(XYZEngine::GameObject* preservedRoot)
    {
        if (preservedRoot == nullptr)
        {
            Restart();
            return;
        }

        LOG_INFO("DeveloperLevel restarting while keeping the game flow UI");
        LoadResources();
        persistentUiRoot = preservedRoot;
        AudioManager::Instance().StopMusic();
        levelManager.StartRun();
        runComplete = false;
        AudioManager::Instance().PlayMusic("Resources/Music/music.ogg");
        BuildLevel(false, preservedRoot);
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
        doorGameObject = nullptr;
        persistentUiRoot = nullptr;
        runComplete = false;
        LOG_INFO("DeveloperLevel stopped");
    }

    void DeveloperLevel::ReturnToMenuKeeping(XYZEngine::GameObject* preservedRoot)
    {
        if (preservedRoot == nullptr)
        {
            return;
        }

        AudioManager::Instance().StopMusic();
        XYZEngine::GameWorld::Instance()->ClearExcept(preservedRoot);
        enemyContainer.reset();
        wallContainer.reset();
        floorContainer.reset();
        player.reset();
        doorGameObject = nullptr;
        persistentUiRoot = preservedRoot;
        runComplete = false;
    }

    void DeveloperLevel::LoadResources()
    {
        if (resourcesLoaded)
        {
            return;
        }

        LoadMenuResources();
        auto* resources = XYZEngine::ResourceSystem::Instance();
        const std::array<std::pair<const char*, const char*>, 49> textures = {{
            {"player", "Resources/Textures/player_walk_frame_04.png"},
            {"player_walk_1", "Resources/Textures/player_walk_frame_01.png"},
            {"player_walk_2", "Resources/Textures/player_walk_frame_02.png"},
            {"player_walk_3", "Resources/Textures/player_walk_frame_03.png"},
            {"player_idle", "Resources/Textures/player_walk_frame_04.png"},
            {"player_back_walk_01", "Resources/Textures/player_back_walk_01.png"},
            {"player_back_walk_02", "Resources/Textures/player_back_walk_02.png"},
            {"player_back_walk_03", "Resources/Textures/player_back_walk_03.png"},
            {"player_back_idle", "Resources/Textures/player_back_idle_04.png"},
            {"player_left_walk_01", "Resources/Textures/player_left_walk_01.png"},
            {"player_left_walk_02", "Resources/Textures/player_left_walk_02.png"},
            {"player_left_walk_03", "Resources/Textures/player_left_walk_03.png"},
            {"player_left_idle", "Resources/Textures/player_left_idle_04.png"},
            {"player_right_walk_01", "Resources/Textures/player_right_walk_01.png"},
            {"player_right_walk_02", "Resources/Textures/player_right_walk_02.png"},
            {"player_right_walk_03", "Resources/Textures/player_right_walk_03.png"},
            {"player_right_idle", "Resources/Textures/player_right_idle_04.png"},
            {"health_bar_frame", "Resources/Textures/health_bar_frame.png"},
            {"health_fill_red", "Resources/Textures/health_fill_red.png"},
            {"shield_icon", "Resources/Textures/shield.png"},
            {"title_background", "Resources/Textures/title_background.png"},
            {"victory_background", "Resources/Textures/victory_background.png"},
            {"defeat_background", "Resources/Textures/defeat_background.png"},
            {"button_container_hover", "Resources/Textures/button_container_hover.png"},
            {"button_container", "Resources/Textures/button_container.png"},
            {"end_screen_container", "Resources/Textures/end_screen_container.png"},
            {"floor_variant_1", "Resources/Textures/floor_stone_dark_02.png"},
            {"floor_variant_2", "Resources/Textures/floor_stone_dark_05.png"},
            {"wall_variant_1", "Resources/Textures/wall_stone_gray_01.png"},
            {"wall_variant_2", "Resources/Textures/wall_stone_gray_02.png"},
            {"wall_variant_3", "Resources/Textures/wall_stone_gray_03.png"},
            {"wall_variant_4", "Resources/Textures/wall_stone_gray_04.png"},
            {"wall_variant_5", "Resources/Textures/wall_stone_gray_05.png"},
            {"enemy", "Resources/Textures/enemy.png"},
            {"projectile", "Resources/Textures/arrow.png"},
            {"item_sword", "Resources/Textures/sword.png"},
            {"item_bow", "Resources/Textures/bow.png"},
            {"item_health_potion", "Resources/Textures/health_potion.png"},
            {"item_shield_potion", "Resources/Textures/shield_potion.png"},
            {"item_stamina_potion", "Resources/Textures/stamina_potion.png"},
            {"item_key", "Resources/Textures/key.png"},
            {"door", "Resources/Textures/double_wooden_door.png"},
            {"slot_empty", "Resources/Textures/slot_empty.png"},
            {"slot_selected", "Resources/Textures/slot_selected.png"},
            {"meter_track", "Resources/Textures/meter_track_transparent.png"},
            {"status_panel", "Resources/Textures/status_panel.png"},
            {"task_table", "Resources/Textures/task_table.png"},
            {"popup_message", "Resources/Textures/popup_message.png"},
            {"game_logo", "Resources/xyz-logo.png"},
        }};

        for (const auto& texture : textures)
        {
            resources->LoadTexture(texture.first, texture.second, false);
        }

        resourcesLoaded = true;
    }

    void DeveloperLevel::LoadMenuResources()
    {
        if (menuResourcesLoaded)
        {
            return;
        }

        auto* resources = XYZEngine::ResourceSystem::Instance();
        resources->LoadTexture("inventory_panel", "Resources/Textures/inventory_panel.png", false);
        resources->LoadTexture("title_background", "Resources/Textures/title_background.png", false);
        resources->LoadTexture("victory_background", "Resources/Textures/victory_background.png", false);
        resources->LoadTexture("defeat_background", "Resources/Textures/defeat_background.png", false);
        resources->LoadTexture("button_container_hover", "Resources/Textures/button_container_hover.png", false);
        resources->LoadTexture("button_container", "Resources/Textures/button_container.png", false);
        resources->LoadTexture("end_screen_container", "Resources/Textures/end_screen_container.png", false);
        resources->LoadTexture("pause_menu", "Resources/Textures/pause_menu.png", false);
        menuResourcesLoaded =
            resources->GetTextureShared("inventory_panel") != nullptr;
    }

    void DeveloperLevel::BuildLevel(bool preservePlayer, XYZEngine::GameObject* preservedRoot)
    {
        auto* world = XYZEngine::GameWorld::Instance();
        if (preservePlayer && player != nullptr)
        {
            world->ClearExcept(player->GetGameObject(), persistentUiRoot);
        }
        else
        {
            if (preservedRoot != nullptr)
            {
                world->ClearExcept(preservedRoot);
            }
            else
            {
                world->Clear();
            }
            player = std::make_shared<Player>();
        }

        enemyContainer.reset();
        wallContainer.reset();
        floorContainer.reset();
        doorGameObject = nullptr;

        const auto doorCell = ChooseDoorCell();
        floorContainer = std::make_unique<FloorContainer>(doorCell.first, doorCell.second);
        wallContainer = std::make_unique<WallContainer>(doorCell.first, doorCell.second);
        enemyContainer = std::make_unique<EnemyContainer>();
        player->SetEnemyContainer(enemyContainer.get());

        auto* playerTransform =
            player->GetGameObject()->GetComponent<XYZEngine::TransformComponent>();
        if (playerTransform != nullptr)
        {
            playerTransform->SetWorldPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
        }

        MazeGenerator maze(ROOM_COLUMNS, ROOM_ROWS, this);
        maze.Generate();
        enemyContainer->SetNavigationGrid(maze.GetGrid());
        // Draw the player after the floor and walls so level tiles cannot cover its sprite.
        world->MoveGameObjectToEnd(player->GetGameObject());
        if (persistentUiRoot != nullptr)
        {
            world->MoveGameObjectToEnd(persistentUiRoot);
        }
        const auto walkablePositions = maze.GetWalkablePositions();

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

        enemyContainer->SetAllEnemiesDefeatedCallback(
            [this](float worldX, float worldY)
            {
                XYZEngine::GameWorld::Instance()->ScheduleAfterUpdate(
                    [this, worldX, worldY]()
                    {
                        if (!runComplete)
                        {
                            CreateWorldPickup(ItemType::Key, worldX, worldY);
                            if (auto* hud =
                                    player->GetGameObject()->GetComponent<InventoryHudComponent>())
                            {
                                hud->ShowFeedback("Last enemy defeated. A key was dropped.");
                            }
                        }
                    });
            });

        CreeperSpawnSettings spawnSettings;
        spawnSettings.count = CREEPER_SPAWN_COUNT +
                              static_cast<unsigned int>((levelManager.GetCurrentLevel() - 1) * 2);
        CreeperSpawner creeperSpawner(spawnSettings);
        creeperSpawner.SpawnCreepers(*enemyContainer, player->GetGameObject(), walkablePositions);

        auto* inventory = player->GetGameObject()->GetComponent<InventoryComponent>();
        if (inventory != nullptr && !inventory->HasItem(ItemType::Bow) &&
            playerTransform != nullptr && !walkablePositions.empty())
        {
            const auto playerPosition = playerTransform->GetWorldPosition();
            constexpr float MIN_BOW_DISTANCE = 280.f;
            constexpr float MIN_ENEMY_DISTANCE = 88.f;
            std::vector<std::pair<float, float>> bowCandidates;
            for (const auto& position : walkablePositions)
            {
                const float dx = position.first - playerPosition.x;
                const float dy = position.second - playerPosition.y;
                if (dx * dx + dy * dy >= MIN_BOW_DISTANCE * MIN_BOW_DISTANCE &&
                    !enemyContainer->IsPositionNearEnemy(position.first, position.second,
                                                         MIN_ENEMY_DISTANCE))
                {
                    bowCandidates.push_back(position);
                }
            }

            const auto& spawnPoints = bowCandidates.empty() ? walkablePositions : bowCandidates;
            std::mt19937 randomEngine(std::random_device{}());
            std::uniform_int_distribution<std::size_t> distribution(0, spawnPoints.size() - 1);
            const auto& bowPosition = spawnPoints[distribution(randomEngine)];
            CreateWorldPickup(ItemType::Bow, bowPosition.first, bowPosition.second);
        }

        if (enemyContainer->GetEnemyCount() == 0 && !walkablePositions.empty())
        {
            CreateWorldPickup(ItemType::Key, walkablePositions.back().first,
                              walkablePositions.back().second);
        }

        std::vector<sf::Vector2f> potionSpawnPoints;
        potionSpawnPoints.reserve(walkablePositions.size());
        for (const auto& position : walkablePositions)
        {
            potionSpawnPoints.emplace_back(position.first, position.second);
        }
        if (auto* spawner = player->GetGameObject()->GetComponent<PotionSpawnerComponent>())
        {
            spawner->SetSpawnPoints(std::move(potionSpawnPoints),
                                    [this](ItemType type, sf::Vector2f position)
                                    { CreateWorldPickup(type, position.x, position.y); });
        }

        doorGameObject = world->CreateGameObject("LevelDoor");
        auto* doorRenderer = doorGameObject->AddComponent<XYZEngine::SpriteRendererComponent>();
        if (const sf::Texture* texture =
                XYZEngine::ResourceSystem::Instance()->GetTextureShared("door"))
        {
            doorRenderer->SetTexture(*texture);
            doorRenderer->SetPixelSize(static_cast<int>(TILE_SIZE), static_cast<int>(TILE_SIZE));
        }
        doorGameObject->GetComponent<XYZEngine::TransformComponent>()->SetWorldPosition(
            ROOM_OFFSET_X + doorCell.first * TILE_SIZE + HALF_TILE,
            ROOM_OFFSET_Y + doorCell.second * TILE_SIZE + HALF_TILE);
        doorGameObject->AddComponent<XYZEngine::SpriteColliderComponent>();
        doorGameObject->AddComponent<XYZEngine::RigidbodyComponent>()->SetKinematic(true);
        doorGameObject->AddComponent<DoorComponent>(player->GetGameObject(),
                                                    [this]() { AdvanceLevel(); });

        if (auto* hud = player->GetGameObject()->GetComponent<InventoryHudComponent>())
        {
            hud->SetLevelNumber(levelManager.GetCurrentLevel());
        }
        if (auto* hud = player->GetGameObject()->GetComponent<HealthBarComponent>())
        {
            hud->SetLevelNumber(levelManager.GetCurrentLevel());
        }
        if (auto* hud = player->GetGameObject()->GetComponent<QuestHudComponent>())
        {
            hud->SetLevelNumber(levelManager.GetCurrentLevel());
        }

        LOG_INFO("Generated level " + std::to_string(levelManager.GetCurrentLevel()));
    }

    void DeveloperLevel::AdvanceLevel()
    {
        if (runComplete)
        {
            return;
        }

        if (levelManager.Advance())
        {
            BuildLevel(true);
            if (auto* hud = player->GetGameObject()->GetComponent<InventoryHudComponent>())
            {
                hud->ShowFeedback("Level " + std::to_string(levelManager.GetCurrentLevel()) +
                                  ". Your health and inventory were preserved.");
            }
            return;
        }

        runComplete = true;
        if (doorGameObject != nullptr)
        {
            XYZEngine::GameWorld::Instance()->DestroyGameObject(doorGameObject);
            doorGameObject = nullptr;
        }
        if (auto* hud = player->GetGameObject()->GetComponent<InventoryHudComponent>())
        {
            hud->ShowRunComplete();
        }
    }

    bool DeveloperLevel::IsRunComplete() const
    {
        return runComplete;
    }

    void DeveloperLevel::SetPersistentUiRoot(XYZEngine::GameObject* uiRoot)
    {
        persistentUiRoot = uiRoot;
    }

    void DeveloperLevel::CreateWorldPickup(ItemType type, float worldX, float worldY)
    {
        if (player == nullptr)
        {
            return;
        }

        const char* textureName = GetItemTextureName(type);
        if (textureName == nullptr || textureName[0] == '\0')
        {
            return;
        }

        auto* pickup = XYZEngine::GameWorld::Instance()->CreateGameObject(std::string("Pickup_") +
                                                                          GetItemDisplayName(type));
        auto* renderer = pickup->AddComponent<XYZEngine::SpriteRendererComponent>();
        if (const sf::Texture* texture =
                XYZEngine::ResourceSystem::Instance()->GetTextureShared(textureName))
        {
            renderer->SetTexture(*texture);
            const int iconSize = type == ItemType::Key ? 30 : 34;
            renderer->SetPixelSize(iconSize, iconSize);
        }
        pickup->GetComponent<XYZEngine::TransformComponent>()->SetWorldPosition(worldX, worldY);
        pickup->AddComponent<ItemPickupComponent>(player->GetGameObject(), type);
    }

    std::pair<int, int> DeveloperLevel::ChooseDoorCell() const
    {
        std::vector<std::pair<int, int>> doorCells;
        for (int x = 1; x < ROOM_COLUMNS - 1; x += 2)
        {
            doorCells.emplace_back(x, 0);
            doorCells.emplace_back(x, ROOM_ROWS - 1);
        }
        for (int y = 1; y < ROOM_ROWS - 1; y += 2)
        {
            doorCells.emplace_back(0, y);
            doorCells.emplace_back(ROOM_COLUMNS - 1, y);
        }

        std::mt19937 randomEngine(std::random_device{}());
        std::uniform_int_distribution<std::size_t> distribution(0, doorCells.size() - 1);
        return doorCells[distribution(randomEngine)];
    }
} // namespace XYZRoguelike
