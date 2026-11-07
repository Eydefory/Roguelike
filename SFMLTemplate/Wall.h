#pragma once

#include "GameWorld.h"
namespace XYZRoguelike
{
    class Wall
    {
    public:
        Wall(float cellX, float cellY);

        XYZEngine::GameObject* GetGameObject() const;

    private:
        XYZEngine::GameObject* gameObject = nullptr;
    };
} // namespace XYZRoguelike
