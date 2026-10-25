#include "BonusCommand.h"

#include "GameState.h"

namespace XYZRoguelike
{
    void FireballCommand::Execute(GameState& gameState)
    {
        gameState.ApplyFireballBonus();
    }

    void ShrinkCommand::Execute(GameState& gameState)
    {
        gameState.ApplyShrinkBonus();
    }

    void SpeedCommand::Execute(GameState& gameState)
    {
        gameState.ApplySpeedBonus();
    }
}