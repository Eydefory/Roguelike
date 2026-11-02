#pragma once

#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "SpriteColliderComponent.h"
#include "TransformComponent.h"
#include "ResourceSystem.h"
#include "EnemySearchComponent.h"
#include "HealthComponent.h"

namespace XYZRoguelike
{
    class Enemy
    {
    public:
        Enemy(XYZEngine::GameObject* player);

        XYZEngine::GameObject* GetGameObject();
        HealthComponent* GetHealthComponent();

        void Update(float deltaTime);

    private:
        XYZEngine::GameObject* gameObject;

        float movementSpeed;
        float stopDistance;
    };
}