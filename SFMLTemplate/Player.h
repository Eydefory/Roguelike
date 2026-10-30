#pragma once

#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "SpriteColliderComponent.h"
#include "InputComponent.h"
#include "CameraComponent.h"
#include "RenderSystem.h"
#include "ResourceSystem.h"

namespace XYZRoguelike
{
    class Player
    {
    public:
        Player();

        XYZEngine::GameObject* GetGameObject();

        void Update(float deltaTime);

    private:
        XYZEngine::GameObject* gameObject;

        float movementSpeed;
    };
}