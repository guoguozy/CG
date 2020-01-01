#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <string>
#include <iostream>
using namespace std;

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    string get_source(const char* filename);//读取文件，获取source
    GLuint compile();//编译着色器
    GLuint get_program();//获取着色器id，program

private:
    QTimer *timer;
    // 你可以将你的shader或者vbo声明为私有变量
    GLuint program;
    string vertex_shader_source;
    string fragment_shader_source;

};
#endif // MYGLWIDGET_H
