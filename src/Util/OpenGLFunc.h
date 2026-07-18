#pragma once
#include <QOpenGLFunctions_4_5_Core>
#include "Singleton.h"
#include "Logger.h"

class OpenGLFunc : public QOpenGLFunctions_4_5_Core
{
public:
    ~OpenGLFunc() {};
    template<typename> friend class Singleton;
private:
    OpenGLFunc() {};    
};

// 有返回值的gl函数使用
#define OPENGLFUNC SINGLETON_PTR(OpenGLFunc)

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	SINGLETON_PTR(OpenGLFunc)->x;\
	ASSERT(GLLogCall(#x,__FILE__,__LINE__))

inline void GLClearError()
{
	while (SINGLETON_PTR(OpenGLFunc)->glGetError() != GL_NO_ERROR);
}

inline bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = SINGLETON_PTR(OpenGLFunc)->glGetError())
	{
		LOG_ERROR << "[OpenGL Error](" << error << "):" <<
			function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}