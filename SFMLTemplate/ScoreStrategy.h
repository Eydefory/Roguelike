#pragma once

namespace XYZRoguelike
{
    class Brick;

    class ScoreStrategy
    {
    public:
        virtual ~ScoreStrategy() = default;

        virtual int GetPoints(
            const Brick& brick
        ) const = 0;
    };

    class StandardScoreStrategy : public ScoreStrategy
    {
    public:
        int GetPoints(
            const Brick& brick
        ) const override;
    };
}