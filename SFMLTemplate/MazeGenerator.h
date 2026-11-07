#pragma once

#include <random>
#include <utility>
#include <vector>

namespace XYZRoguelike
{
    class DeveloperLevel;

    class MazeGenerator
    {
    public:
        MazeGenerator(int width, int height, DeveloperLevel* level);

        void Generate();

        // Returns world-space tile centers for cells that are not walls.
        std::vector<std::pair<float, float>> GetWalkablePositions() const;
        const std::vector<std::vector<bool>>& GetGrid() const;

    private:
        int width;
        int height;

        DeveloperLevel* level;

        std::vector<std::vector<bool>> grid;

        std::mt19937 randomEngine;

        std::vector<std::pair<int, int>> GetAvailableDirections(int x, int y);

        void RemoveWall(int x1, int y1, int x2, int y2);

        void CreateInternalWalls();
    };
} // namespace XYZRoguelike
