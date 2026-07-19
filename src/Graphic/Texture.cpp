#include "Engine.h"
#include "Texture.h"
#include "OpenGLFunc.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace why
{
    Texture::Texture(int width, int height, int numChannels, unsigned char* data)
        : m_width(width), m_height(height), m_numChannels(numChannels)
    {
        Init(width, height, numChannels, data);
    }

    Texture::~Texture()
    {
        if (m_textureID > 0)
        {
            GLCall(glDeleteTextures(1, &m_textureID));
        }
    }

    GLuint Texture::GetID() const
    {
        return m_textureID;
    }
    void Texture::Init(int width, int height, int numChannels, unsigned char* data)
    {
        GLCall(glGenTextures(1, &m_textureID));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_textureID));

        // 兼容RGB和RGBA
        GLint internalFormat = GL_RGB;
        GLenum format = GL_RGB;

        if (numChannels == 4)
        {
            internalFormat = GL_RGBA;
            format = GL_RGBA;
        }

        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data));

        GLCall(glGenerateMipmap(GL_TEXTURE_2D));

        //GL_REPEAT铺地板 设置纹理超出 UV 坐标 [0,1] 范围时的采样行为。
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

        //GL_TEXTURE_MIN_FILTER：纹理缩小过滤 GL_TEXTURE_MAG_FILTER纹理放大过滤
        //当绘制物体离镜头很远，贴图在屏幕上占的像素比原图纹理像素更少（纹理被缩小），此时用该参数决定采样算法。
        /*
            MIPMAP：多级渐远纹理。提前生成多张逐级缩小的纹理图，远距离渲染时自动选用更小的贴图，减少锯齿、闪烁、提升性能。
            前后两个 LINEAR：
            第一个：在选中的这一层 mipmap 内部，用双线性插值采样；
            第二个：在相邻两层 mipmap 之间，再做一次线性插值混合。
            效果：画面最平滑、远距离几乎无锯齿闪烁，是游戏 3D 场景标准最优缩小过滤。
            前提：必须调用 glGenerateMipmap(GL_TEXTURE_2D) 生成 mipmap 层级，否则该模式无效报错。
        */
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        /*
            放大时使用双线性插值，取纹理周边 4 个像素加权平均，画面柔和模糊，不会出现马赛克方块。
            MAG_FILTER 只有两种合法选项（OpenGL 规范限制，放大不支持 mipmap）
            GL_LINEAR：双线性平滑放大（常用）
            GL_NEAREST：最近点采样，硬像素马赛克风格
        */
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    }

    std::shared_ptr<Texture> Texture::Load(const std::string& path)
    {
        int width, height, numChannels;
        auto fullPath = SINGLETON_PTR(Engine)->GetFileSystem().GetAssetsFolder() / path;

        if (!std::filesystem::exists(fullPath))
        {
            return nullptr;
        }

        std::shared_ptr<Texture> result;

        unsigned char* data = stbi_load(fullPath.string().c_str(), &width, &height, &numChannels, 0);

        if (data)
        {
            result = std::make_shared<Texture>(width, height, numChannels, data);
            stbi_image_free(data);
        }
        else
        {
            LOG_ERROR << "texture load failed,path:" << fullPath;
        }

        return result;
    }
}