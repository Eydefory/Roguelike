#pragma once

#include "Component.h"

#include <functional>

namespace XYZRoguelike
{
    class DoorComponent : public XYZEngine::Component
    {
    public:
        DoorComponent(XYZEngine::GameObject* gameObject, XYZEngine::GameObject* player,
                      std::function<void()> onOpened);

        void Update(float deltaTime) override;
        void Render() override;

    private:
        XYZEngine::GameObject* player = nullptr;
        std::function<void()> onOpened;
        bool interactKeyWasPressed = false;
    };
} // namespace XYZRoguelike
