#pragma once
#include <qcoreevent.h>
#include "RenderQueue.h"

namespace why
{
    class Application
    {
    public:
        virtual bool Init() = 0;
        // deltaTime in seconds
        virtual void Update(float deltaTime) = 0;
        virtual void Destroy() = 0;
    };
}