#pragma once

namespace XYZRoguelike
{
    class GameState;

    class BonusCommand
    {
    public:
        virtual ~BonusCommand() = default;

        virtual void Execute(GameState& gameState) = 0;
    };

    class FireballCommand : public BonusCommand
    {
    public:
        void Execute(GameState& gameState) override;
    };

    class ShrinkCommand : public BonusCommand
    {
    public:
        void Execute(GameState& gameState) override;
    };

    class SpeedCommand : public BonusCommand
    {
    public:
        void Execute(GameState& gameState) override;
    };
}