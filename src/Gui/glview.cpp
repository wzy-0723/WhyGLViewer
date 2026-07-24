#include "glview.h"
#include "OpenGLFunc.h"
#include "GraphicsAPI.h"
#include "Game.h"
#include <qevent.h>
#include "Engine.h"

GLView::GLView(QWidget *parent)
    : QOpenGLWidget{parent}
{
    // 点击窗口即可获取键盘焦点
    this->setFocusPolicy(Qt::ClickFocus);
    this->setCursor(Qt::BlankCursor);
}

GLView::~GLView()
{
    makeCurrent();
    doneCurrent();
}

void GLView::initializeGL()
{
    SINGLETON_PTR(OpenGLFunc)->initializeOpenGLFunctions();
    SINGLETON_PTR(why::Engine)->Init();
}

void GLView::resizeGL(int w, int h)
{
    OPENGLFUNC->glViewport(0, 0, w, h);
}


void GLView::paintGL()
{
    SINGLETON_PTR(why::Engine)->Run();

    SINGLETON_PTR(why::Engine)->GetInputManager()->ListenMouseMove(QCursor::pos());

    // 模拟glfw循环绘制效果，只是投递异步消息，不会爆栈
    update();
}

bool GLView::event(QEvent* e)
{    
    //@todo
    bool res = SINGLETON_PTR(why::Engine)->GetInputManager()->HandleEvent(e);

    if (res)
    {
        return true;
    }
    else
    {
        // 其他所有事件交给父类默认处理，必须写这一行
        return QOpenGLWidget::event(e);
    }
}

