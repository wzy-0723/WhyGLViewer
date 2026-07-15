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
        virtual void Update(const QEvent& e) = 0;
        virtual void Destroy() = 0;

        RenderQueue& GetRenderQueue() { return m_rederQueue; };

    protected:
    private:

        RenderQueue m_rederQueue;        
    };
}