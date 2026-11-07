#pragma once

#include "EngineAPI.h"
#include "GameObject.h"
#include "PhysicsSystem.h"

#include <functional>

namespace XYZEngine
{
    class XYZENGINE_API GameWorld
    {
    public:
        static GameWorld* Instance();

        void Update(float deltaTime);
        void FixedUpdate(float deltaTime);
        void Render();
        void LateUpdate();

        GameObject* CreateGameObject();
        GameObject* CreateGameObject(std::string name);
        void MoveGameObjectToEnd(GameObject* gameObject);
        void DestroyGameObject(GameObject* gameObject);
        void Clear();
        void ClearExcept(GameObject* preservedRoot);
        void ClearExcept(GameObject* firstPreservedRoot, GameObject* secondPreservedRoot);
        void ScheduleAfterUpdate(std::function<void()> action);
        void SetPaused(bool paused);
        bool IsPaused() const;

        void Print() const;

    private:
        GameWorld() {}
        ~GameWorld() {}

        GameWorld(GameWorld const&) = delete;
        GameWorld& operator=(GameWorld const&) = delete;

        float fixedCounter = 0.f;
        bool paused = false;

        std::vector<GameObject*> gameObjects = {};
        std::vector<GameObject*> markedToDestroyGameObjects = {};
        std::vector<std::function<void()>> postUpdateActions;

        void DestroyGameObjectImmediate(GameObject* gameObject);
    };
} // namespace XYZEngine
