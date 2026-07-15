#include "glview.h"
#include "OpenGLFunc.h"
#include "GraphicsAPI.h"
#include "Game.h"

GLView::GLView(QWidget *parent)
    : QOpenGLWidget{parent}
{
    m_pApplication = std::make_unique<why::Game>();
}

GLView::~GLView()
{
    makeCurrent();
    doneCurrent();
}

void GLView::initializeGL()
{
    SINGLETON_PTR(OpenGLFunc)->initializeOpenGLFunctions();

    m_pApplication->Init();
}

void GLView::resizeGL(int w, int h)
{

}

void GLView::paintGL()
{
    SINGLETON_PTR(why::GraphicsAPI)->SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    SINGLETON_PTR(why::GraphicsAPI)->ClearBuffers();

    // 先更新数据，再提交绘制
    m_pApplication->Update(QEvent(QEvent::Type::None));
    m_pApplication->GetRenderQueue().Draw();
}

void GLView::event(QEvent e)
{
    
}

