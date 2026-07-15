#ifndef GLVIEW_H
#define GLVIEW_H

#include <QOpenGLWidget>

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "OpenGLRenderer.h"
#include "Singleton.h"
#include "Material.h"

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
    Renderer* m_pOpenGLRenderer{ nullptr };


    why::Material m_material;

signals:
};



#endif // GLVIEW_H
