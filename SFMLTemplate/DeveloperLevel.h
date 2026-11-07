
#pragma once

#include "EnemyContainer.h"
#include "FloorContainer.h"
#include "InventoryComponent.h"
#include "LevelManager.h"
#include "Player.h"
#include "Scene.h"
#include "WallContainer.h"

#include <memory>
#include <utility>

namespace XYZRoguelike
{
    class DeveloperLevel : public XYZEngine::Scene
    {
    public:
        void PrepareMenu();
        void Start() override;
        void Restart() override;
        void RestartKeeping(XYZEngine::GameObject* preservedRoot);
        void ReturnToMenuKeeping(XYZEngine::GameObject* preservedRoot);
        void Stop() override;
        bool IsRunComplete() const;
        void SetPersistentUiRoot(XYZEngine::GameObject* uiRoot);

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
        void LoadResources();
        void LoadMenuResources();
        void BuildLevel(bool preservePlayer, XYZEngine::GameObject* preservedRoot = nullptr);
        void AdvanceLevel();
        void CreateWorldPickup(ItemType type, float worldX, float worldY);
        std::pair<int, int> ChooseDoorCell() const;

        std::shared_ptr<Player> player;

        std::unique_ptr<EnemyContainer> enemyContainer;
        std::unique_ptr<WallContainer> wallContainer;
        std::unique_ptr<FloorContainer> floorContainer;
        LevelManager levelManager;
        XYZEngine::GameObject* doorGameObject = nullptr;
        XYZEngine::GameObject* persistentUiRoot = nullptr;
        bool resourcesLoaded = false;
        bool menuResourcesLoaded = false;
        bool runComplete = false;
    };
} // namespace XYZRoguelike
