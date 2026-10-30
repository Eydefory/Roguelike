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

    private:
        std::vector<std::unique_ptr<Wall>> walls;
    };
}