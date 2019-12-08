/*###################################################
##  文件: myglwidget.h
##  文件说明： 声明两个类，加载头文件
##  类： MyGLWidget，ObjLoader
#####################################################*/
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
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <istream>
#include <fstream>
#include <sstream>
using namespace std;
class MyGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL();//初始化
    void paintGL();//绘制图
    void resizeGL(int width, int height);//当窗口大小改变时调整视窗尺寸

private:
    QTimer *timer;//定时器
};

class ObjLoader
{
public:
    ObjLoader(string filename);//构造函数
    void Draw();//绘制函数
private:
    vector<vector<GLfloat>>vSets;//存放顶点(x,y,z)坐标
    vector<vector<GLint>>fSets;//存放面的三个顶点索引
};

#endif // MYGLWIDGET_H
