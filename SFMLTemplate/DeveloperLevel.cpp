#include "DeveloperLevel.h"

#include "Constants.h"
#include "ResourceSystem.h"
#include "GameWorld.h"
#include "AudioManager.h"

namespace XYZRoguelike
{
    void DeveloperLevel::Start()
    {
        XYZEngine::ResourceSystem* resources =
            XYZEngine::ResourceSystem::Instance();

        AudioManager::Instance().PlayMusic("Resources/Music/music.ogg");

        resources->LoadTexture(
            "player",
            "Resources/Textures/player.png"
        );

        resources->LoadTexture(
            "enemy",
            "Resources/Textures/enemy.png"
        );

        resources->LoadTexture(
            "floor",
            "Resources/Textures/floor.png"
        );

        resources->LoadTexture(
            "wall_top",
            "Resources/Textures/wall_top.png"
        );

        resources->LoadTexture(
            "wall_top_alt",
            "Resources/Textures/wall_top_alt.png"
        );

        resources->LoadTexture(
            "wall_bottom",
            "Resources/Textures/wall_bottom.png"
        );

        resources->LoadTexture(
            "wall_bottom_alt",
            "Resources/Textures/wall_bottom_alt.png"
        );

        resources->LoadTexture(
            "wall_left",
            "Resources/Textures/wall_left.png"
        );

        resources->LoadTexture(
            "wall_left_alt",
            "Resources/Textures/wall_left_alt.png"
        );

        resources->LoadTexture(
            "wall_right",
            "Resources/Textures/wall_right.png"
        );

        resources->LoadTexture(
            "wall_right_alt",
            "Resources/Textures/wall_right_alt.png"
        );

        resources->LoadTexture(
            "wall_top_left",
            "Resources/Textures/wall_top_left.png"
        );

        resources->LoadTexture(
            "wall_top_right",
            "Resources/Textures/wall_top_right.png"
        );

        resources->LoadTexture(
            "wall_bottom_left",
            "Resources/Textures/wall_bottom_left.png"
        );

        resources->LoadTexture(
            "wall_bottom_right",
            "Resources/Textures/wall_bottom_right.png"
        );

        floorContainer = std::make_unique<FloorContainer>();
        wallContainer = std::make_unique<WallContainer>();

        player = std::make_shared<Player>();

        enemyContainer = std::make_unique<EnemyContainer>(
            player->GetGameObject()
        );
    }

    void DeveloperLevel::Restart()
    {
        Stop();
        Start();
    }

    void DeveloperLevel::Stop()
    {
        AudioManager::Instance().StopMusic();

        XYZEngine::GameWorld::Instance()->Clear();

        enemyContainer.reset();
        wallContainer.reset();
        floorContainer.reset();
        player.reset();
    }
}