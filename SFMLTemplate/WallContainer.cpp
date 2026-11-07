#include "WallContainer.h"
#include "Constants.h"
#include "Loger.h"

#include <stdexcept>

namespace XYZRoguelike
{
    WallContainer::WallContainer(int exitCellX, int exitCellY)
        : exitCellX(exitCellX), exitCellY(exitCellY)
    {
        CreateWalls();
    }

    void WallContainer::CreateWalls()
    {
        walls.clear();

        for (int x = 0; x < ROOM_COLUMNS; ++x)
        {
            if (exitCellX != x || exitCellY != 0)
            {
                walls.push_back(std::make_unique<Wall>(static_cast<float>(x), 0.f));
            }

            if (exitCellX != x || exitCellY != ROOM_ROWS - 1)
            {
                walls.push_back(std::make_unique<Wall>(static_cast<float>(x),
                                                       static_cast<float>(ROOM_ROWS - 1)));
            }
        }

        for (int y = 1; y < ROOM_ROWS - 1; ++y)
        {
            if (exitCellX != 0 || exitCellY != y)
            {
                walls.push_back(std::make_unique<Wall>(0.f, static_cast<float>(y)));
            }

            if (exitCellX != ROOM_COLUMNS - 1 || exitCellY != y)
            {
                walls.push_back(std::make_unique<Wall>(static_cast<float>(ROOM_COLUMNS - 1),
                                                       static_cast<float>(y)));
            }
        }

        LOG_INFO("WallContainer: walls created");
    }

    void WallContainer::AddWall(std::unique_ptr<Wall> wall)
    {
        if (!wall)
        {
            LOG_WARN("WallContainer: nullptr wall");
            throw std::invalid_argument("Wall cannot be nullptr");
        }

        walls.push_back(std::move(wall));
        LOG_INFO("WallContainer: wall added");
    }
} // namespace XYZRoguelike
