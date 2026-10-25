#pragma once

namespace XYZRoguelike
{
    constexpr unsigned int SCREEN_WIDTH = 800;
    constexpr unsigned int SCREEN_HEIGHT = 600;

    constexpr float PLATFORM_WIDTH = 120.f;
    constexpr float PLATFORM_HEIGHT = 20.f;
    constexpr float PLATFORM_SPEED = 450.f;

    constexpr float BALL_RADIUS = 9.f;
    constexpr float BALL_SPEED = 350.f;

    constexpr int BLOCK_ROWS = 5;
    constexpr int BLOCK_COLUMNS = 10;

    constexpr float BLOCK_WIDTH = 68.f;
    constexpr float BLOCK_HEIGHT = 25.f;
    constexpr float BLOCK_GAP = 5.f;

    constexpr float BLOCK_START_X = 35.f;
    constexpr float BLOCK_START_Y = 70.f;

    constexpr int START_LIVES = 3;

    constexpr float BONUS_SIZE = 22.f;
    constexpr float BONUS_FALL_SPEED = 130.f;
    constexpr float BONUS_DROP_CHANCE = 100.f;
    constexpr float BONUS_DURATION = 7.f;

    constexpr float FIREBALL_SPEED_MULTIPLIER = 1.15f;
    constexpr float SPEED_BONUS_MULTIPLIER = 1.45f;
    constexpr float SHRINK_PLATFORM_WIDTH = 70.f;
}

//hi