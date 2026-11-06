#pragma once

#include "Floor.h"
#include <memory>
#include <vector>

namespace XYZRoguelike
{
    class FloorContainer
    {
    public:
        FloorContainer();
        void CreateFloor();
        void AddFloor(std::unique_ptr<Floor> floor);

    private:
        std::vector<std::unique_ptr<Floor>> tiles;
    };
} // namespace XYZRoguelike
