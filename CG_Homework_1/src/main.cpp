/*
 * 姓名：郭梓煜
 * 文件说明：main函数
 *         调用展示场景并将windowtitle置为Homework 1
*/
#include "myglwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyGLWidget w;
    w.setWindowTitle("Homework 1");
    w.show();
    return a.exec();
}
