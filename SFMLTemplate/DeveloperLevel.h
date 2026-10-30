#pragma once

#include "Scene.h"
#include "Player.h"
#include "EnemyContainer.h"
#include "WallContainer.h"
#include "FloorContainer.h"

#include <memory>

namespace XYZRoguelike
{
    class DeveloperLevel : public XYZEngine::Scene
    {
    public:
        void Start() override;
        void Restart() override;
        void Stop() override;

    private:
        std::shared_ptr<Player> player;

        std::unique_ptr<EnemyContainer> enemyContainer;
        std::unique_ptr<WallContainer> wallContainer;
        std::unique_ptr<FloorContainer> floorContainer;
    };
}