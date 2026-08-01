#include "Audio.h"
#include "Engine.h"
#include <miniaudio.h>

namespace why
{
    Audio::~Audio()
    {
        if (m_sound)
        {
            ma_sound_uninit(m_sound.get());
        }
        if (m_decoder)
        {
            ma_decoder_uninit(m_decoder.get());
        }
    }

/*设置 3D 空间音源坐标
传入 glm 三维世界坐标，设置音源在 3D 场景中的空间位置；
ma_sound_set_position：miniaudio 3D 空间音频接口，修改音源世界坐标，
引擎会依据听者位置自动计算左右声道音量差、衰减（立体空间音效）；
先判断音源有效再调用接口，防止空指针调用。
*/
    void Audio::SetPosition(const glm::vec3& position)
    {
        if (m_sound)
        {
            ma_sound_set_position(m_sound.get(), position.x, position.y, position.z);
        }
    }
/*播放音频，控制循环属性
ma_sound_start()：启动音源播放，从当前播放点位开始发声；
根据传入布尔值，转为 miniaudio 宏常量 MA_TRUE/MA_FALSE；
ma_sound_set_looping()：设置音源是否循环播放。
*/
    void Audio::Play(bool loop)
    {
        if (m_sound)
        {
            ma_sound_start(m_sound.get());
            auto looping = loop ? MA_TRUE : MA_FALSE;
            ma_sound_set_looping(m_sound.get(), looping);
        }
    }
/*播放音频，控制循环属性
ma_sound_stop()：暂停 / 终止音频播放；
ma_sound_seek_to_pcm_frame(..., 0)：将播放指针跳转至第 0 个 PCM 音频帧，也就是音频开头；
效果：停止播放后，下次调用 Play 会从头播放，而非从暂停位置继续。
*/
    void Audio::Stop()
    {
        if (m_sound)
        {
            ma_sound_stop(m_sound.get());
            ma_sound_seek_to_pcm_frame(m_sound.get(), 0);
        }
    }
/*查询播放状态
ma_sound_is_playing()：miniaudio 接口，返回布尔值标识音源当前是否正在播放；
音源不存在时统一返回 false。
*/
    bool Audio::IsPlaying() const
    {
        if (m_sound)
        {
            return ma_sound_is_playing(m_sound.get());
        }
        return false;
    }
/*设置音量
std::clamp：把传入音量数值限制在 0.0 ~ 1.0 区间；
ma_sound_set_volume()：设置音源音量系数，0 代表静音，1 代表原始最大音量。
*/
    void Audio::SetVolume(float volume)
    {
        if (m_sound)
        {
            ma_sound_set_volume(m_sound.get(), std::clamp(volume, 0.0f, 1.0f));
        }
    }
/*获取当前音量
调用 miniaudio 接口读取音源当前音量数值；无音源时返回 0。
*/
    float Audio::GetVolume() const
    {
        if (m_sound)
        {
            return ma_sound_get_volume(m_sound.get());
        }
        return 0.0f;
    }
/*静态资源加载函数（核心：读取音频文件 → 创建解码器 → 生成可播放音源）
*/
    std::shared_ptr<Audio> Audio::Load(const std::string& path)
    {
        auto buffer = SINGLETON_PTR(Engine)->GetFileSystem().LoadAssetFile(path);
        auto engine = SINGLETON_PTR(Engine)->GetAudioManager().GetEngine();

        auto audio = std::make_shared<Audio>();
        audio->m_sound = std::make_unique<ma_sound>();
        audio->m_buffer = buffer;
        audio->m_decoder = std::make_unique<ma_decoder>();
		/*miniaudio 内存解码器初始化接口：
直接基于内存中音频二进制数据创建解码器，无需读取磁盘文件；
参数依次：缓冲区首地址、缓冲区字节大小、解码配置、解码器实例指针。
*/
        auto result = ma_decoder_init_memory(audio->m_buffer.data(), audio->m_buffer.size(), 
            nullptr, audio->m_decoder.get());
        if (result != MA_SUCCESS)
        {
            return nullptr;
        }
		//判断解码器初始化结果，失败则返回空指针，资源加载终止。
        result = ma_sound_init_from_data_source(
            engine,
            audio->m_decoder.get(),
            0,
            NULL,
            audio->m_sound.get()
        );
        if (result != MA_SUCCESS)
        {
            return nullptr;
        }
//开启该音源的3D 空间音效计算，启用之后 SetPosition 设置的坐标才会生效，具备远近、方位听觉效果。
        ma_sound_set_spatialization_enabled(audio->m_sound.get(), MA_TRUE);
        return audio;
    }
}