#ifndef GLVIEW_H
#define GLVIEW_H

#include <QOpenGLWidget>

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "OpenGLRenderer.h"
#include "Singleton.h"
#include "Material.h"
#include "Mesh.h"

#include "Application.h"

//#include <glm/glm.hpp>

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

    virtual void event(QEvent e);
private:
    Renderer* m_pOpenGLRenderer{ nullptr };




    std::unique_ptr<why::Application> m_pApplication;

signals:
};



#endif // GLVIEW_H
