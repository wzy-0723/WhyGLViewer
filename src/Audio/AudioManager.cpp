#include "audio/AudioManager.h"
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

namespace why
{
    AudioManager::AudioManager()
    {
        m_engine = std::make_unique<ma_engine>();
    }

    AudioManager::~AudioManager()
    {
        if (m_engine)
        {
            ma_engine_uninit(m_engine.get());
        }
    }

    bool AudioManager::Init()
    {
        auto result = ma_engine_init(nullptr, m_engine.get());
        return result == MA_SUCCESS;
    }

    ma_engine* AudioManager::GetEngine()
    {
        return m_engine.get();
    }

/* 设置 3D 音效收听者位置
音频世界分为两部分：音源（Sound） + 收听者 (Listener，耳朵)；
第二个参数 0：miniaudio 默认仅有 0 号收听者（玩家相机 / 角色耳朵位置）；
传入三维世界坐标，设置收听者在场景中的位置；
引擎会根据音源位置、收听者位置距离、朝向，自动计算声音衰减、左右声道平衡，实现立体声 3D 音效。
游戏里每一帧跟随相机调用此函数，即可实现走路时远近声音变化、左右声源区分效果。
*/
    void AudioManager::SetListenerPosition(const glm::vec3& pos)
    {
        if (m_engine)
        {
            ma_engine_listener_set_position(m_engine.get(), 0, pos.x, pos.y, pos.z);
        }
    }
}