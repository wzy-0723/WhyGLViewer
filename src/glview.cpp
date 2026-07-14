#include "glview.h"

GLView::GLView(QWidget *parent)
    : QOpenGLWidget{parent}
{}

GLView::~GLView()
{
    makeCurrent();
    doneCurrent();
}

void GLView::initializeGL()
{
    m_pOpenGLRenderer = new OpenGLRenderer();
    m_pOpenGLRenderer->InitShader({ ":/shader/1.model_loading.vert" ,":/shader/1.model_loading.frag" });
}

void GLView::resizeGL(int w, int h)
{

}

void GLView::paintGL()
{
    m_pOpenGLRenderer->ClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    m_pOpenGLRenderer->Draw(
        {
        -0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f,
        0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f,
        0.0f,  0.5f, 0.0f, 0.f, 0.f, 1.f
        },
        { 0, 1, 2 }
    );
}

