#include "AudioListenerComponent.h"
#include "GameObject.h"
#include "Engine.h"

namespace why
{
    void AudioListenerComponent::Update(float deltaTime)
    {
        auto pos = m_owner->GetWorldPosition();
        
        SINGLETON_PTR(Engine)->GetAudioManager().SetListenerPosition(pos);
    }
}