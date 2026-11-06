#include "WallContainer.h"
#include "Constants.h"
#include "Loger.h"

#include <stdexcept>

namespace XYZRoguelike
{
    WallContainer::WallContainer()
    {
        CreateWalls();
    }

    void WallContainer::CreateWalls()
    {
        walls.clear();

        for (int x = 0; x < ROOM_COLUMNS; ++x)
        {
            const bool left = x == 0;
            const bool right = x == ROOM_COLUMNS - 1;

            std::string topTexture;
            std::string bottomTexture;

            if (left)
            {
                topTexture = "wall_top_left";
                bottomTexture = "wall_bottom_left";
            }
            else if (right)
            {
                topTexture = "wall_top_right";
                bottomTexture = "wall_bottom_right";
            }
            else
            {
                topTexture = (x % 2 == 0) ? "wall_top" : "wall_top_alt";
                bottomTexture = (x % 2 == 0) ? "wall_bottom" : "wall_bottom_alt";
            }

            walls.push_back(std::make_unique<Wall>(static_cast<float>(x), 0.f, topTexture));

            walls.push_back(std::make_unique<Wall>(
                static_cast<float>(x), static_cast<float>(ROOM_ROWS - 1), bottomTexture));
        }

        for (int y = 1; y < ROOM_ROWS - 1; ++y)
        {
            const std::string leftTexture = (y % 2 == 0) ? "wall_left" : "wall_left_alt";

            const std::string rightTexture = (y % 2 == 0) ? "wall_right" : "wall_right_alt";

            walls.push_back(std::make_unique<Wall>(0.f, static_cast<float>(y), leftTexture));

            walls.push_back(std::make_unique<Wall>(static_cast<float>(ROOM_COLUMNS - 1),
                                                   static_cast<float>(y), rightTexture));
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
