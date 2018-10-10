#include "myopengl.h"
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>

MyOpenGL::MyOpenGL() : tex_index(0)
{
    this->grabKeyboard();
}

MyOpenGL::~MyOpenGL()
{
	glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &buffer);
    glDeleteTextures(2, texture);
	glDeleteProgram(program);
}

void MyOpenGL::initializeGL()
{
    initializeOpenGLFunctions();

    // Set the tex
#define B 0x00, 0x00, 0x00, 0x00
#define W 0xFF, 0xFF, 0xFF, 0xFF
   static const GLubyte tex_data[] = {
        B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
        W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
        B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
        W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
        B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
        W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
        B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
        W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
        B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
        W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
        B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
        W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
        B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
        W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
        B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
       // This line will cause a IDE warning : reach parse limit for initializer clause
//        W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B
    };
#undef B
#undef W

   glCreateTextures(GL_TEXTURE_2D, 1, texture);
   glBindTexture(GL_TEXTURE_2D, texture[0]);
   glTextureStorage2D(texture[0], 1, GL_RGBA8, 16, 15);
   glTextureSubImage2D(texture[0], 0, 0, 0, 16, 15, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
   glTextureParameteri(texture[0], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTextureParameteri(texture[0], GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   texture[1] = LoadKtx("../textures/pattern1.ktx");

   obj.load("../objects/torus_nrms_tc.sbm");

   shaderInfo shaders[] = {
       { GL_VERTEX_SHADER, "../shaders/simpletexcoords/render.vs.glsl", 0 },
       { GL_FRAGMENT_SHADER, "../shaders/simpletexcoords/render.fs.glsl", 0 },
       { GL_NONE, NULL, 0 }
   };
   program = LoadShader(shaders);
   mv_location = glGetUniformLocation(program, "mv_matrix");
   proj_location = glGetUniformLocation(program, "proj_matrix");

   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);

}

void MyOpenGL::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    aspect = (float)w / (float)h;
    // Projection matrix
    projection.setToIdentity();
    projection.perspective(45.0f, aspect, 0.1f, 100.0f);
}

void MyOpenGL::paintGL()
{
    static const GLfloat bk_color[] = { 0.0f, 0.25f, 0.0f, 1.0f };
    static const GLfloat depth = 1.0f;
    glClearBufferfv(GL_COLOR, 0, bk_color);
    glClearBufferfv(GL_DEPTH, 0, &depth);

    // View matrix
    view.setToIdentity();
    view.lookAt(QVector3D(0.0f, 0.0f, 5.0f), QVector3D(0.0f, 0.0f, 0.0f),
                QVector3D(0.0f, 1.0f, 0.0f));
    // Model matrix
    model.setToIdentity();
    model.translate(xtrans, ytrans, ztrans);
    model.rotate(rotation);
    // Model-view matrix
    QMatrix4x4 mv = view * model;

    glUniformMatrix4fv(mv_location, 1, GL_FALSE, mv.constData());
    glUniformMatrix4fv(proj_location, 1, GL_FALSE, projection.constData());


    glBindTexture(GL_TEXTURE_2D, texture[tex_index]);
	glUseProgram(program);

    obj.render();

}

void MyOpenGL::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        mousePos = QVector2D(event->pos()); // Get mouse position when pressed
    else
        QWidget::mousePressEvent(event);
}

void MyOpenGL::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        QVector2D newPos = QVector2D(event->pos());
        QVector2D diff = newPos - mousePos;
        double angle = (diff.length()) / 3.6;
        // Rotation axis is perpendicular to the mouse position difference vector
        QVector3D rotationAxis = QVector3D(diff.y(), diff.x(), 0.0).normalized();
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angle) * rotation;
        mousePos = newPos;
        this->update();
    }
    else QWidget::mouseMoveEvent(event);
}

void MyOpenGL::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;
    if (numDegrees.y() > 0)
        ztrans += 0.25f;
    else if (numDegrees.y() < 0)
        ztrans -= 0.25f;
    else
        QWidget::wheelEvent(event);
    this->update();
}

void MyOpenGL::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_T) {
        ++tex_index;
        if (tex_index > 1)
            tex_index = 0;
    }
    else
        QWidget::keyPressEvent(event);
    this->update();
}
