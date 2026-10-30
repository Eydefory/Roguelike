#include "FloorContainer.h"
#include "Constants.h"

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
                const float worldX =
                    ROOM_OFFSET_X + x * TILE_SIZE;

                const float worldY =
                    ROOM_OFFSET_Y + y * TILE_SIZE;

                tiles.push_back(std::make_unique<Floor>(
                    worldX,
                    worldY,
                    TILE_SIZE
                ));
            }
        }
    }
}