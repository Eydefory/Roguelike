#pragma once

namespace XYZRoguelike
{
    constexpr unsigned int SCREEN_WIDTH = 1280;
    constexpr unsigned int SCREEN_HEIGHT = 720;

    constexpr float TILE_SIZE = 64.f;
    constexpr float HALF_TILE = TILE_SIZE * 0.5f;

    constexpr int ROOM_COLUMNS = 20;
    constexpr int ROOM_ROWS = 11;

    constexpr float ROOM_OFFSET_X = 0.f;
    constexpr float ROOM_OFFSET_Y = 8.f;

    constexpr float PLAYER_WIDTH = 48.f;
    constexpr float PLAYER_HEIGHT = 48.f;
    constexpr float PLAYER_SPEED = 250.f;

    constexpr float ENEMY_WIDTH = 48.f;
    constexpr float ENEMY_HEIGHT = 48.f;
    constexpr float ENEMY_SPEED = 120.f;

    constexpr float ENEMY_DETECTION_RADIUS = 300.f;
    constexpr float ENEMY_STOP_DISTANCE = 55.f;

    
    constexpr float ROOM_MIN_PLAYER_X =
        ROOM_OFFSET_X + TILE_SIZE + PLAYER_WIDTH * 0.5f;

    constexpr float ROOM_MAX_PLAYER_X =
        ROOM_OFFSET_X + ROOM_COLUMNS * TILE_SIZE - TILE_SIZE - PLAYER_WIDTH * 0.5f;

    constexpr float ROOM_MIN_PLAYER_Y =
        ROOM_OFFSET_Y + TILE_SIZE + PLAYER_HEIGHT * 0.5f;

    constexpr float ROOM_MAX_PLAYER_Y =
        ROOM_OFFSET_Y + ROOM_ROWS * TILE_SIZE - TILE_SIZE - PLAYER_HEIGHT * 0.5f;

    
    constexpr float ROOM_MIN_ENEMY_X =
        ROOM_OFFSET_X + TILE_SIZE + ENEMY_WIDTH * 0.5f;

    constexpr float ROOM_MAX_ENEMY_X =
        ROOM_OFFSET_X + ROOM_COLUMNS * TILE_SIZE - TILE_SIZE - ENEMY_WIDTH * 0.5f;

    constexpr float ROOM_MIN_ENEMY_Y =
        ROOM_OFFSET_Y + TILE_SIZE + ENEMY_HEIGHT * 0.5f;

    constexpr float ROOM_MAX_ENEMY_Y =
        ROOM_OFFSET_Y + ROOM_ROWS * TILE_SIZE - TILE_SIZE - ENEMY_HEIGHT * 0.5f;
}