#pragma once

#include "EngineAPI.h"
#include "GameObject.h"
#include "PhysicsSystem.h"

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
        void DestroyGameObject(GameObject* gameObject);
        void Clear();

        void Print() const;

    private:
        GameWorld() {}
        ~GameWorld() {}

        GameWorld(GameWorld const&) = delete;
        GameWorld& operator=(GameWorld const&) = delete;

        float fixedCounter = 0.f;

        std::vector<GameObject*> gameObjects = {};
        std::vector<GameObject*> markedToDestroyGameObjects = {};

        void DestroyGameObjectImmediate(GameObject* gameObject);
    };
}