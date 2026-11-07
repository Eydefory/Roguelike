#pragma once

namespace XYZRoguelike
{
    constexpr unsigned int SCREEN_WIDTH = 1280;
    constexpr unsigned int SCREEN_HEIGHT = 720;

    constexpr float TILE_SIZE = 56.f;
    constexpr float HALF_TILE = TILE_SIZE * 0.5f;

    constexpr int ROOM_COLUMNS = 23;
    constexpr int ROOM_ROWS = 13;
    constexpr unsigned int INVENTORY_SLOT_COUNT = 12;
    constexpr int TOTAL_LEVEL_COUNT = 3;

    constexpr float ROOM_OFFSET_X = 0.f;
    constexpr float ROOM_OFFSET_Y = 8.f;

    constexpr float PLAYER_WIDTH = 48.f;
    constexpr float PLAYER_HEIGHT = 48.f;
    constexpr float PLAYER_SPEED = 250.f;
    constexpr int PLAYER_MAX_HEALTH = 100;
    constexpr int PLAYER_STARTING_ARMOR = 5;
    constexpr int PLAYER_ATTACK_DAMAGE = 20;
    constexpr int PLAYER_BOW_DAMAGE = 25;
    constexpr float PLAYER_ATTACK_COOLDOWN = 0.5f;
    constexpr float PLAYER_ATTACK_RANGE = 72.f;
    constexpr float PLAYER_BOW_RANGE = 650.f;
    constexpr float PLAYER_PROJECTILE_SPEED = 520.f;
    constexpr int PLAYER_HEALING_POTION_AMOUNT = 40;
    constexpr int PLAYER_SHIELD_POTION_AMOUNT = 40;
    constexpr float PLAYER_STAMINA_POTION_DURATION = 5.f;
    constexpr float ITEM_PICKUP_RANGE = 38.f;
    constexpr float DOOR_INTERACTION_RANGE = 90.f;
    constexpr float PLAYER_SPRINT_SPEED_MULTIPLIER = 1.5f;
    constexpr float PLAYER_MAX_STAMINA = 100.f;
    constexpr float PLAYER_STAMINA_DRAIN_PER_SECOND = 35.f;
    constexpr float PLAYER_STAMINA_REGEN_PER_SECOND = 22.f;
    constexpr float PLAYER_STAMINA_REGEN_DELAY = 0.6f;
    constexpr float ENEMY_WIDTH = 48.f;
    constexpr float ENEMY_HEIGHT = 48.f;
    constexpr float ENEMY_SPEED = 120.f;

    constexpr float ENEMY_DETECTION_RADIUS = 300.f;
    constexpr float ENEMY_STOP_DISTANCE = 55.f;
    constexpr int ENEMY_MAX_HEALTH = 60;
    constexpr int ENEMY_ARMOR = 10;
    constexpr int ENEMY_DAMAGE = 10;
    constexpr float ENEMY_ATTACK_COOLDOWN = 1.f;
    constexpr float ENEMY_ATTACK_DISTANCE = 55.f;

    constexpr int CREEPER_MAX_HEALTH = 80;
    constexpr int CREEPER_ARMOR = 0;
    constexpr int CREEPER_DAMAGE = 15;
    constexpr float CREEPER_SPEED = 140.f;
    constexpr float CREEPER_DETECTION_RADIUS = 360.f;
    constexpr float CREEPER_STOP_DISTANCE = 48.f;
    constexpr float CREEPER_ATTACK_COOLDOWN = 1.5f;
    constexpr float CREEPER_ATTACK_DISTANCE = 60.f;
    constexpr unsigned int CREEPER_SPAWN_COUNT = 5;
    constexpr float CREEPER_MIN_PLAYER_DISTANCE = 200.f;

    constexpr float ROOM_MIN_PLAYER_X = ROOM_OFFSET_X + TILE_SIZE + PLAYER_WIDTH * 0.5f;

    constexpr float ROOM_MAX_PLAYER_X =
        ROOM_OFFSET_X + ROOM_COLUMNS * TILE_SIZE - TILE_SIZE - PLAYER_WIDTH * 0.5f;

    constexpr float ROOM_MIN_PLAYER_Y = ROOM_OFFSET_Y + TILE_SIZE + PLAYER_HEIGHT * 0.5f;

    constexpr float ROOM_MAX_PLAYER_Y =
        ROOM_OFFSET_Y + ROOM_ROWS * TILE_SIZE - TILE_SIZE - PLAYER_HEIGHT * 0.5f;

    constexpr float ROOM_MIN_ENEMY_X = ROOM_OFFSET_X + TILE_SIZE + ENEMY_WIDTH * 0.5f;

    constexpr float ROOM_MAX_ENEMY_X =
        ROOM_OFFSET_X + ROOM_COLUMNS * TILE_SIZE - TILE_SIZE - ENEMY_WIDTH * 0.5f;

    constexpr float ROOM_MIN_ENEMY_Y = ROOM_OFFSET_Y + TILE_SIZE + ENEMY_HEIGHT * 0.5f;

    constexpr float ROOM_MAX_ENEMY_Y =
        ROOM_OFFSET_Y + ROOM_ROWS * TILE_SIZE - TILE_SIZE - ENEMY_HEIGHT * 0.5f;
} // namespace XYZRoguelike
