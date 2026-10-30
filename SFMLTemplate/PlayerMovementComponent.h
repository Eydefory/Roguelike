#pragma once

#include "Component.h"
#include "InputComponent.h"
#include "TransformComponent.h"

namespace XYZRoguelike
{
    class PlayerMovementComponent : public XYZEngine::Component
    {
    public:
        PlayerMovementComponent(XYZEngine::GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override;

    private:
        float movementSpeed;
    };
}