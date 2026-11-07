#pragma once

#include "Constants.h"

namespace XYZRoguelike
{
    class LevelManager
    {
    public:
        void StartRun()
        {
            currentLevel = 1;
        }

        bool HasNextLevel() const
        {
            return currentLevel > 0 && currentLevel < TOTAL_LEVEL_COUNT;
        }

        bool Advance()
        {
            if (!HasNextLevel())
            {
                return false;
            }

            ++currentLevel;
            return true;
        }

        int GetCurrentLevel() const
        {
            return currentLevel;
        }

        bool IsLastLevel() const
        {
            return currentLevel == TOTAL_LEVEL_COUNT;
        }

    private:
        int currentLevel = 0;
    };
} // namespace XYZRoguelike
