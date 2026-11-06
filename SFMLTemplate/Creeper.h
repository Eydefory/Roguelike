#pragma once

#include "Enemy.h"

namespace XYZRoguelike
{
    // A faster, more durable enemy archetype with a longer detection range.
    class Creeper : public Enemy
    {
    public:
        Creeper(XYZEngine::GameObject* player, float worldX, float worldY);
    };
} // namespace XYZRoguelike
