#pragma once

#include "EngineAPI.h"

namespace XYZEngine
{
    class GameObject;

    class XYZENGINE_API Component
    {
    public:
        Component(GameObject* gameObject);
        virtual ~Component();

        virtual void Update(float deltaTime) = 0;
        virtual void Render() = 0;
        virtual bool IsOverlay() const
        {
            return false;
        }

        GameObject* GetGameObject();

    protected:
        GameObject* gameObject;
    };
} // namespace XYZEngine
