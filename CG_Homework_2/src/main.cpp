/*###################################################
##  文件: main.cpp
##  文件说明： main函数
##           设置窗口title，size等
#####################################################*/
#include "myglwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyGLWidget w;
    w.setWindowTitle("Homework 2");
    w.resize(1080, 768);//设置初始窗口大小
    w.show();
    return a.exec();
}
