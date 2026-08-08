#include "font/FontManager.h"
#include "Engine.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace why
{
    FontManager::~FontManager()
    {
        if (m_fontLibrary)
        {
            //作用：销毁由FT_Init_FreeType创建的FT_Library实例，释放 FreeType 内部全局内存、缓存。
            FT_Done_FreeType(m_fontLibrary);
            m_fontLibrary = nullptr;
        }
    }

    void FontManager::Init()
    {
        //初始化 FreeType 运行时，分配全局上下文句柄存入传入指针。
        FT_Error error = FT_Init_FreeType(&m_fontLibrary);
        if (error != FT_Err_Ok)
        {
            m_fontLibrary = nullptr;
            return;
        }
    }

    std::shared_ptr<Font> FontManager::GetFont(const std::string& path, int size)
    {
        // 查找缓存
        auto fontFamilyIt = m_fonts.find(path);
        if (fontFamilyIt != m_fonts.end())
        {
            auto fontIt = fontFamilyIt->second.find(size);
            if (fontIt != fontFamilyIt->second.end())
            {
                return fontIt->second;
            }
        }
        
        // 缓存找不到再加载
        auto buffer = SINGLETON_PTR(Engine)->GetFileSystem().LoadAssetFile(path);
        if (buffer.empty())
        {
            return nullptr;
        }

        FT_Face face;
        //直接从内存二进制缓冲区加载字体，适配引擎资源内存加载流程。
        FT_Error result = FT_New_Memory_Face(
            m_fontLibrary, reinterpret_cast<FT_Byte*>(buffer.data()), buffer.size(),
            0, &face);
        if (result != FT_Err_Ok)
        {
            return nullptr;
        }

        //设置字体渲染像素尺寸
        FT_Set_Pixel_Sizes(face, 0, size);

        //计算图集纹理尺寸
        const int lineHeight = face->size->metrics.height >> 6;
        int maxDimension = static_cast<int>(std::sqrt(128.0f) * (lineHeight + 1));
        int textureWidth = 1;
        while (textureWidth < maxDimension)
        {
            textureWidth <<= 1;
        }
        int textureHeight = textureWidth;

        const size_t stride = textureWidth * 4;
        const size_t totalBytes = static_cast<size_t>(textureWidth * textureHeight * 4);
        auto atlas = new unsigned char[totalBytes];
        std::memset(atlas, 0, totalBytes);

        int penX = 0;
        int penY = 0;

        auto font = std::make_shared<Font>();

        for (int c = 0; c < 128; ++c)
        {
            /*
            参数 2：字符 ASCII 编码；
            参数 3：加载标记FT_LOAD_RENDER：加载字形同时直接渲染生成灰度位图存入face->glyph->bitmap，无需手动渲染；
            */
            if (FT_Load_Char(face, c, FT_LOAD_RENDER) != FT_Err_Ok)
            {
                font->m_descriptions[c] = { 0, 0, 0, 0, 0, 0, 0 };
                continue;
            }
            /*
            bitmap：字符灰度位图数据，结构体FT_Bitmap：
            buffer：一维灰度字节数组，单通道 0 (透明)~255 (实心)
            width/rows：位图宽、高像素
            pitch：单行字节跨度（一行像素占用字节数）
            */

            FT_Bitmap& bmp = face->glyph->bitmap;

            if (penX + static_cast<int>(bmp.width) >= textureWidth)
            {
                penX = 0;
                penY += lineHeight + 1;
            }

            for (uint32_t row = 0; row < bmp.rows; ++row)
            {
                for (uint32_t col = 0; col < bmp.width; ++col)
                {
                    int x = penX + static_cast<int>(col);
                    int y = penY + static_cast<int>(row);
                    if (x < 0 || x >= textureWidth ||
                        y < 0 || y >= textureHeight)
                    {
                        continue;
                    }

                    const unsigned char value = bmp.buffer[row * bmp.pitch + col];
                    const size_t idx = static_cast<size_t>(y) * stride + x * 4;

                    atlas[idx + 0] = value;
                    atlas[idx + 1] = value;
                    atlas[idx + 2] = value;
                    atlas[idx + 3] = value;
                }
            }

            auto& gd = font->m_descriptions[c];
            gd.x0 = penX;
            gd.y0 = penY;
            gd.x1 = penX + static_cast<int>(bmp.width);
            gd.y1 = penY + static_cast<int>(bmp.rows);
            gd.width = static_cast<int>(bmp.width);
            gd.height = static_cast<int>(bmp.rows);
            gd.advance = (face->glyph->advance.x >> 6);

            penX += static_cast<int>(bmp.width + 1);
        }

        font->m_texture = std::make_shared<Texture>(textureWidth, textureHeight, 4, atlas);
        font->m_size = size;

        m_fonts[path][size] = font;

        delete[] atlas;
        FT_Done_Face(face);

        return font;
    }
}