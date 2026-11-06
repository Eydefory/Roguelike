#include "FloorContainer.h"
#include "Constants.h"
#include "Loger.h"

#include <stdexcept>

namespace XYZRoguelike
{
    FloorContainer::FloorContainer()
    {
        CreateFloor();
    }

    void FloorContainer::CreateFloor()
    {
        tiles.clear();

        for (int y = 1; y < ROOM_ROWS - 1; ++y)
        {
            for (int x = 1; x < ROOM_COLUMNS - 1; ++x)
            {
                const float worldX = ROOM_OFFSET_X + x * TILE_SIZE;
                const float worldY = ROOM_OFFSET_Y + y * TILE_SIZE;

                tiles.push_back(std::make_unique<Floor>(worldX, worldY, TILE_SIZE));
            }
        }

        LOG_INFO("FloorContainer: floor created");
    }

    void FloorContainer::AddFloor(std::unique_ptr<Floor> floor)
    {
        if (!floor)
        {
            LOG_WARN("FloorContainer: nullptr floor");
            throw std::invalid_argument("Floor cannot be nullptr");
        }

        tiles.push_back(std::move(floor));
        LOG_INFO("FloorContainer: floor added");
    }
} // namespace XYZRoguelike
