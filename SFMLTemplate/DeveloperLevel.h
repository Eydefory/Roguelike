
#pragma once

#include "EnemyContainer.h"
#include "FloorContainer.h"
#include "Player.h"
#include "Scene.h"
#include "WallContainer.h"

#include <memory>

namespace XYZRoguelike
{
    class DeveloperLevel : public XYZEngine::Scene
    {
    public:
        void Start() override;
        void Restart() override;
        void Stop() override;

        FloorContainer* GetFloorContainer() const
        {
            return floorContainer.get();
        }

        WallContainer* GetWallContainer() const
        {
            return wallContainer.get();
        }

        Player* GetPlayer() const
        {
            return player.get();
        }

    private:
        std::shared_ptr<Player> player;

        std::unique_ptr<EnemyContainer> enemyContainer;
        std::unique_ptr<WallContainer> wallContainer;
        std::unique_ptr<FloorContainer> floorContainer;
    };
} // namespace XYZRoguelike
