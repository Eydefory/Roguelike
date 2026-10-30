#pragma once

#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "TransformComponent.h"

namespace XYZRoguelike
{
    class Floor
    {
    public:
        Floor(float x, float y, float size);

        XYZEngine::GameObject* GetGameObject();

    private:
        XYZEngine::GameObject* gameObject;
    };
}