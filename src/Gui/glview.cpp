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

}


void GLView::paintGL()
{
    SINGLETON_PTR(why::Engine)->Run();
    // 模拟glfw循环绘制效果，只是投递异步消息，不会爆栈
    update();
}

bool GLView::event(QEvent* e)
{
    
    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEv = static_cast<QKeyEvent*>(e);
        int key = keyEv->key(); // 获取按键枚举 Qt::Key_XXX
        SINGLETON_PTR(why::Engine)->GetInputManager()->SetKeyPressed(static_cast<Qt::Key>(key), true);
        //// 示例常用按键
        //switch (key)
        //{
        //case Qt::Key_W:
        //    break;
        //case Qt::Key_S:
        //    break;
        //case Qt::Key_Escape:
        //    break;
        //case Qt::Key_Shift:
        //case Qt::Key_Control:
        //case Qt::Key_Alt:
        //    break;
        //default:
        //    break;
        //}

        return true;
    }
    else if (e->type() == QEvent::KeyRelease)
    {
        QKeyEvent* keyEv = static_cast<QKeyEvent*>(e);
        int key = keyEv->key();
        SINGLETON_PTR(why::Engine)->GetInputManager()->SetKeyPressed(static_cast<Qt::Key>(key), false);
        return true;
    }

    // 其他所有事件交给父类默认处理，必须写这一行
    return QOpenGLWidget::event(e);
}

