#pragma once

#include "CameraComponent.h"
#include "GameWorld.h"
#include "HealthComponent.h"
#include "InputComponent.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"
#include "SpriteColliderComponent.h"
#include "SpriteRendererComponent.h"

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
} // namespace XYZRoguelike