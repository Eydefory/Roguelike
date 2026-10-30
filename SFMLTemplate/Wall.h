#pragma once

#include "GameWorld.h"
#include <string>

namespace XYZRoguelike
{
    class Wall
    {
    public:
        Wall(float cellX, float cellY, const std::string& textureName);

        XYZEngine::GameObject* GetGameObject() const;

    private:
        XYZEngine::GameObject* gameObject = nullptr;
    };
}