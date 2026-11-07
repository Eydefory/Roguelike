#pragma once

#include "Floor.h"
#include <memory>
#include <vector>

namespace XYZRoguelike
{
    class FloorContainer
    {
    public:
        explicit FloorContainer(int exitCellX = -1, int exitCellY = -1);
        void CreateFloor();
        void AddFloor(std::unique_ptr<Floor> floor);

    private:
        std::vector<std::unique_ptr<Floor>> tiles;
        int exitCellX = -1;
        int exitCellY = -1;
    };
} // namespace XYZRoguelike
