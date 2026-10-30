#pragma once

#include "EngineAPI.h"
#include "Component.h"
#include <SFML/Window.hpp>

namespace XYZEngine
{
    class XYZENGINE_API InputComponent : public Component
    {
    public:
        InputComponent(GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override;

        float GetHorizontalAxis() const;
        float GetVerticalAxis() const;

    private:
        float horizontalAxis = 0.f;
        float verticalAxis = 0.f;
    };
}