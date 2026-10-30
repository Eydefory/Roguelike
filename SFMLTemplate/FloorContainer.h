#pragma once

#include "Floor.h"
#include <vector>
#include <memory>

namespace XYZRoguelike
{
    class FloorContainer
    {
    public:
        FloorContainer();

        void CreateFloor();

    private:
        std::vector<std::unique_ptr<Floor>> tiles;
    };
}