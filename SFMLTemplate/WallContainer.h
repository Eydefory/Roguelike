#pragma once

#include "Wall.h"
#include <memory>
#include <vector>

namespace XYZRoguelike
{
    class WallContainer
    {
    public:
        WallContainer();
        void CreateWalls();
        void AddWall(std::unique_ptr<Wall> wall);

    private:
        std::vector<std::unique_ptr<Wall>> walls;
    };
} // namespace XYZRoguelike
