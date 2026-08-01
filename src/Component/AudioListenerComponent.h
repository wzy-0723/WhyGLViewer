#pragma once
#include "Component.h"

namespace why
{
    class AudioListenerComponent : public Component
    {
        COMPONENT(AudioListenerComponent)
    public:
        void Update(float deltaTime) override;
    };
}