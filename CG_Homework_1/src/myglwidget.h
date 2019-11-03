/*
 * 姓名：郭梓煜
 * 文件说明：定义MyGLWidget类
 *         调用GL/glew.h等库
*/
#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class MyGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent *e);

    void my_Translate(GLfloat (*my_Matrix)[4], GLfloat x, GLfloat y, GLfloat z);//平移
    void my_Rotate(GLfloat (*my_Matrix)[4], GLfloat x, GLfloat y, GLfloat z, GLfloat angle);//旋转
    void my_GLlines_for_scene1(int x_1, int y_1, int x_2, int y_2);//画线
    void my_GLlines_for_scene2(int x_1, int y_1, int x_2, int y_2);//画线，抗锯齿
    GLfloat *my_change_point(GLfloat (*my_Matrix)[4], GLfloat x, GLfloat y, GLfloat z);//变换点坐标
    void my_MultMatrix(GLfloat (*matrix_1)[4], GLfloat (*matrix_2)[4]);//矩阵相乘
    void my_quaternion_Rotate(GLfloat (*my_Matrix)[4], GLfloat a, GLfloat b, GLfloat c, GLfloat angle);//四元数旋转

private:
    int scene_id;
    void scene_0();
    void scene_1();
    void scene_2();//展示四元数旋转，抗锯齿效果
};
#endif // MYGLWIDGET_H
