#pragma once

#include "Wall.h"
#include <memory>
#include <vector>

namespace XYZRoguelike
{
    class WallContainer
    {
    public:
        explicit WallContainer(int exitCellX = -1, int exitCellY = -1);
        void CreateWalls();
        void AddWall(std::unique_ptr<Wall> wall);

    private:
        std::vector<std::unique_ptr<Wall>> walls;
        int exitCellX = -1;
        int exitCellY = -1;
    };
} // namespace XYZRoguelike
