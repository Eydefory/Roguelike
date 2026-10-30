#pragma once

#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "SpriteColliderComponent.h"
#include "TransformComponent.h"
#include "ResourceSystem.h"
#include "EnemySearchComponent.h"

namespace XYZRoguelike
{
    class Enemy
    {
    public:
        Enemy(XYZEngine::GameObject* player);

        XYZEngine::GameObject* GetGameObject();

        void Update(float deltaTime);

    private:
        XYZEngine::GameObject* gameObject;

        float movementSpeed;
        float stopDistance;
    };
}