#ifndef MYOPENGL_H
#define MYOPENGL_H

#include <myopenglwidget.h>
#include "object.h"
#include <QMatrix4x4>
#include <QVector2D>
#include <QQuaternion>

class MyOpenGL : public MyOpenGLWidget
{
public:
    MyOpenGL();
	~MyOpenGL();
	
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    GLuint program;
    GLuint vao;
    GLuint buffer;
    GLuint texture[2];
    GLuint tex_index;
    GLint mv_location;
    GLint proj_location;

    float aspect;

    sb7::object obj;

    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;
    GLfloat xtrans, ytrans, ztrans; // translation on x, y, z-axis
    QVector2D mousePos;
    QQuaternion rotation;
};

#endif // MYOPENGL_H
