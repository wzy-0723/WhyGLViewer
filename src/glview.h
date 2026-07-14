#ifndef GLVIEW_H
#define GLVIEW_H

#include <QOpenGLWidget>

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "opengl_renderer.h"
#include "singleton.h"


class GLView : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit GLView(QWidget *parent = nullptr);
    ~GLView();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

private:
    OpenGLRenderer* m_pOpenGLRenderer{ nullptr };


signals:
};



#endif // GLVIEW_H
