#pragma once

#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "SpriteColliderComponent.h"
#include "InputComponent.h"
#include "CameraComponent.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"
#include "HealthComponent.h"

namespace XYZRoguelike
{
    class EnemyContainer;

    class Player
    {
    public:
        Player();

        XYZEngine::GameObject* GetGameObject();

        HealthComponent* GetHealthComponent();

        void Update(float deltaTime);

        void SetEnemyContainer(EnemyContainer* container);

    private:
        XYZEngine::GameObject* gameObject;
        float movementSpeed;
    };
}