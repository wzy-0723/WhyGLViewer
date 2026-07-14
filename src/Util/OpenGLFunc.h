#include <QOpenGLFunctions_4_5_Core>

class OpenGLFunc : public QOpenGLFunctions_4_5_Core
{
public:
    ~OpenGLFunc() {};
    template<typename> friend class Singleton;
private:
    OpenGLFunc() {};
};


