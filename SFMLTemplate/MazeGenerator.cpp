#include "MazeGenerator.h"

#include "Constants.h"
#include "DeveloperLevel.h"
#include "Floor.h"
#include "FloorContainer.h"
#include "Loger.h"
#include "Wall.h"
#include "WallContainer.h"

#include <algorithm>
#include <chrono>
#include <memory>
#include <stdexcept>

namespace XYZRoguelike
{
    MazeGenerator::MazeGenerator(int width, int height, DeveloperLevel* level)
        : width(width), height(height), level(level),
          randomEngine(static_cast<unsigned int>(
              std::chrono::steady_clock::now().time_since_epoch().count()))
    {
        if (width < 5 || height < 5)
        {
            throw std::invalid_argument("Maze dimensions must be at least 5");
        }

        if (level == nullptr)
        {
            throw std::invalid_argument("DeveloperLevel cannot be nullptr");
        }

        grid.resize(height, std::vector<bool>(width, true));
    }

    void MazeGenerator::Generate()
    {
        LOG_INFO("MazeGenerator: generation started");

        grid.assign(height, std::vector<bool>(width, true));

        int startX = 1;
        int startY = 1;

        grid[startY][startX] = false;

        std::vector<std::pair<int, int>> stack;

        stack.push_back({startX, startY});

        while (!stack.empty())
        {
            auto current = stack.back();

            int x = current.first;
            int y = current.second;

            auto directions = GetAvailableDirections(x, y);

            if (directions.empty())
            {
                stack.pop_back();
                continue;
            }

            std::uniform_int_distribution<int> distribution(0, static_cast<int>(directions.size()) -
                                                                   1);

            auto direction = directions[distribution(randomEngine)];

            int nx = x + direction.first;
            int ny = y + direction.second;

            RemoveWall(x, y, nx, ny);

            grid[ny][nx] = false;

            stack.push_back({nx, ny});
        }

        CreateInternalWalls();

        LOG_INFO("MazeGenerator: generation completed");
    }

    std::vector<std::pair<float, float>> MazeGenerator::GetWalkablePositions() const
    {
        std::vector<std::pair<float, float>> positions;

        for (int y = 1; y < height - 1; ++y)
        {
            for (int x = 1; x < width - 1; ++x)
            {
                if (grid[y][x])
                {
                    continue;
                }

                const float worldX = ROOM_OFFSET_X + static_cast<float>(x) * TILE_SIZE + HALF_TILE;
                const float worldY = ROOM_OFFSET_Y + static_cast<float>(y) * TILE_SIZE + HALF_TILE;
                positions.emplace_back(worldX, worldY);
            }
        }

        return positions;
    }

    std::vector<std::pair<int, int>> MazeGenerator::GetAvailableDirections(int x, int y)
    {
        std::vector<std::pair<int, int>> directions = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};

        std::vector<std::pair<int, int>> available;

        for (const auto& direction : directions)
        {
            int nx = x + direction.first;
            int ny = y + direction.second;

            if (nx <= 0 || nx >= width - 1)
                continue;

            if (ny <= 0 || ny >= height - 1)
                continue;

            if (grid[ny][nx])
            {
                available.push_back(direction);
            }
        }

        return available;
    }

    void MazeGenerator::RemoveWall(int x1, int y1, int x2, int y2)
    {
        int wallX = (x1 + x2) / 2;
        int wallY = (y1 + y2) / 2;

        grid[wallY][wallX] = false;
    }

    void MazeGenerator::CreateInternalWalls()
    {
        auto walls = level->GetWallContainer();

        if (walls == nullptr)
        {
            LOG_ERROR("MazeGenerator: WallContainer is nullptr");
            return;
        }

        for (int y = 1; y < height - 1; ++y)
        {
            for (int x = 1; x < width - 1; ++x)
            {
                if (!grid[y][x])
                    continue;

                walls->AddWall(std::make_unique<Wall>(static_cast<float>(x), static_cast<float>(y),
                                                      "wall_top"));
            }
        }

        LOG_INFO("MazeGenerator: internal walls created");
    }
} // namespace XYZRoguelike
