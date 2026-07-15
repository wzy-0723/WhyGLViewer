#pragma once
#include <QOpenGLFunctions_4_5_Core>
#include "Singleton.h"


class OpenGLFunc : public QOpenGLFunctions_4_5_Core
{
public:
    ~OpenGLFunc() {};
    template<typename> friend class Singleton;
private:
    OpenGLFunc() {};    
};

#define OPENGLFUNC SINGLETON_PTR(OpenGLFunc)
