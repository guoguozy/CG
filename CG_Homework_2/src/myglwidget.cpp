/*###################################################
##  文件: myglwidget.cpp
##  文件说明： 实现myglwidget.h中两个类的函数以及对图中物体的绘制
##  包括函数： （按顺序排列）
##   MyGLWidget::MyGLWidget                    Line 66
##   MyGLWidget::~MyGLWidget()                 Line 79
##   void MyGLWidget::initializeGL()           Line 89
##   void MyGLWidget::resizeGL                 Line 111
##   void DrawBody()                           Line 126
##   void DrawShoulder()                       Line 168
##   void DrawHip()                            Line 210
##   void DrawArmA()                           Line 252
##   void DrawArmB()                           Line 296
##   void DrawLegB()                           Line 339
##   void DrawLegA()                           Line 383
##   void DrawHead()                           Line 427
##   void DrawNeck()                           Line 536
##   void Drawflyhorse()                       Line 580
##   void Draweagle()                          Line 596
##   void change_eagle()                       Line 612
##   void change_flyhorse()                    Line 650
##   void change_robot()                       Line 666
##   void draw_circle()                        Line 744
##   void MyGLWidget::paintGL()                Line 766
##   ObjLoader::ObjLoader(string filename)     Line 876
##   void ObjLoader::Draw()                    Line 938
#####################################################*/
#include "myglwidget.h"
#include <iostream>
#include <windows.h>
using namespace std;

/*#####################################################################
##  全局变量的声明
##  filename：obj文件存储路径
##  objModel：调用obj文件加载的实体
##  angleof: l代表left，a,b代表下，上腿或胳膊，arm，leg代表胳膊，腿
##  la,ra,ll,rl：前面的l,r代表左，右，后面的a,l代表arm，leg
##              所存数值代表对应胳膊或腿的摆动角度，将实时改变
##  high，up：high为肩头飞马的弹跳高度，up：1即跳起，0即落下
##  eagle_x,eagle_y,eagle_angle：环绕老鹰的运动坐标以及其旋转角度
##  qian,hou,zuo,you：记录老鹰的运动方向，1为按该方向前进，0在该方向不变
##  a,b,c,angle,angle1,angle2：存储角度变化数值，用于使胳膊，腿变化角度
##                       可直接改动来改变机器人转动，以及胳膊，腿摆动快慢
#####################################################################*/
string filename_1 = "../obj/eagle.obj";
string filename_2 = "../obj/flyhorse.obj";//obj文件路径
ObjLoader objModel = ObjLoader(filename_1);
ObjLoader objModel1 = ObjLoader(filename_2);//载入obj文件，创建类实例
float angleoflarma, angleoflarmb, angleofrarma, angleofrarmb = 0;
float angleofllegb, angleofrlegb, angleofllega, angleofrlega = 0;//四肢躯干角度
float la, ra, ll, rl = 0;//存储中间变量角度
float high = -0.5, up = 1.0;//肩头飞马所用参数
float eagle_x = 3.0, eagle_y = -2.0, eagle_angle = 0.0; //老鹰所用参数
float qian = 1.0, hou = 0.0, zuo = 0.0, you = 0.0;
float a = 0.2, b = 0.1, c = 0.18, angle = 0.2;//机器人旋转所用参数
const float angle1 = 0.4, angle2 = 0.2;

/*###################################################
##  函数: MyGLWidget
##  函数描述： MyGLWidget类的构造函数，实例化定时器timer
##  参数描述：
##  parent: MyGLWidget的父对象
#####################################################*/

MyGLWidget::MyGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    timer = new QTimer(this); // 实例化一个定时器
    timer->start(16); // 时间间隔设置为16ms，可以根据需要调整
    connect(timer, SIGNAL(timeout()), this, SLOT(update())); // 连接update()函数，每16ms触发一次update()函数进行重新绘图
}

/*###################################################
##  函数: ~MyGLWidget
##  函数描述： ~MyGLWidget类的析构函数，删除timer
##  参数描述： 无
#####################################################*/
MyGLWidget::~MyGLWidget()
{
    delete this->timer;
}

/*###################################################
##  函数: initializeGL
##  函数描述： 初始化绘图参数，如视窗大小、背景色等
##  参数描述： 无
#####################################################*/
void MyGLWidget::initializeGL()
{
    glViewport(0, 0, width(), height());//设置视口
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);//设置背景颜色
    glClearDepth(1.0);//清除深度缓存时使用的深度值
    glDepthFunc(GL_LESS);//指定“目标像素与当前像素在z方向上值大小比较”的函数，符合输入的深度值小于参考值的目标像素才进行绘制，否则对目标像素不予绘制。
    glEnable(GL_DEPTH_TEST);//启动深度测试
    glShadeModel(GL_SMOOTH);//指明着色技术

    glMatrixMode(GL_PROJECTION);//切换为投影矩阵
    glLoadIdentity();//重置当前指定的矩阵为单位矩阵

    gluPerspective(45.0f, width() / height(), 0.1f, 100.0f);//指定了观察的视景体在世界坐标系中的具体大小

    glMatrixMode(GL_MODELVIEW);//对模型视景处操作
}

/*###################################################
##  函数: resizeGL
##  函数描述： 当窗口大小改变时调整视窗尺寸
##  参数描述： 无
#####################################################*/
void MyGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);//设置视口
    glMatrixMode(GL_PROJECTION);//切换为投影矩阵
    glLoadIdentity();//重置当前指定的矩阵为单位矩阵
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);//指定了观察的视景体在世界坐标系中的具体大小
    glMatrixMode(GL_MODELVIEW);//对模型视景处操作
    update();//调用update更新绘制
}

/*###################################################
##  函数: DrawBody
##  函数描述： 画机器人的身体
##  参数描述： 无
#####################################################*/
void DrawBody()
{
    glColor3f(0.87843f, 1.0f, 1.0f);//设置颜色为LightCyan
    glBegin(GL_QUADS);//绘制矩形
    //forward 四个点
    glVertex3f(-0.5f, 1.0f, 0.25f);
    glVertex3f(0.5f, 1.0f, 0.25f);
    glVertex3f(0.5f, -1.0f, 0.25f);
    glVertex3f(-0.5f, -1.0f, 0.25f);
    //left 四个点
    glVertex3f(0.5f, 1.0f, 0.25f);
    glVertex3f(0.5f, 1.0f, -0.25f);
    glVertex3f(0.5f, -1.0f, -0.25f);
    glVertex3f(0.5f, -1.0f, 0.25f);
    //back 四个点
    glVertex3f(0.5f, 1.0f, -0.25f);
    glVertex3f(-0.5f, 1.0f, -0.25f);
    glVertex3f(-0.5f, -1.0f, -0.25f);
    glVertex3f(0.5f, -1.0f, -0.25f);
    //right 四个点
    glVertex3f(-0.5f, 1.0f, 0.25f);
    glVertex3f(-0.5f, 1.0f, -0.25f);
    glVertex3f(-0.5f, -1.0f, -0.25f);
    glVertex3f(-0.5f, -1.0f, 0.25f);
    //top 四个点
    glVertex3f(0.5f, 1.0f, 0.25f);
    glVertex3f(0.5f, 1.0f, -0.25f);
    glVertex3f(-0.5f, 1.0f, -0.25f);
    glVertex3f(-0.5f, 1.0f, 0.25f);
    //bottom 四个点
    glVertex3f(0.5f, -1.0f, 0.25f);
    glVertex3f(0.5f, -1.0f, -0.25f);
    glVertex3f(-0.5f, -1.0f, -0.25f);
    glVertex3f(-0.5f, -1.0f, 0.25f);
    glEnd();//结束绘制
}

/*###################################################
##  函数: DrawShoulder
##  函数描述： 画机器人的肩膀
##  参数描述： 无
#####################################################*/
void DrawShoulder()
{
    glColor3f(1.0f, 0.93725f, 0.85882f);//设置颜色为AntiqueWhite1
    glBegin(GL_QUADS);//绘制矩形
    //forward 四个点
    glVertex3f(-1.0f, 0.2f, 0.25f);
    glVertex3f(1.0f, 0.2f, 0.25f);
    glVertex3f(1.0f, -0.2f, 0.25f);
    glVertex3f(-1.0f, -0.2f, 0.25f);
    //left 四个点
    glVertex3f(1.0f, 0.2f, 0.25f);
    glVertex3f(1.0f, 0.2f, -0.25f);
    glVertex3f(1.0f, -0.2f, -0.25f);
    glVertex3f(1.0f, -0.2f, 0.25f);
    //back 四个点
    glVertex3f(1.0f, 0.2f, -0.25f);
    glVertex3f(-1.0f, 0.2f, -0.25f);
    glVertex3f(-1.0f, -0.2f, -0.25f);
    glVertex3f(1.0f, -0.2f, -0.25f);
    //right 四个点
    glVertex3f(-1.0f, 0.2f, 0.25f);
    glVertex3f(-1.0f, 0.2f, -0.25f);
    glVertex3f(-1.0f, -0.2f, -0.25f);
    glVertex3f(-1.0f, -0.2f, 0.25f);
    //top 四个点
    glVertex3f(1.0f, 0.2f, 0.25f);
    glVertex3f(1.0f, 0.2f, -0.25f);
    glVertex3f(-1.0f, 0.2f, -0.25f);
    glVertex3f(-1.0f, 0.2f, 0.25f);
    //bottom 四个点
    glVertex3f(1.0f, -0.2f, 0.25f);
    glVertex3f(1.0f, -0.2f, -0.25f);
    glVertex3f(-1.0f, -0.2f, -0.25f);
    glVertex3f(-1.0f, -0.2f, 0.25f);
    glEnd();//结束绘制
}

/*###################################################
##  函数: DrawHip
##  函数描述： 画机器人的跨部
##  参数描述： 无
#####################################################*/
void DrawHip()
{
    glColor3f(1.0f, 0.93725f, 0.85882f);//设置颜色为AntiqueWhite1
    glBegin(GL_QUADS);//绘制矩形
    //forward 四个点
    glVertex3f(-0.7f, 0.2f, 0.25f);
    glVertex3f(0.7f, 0.2f, 0.25f);
    glVertex3f(0.7f, -0.2f, 0.25f);
    glVertex3f(-0.7f, -0.2f, 0.25f);
    //left 四个点
    glVertex3f(0.7f, 0.2f, 0.25f);
    glVertex3f(0.7f, 0.2f, -0.25f);
    glVertex3f(0.7f, -0.2f, -0.25f);
    glVertex3f(0.7f, -0.2f, 0.25f);
    //back 四个点
    glVertex3f(0.7f, 0.2f, -0.25f);
    glVertex3f(-0.7f, 0.2f, -0.25f);
    glVertex3f(-0.7f, -0.2f, -0.25f);
    glVertex3f(0.7f, -0.2f, -0.25f);
    //right 四个点
    glVertex3f(-0.7f, 0.2f, 0.25f);
    glVertex3f(-0.7f, 0.2f, -0.25f);
    glVertex3f(-0.7f, -0.2f, -0.25f);
    glVertex3f(-0.7f, -0.2f, 0.25f);
    //top 四个点
    glVertex3f(0.7f, 0.2f, 0.25f);
    glVertex3f(0.7f, 0.2f, -0.25f);
    glVertex3f(-0.7f, 0.2f, -0.25f);
    glVertex3f(-0.7f, 0.2f, 0.25f);
    //bottom 四个点
    glVertex3f(0.7f, -0.2f, 0.25f);
    glVertex3f(0.7f, -0.2f, -0.25f);
    glVertex3f(-0.7f, -0.2f, -0.25f);
    glVertex3f(-0.7f, -0.2f, 0.25f);
    glEnd();//结束绘制
}

/*###################################################
##  函数: DrawArmA
##  函数描述： 画机器人的胳膊(包括左右两边的下半截胳膊)
##  参数描述： 无
#####################################################*/
void DrawArmA()
{
    glColor3f(1.0f, 0.93725f, 0.85882f);//设置颜色为AntiqueWhite1
    glBegin(GL_QUADS);//绘制矩形

    //forward 四个点
    glVertex3f(-0.2f, 0.2f, 0.25f);
    glVertex3f(0.2f, 0.2f, 0.25f);
    glVertex3f(0.2f, -0.8f, 0.25f);
    glVertex3f(-0.2f, -0.8f, 0.25f);
    //left 四个点
    glVertex3f(0.2f, 0.2f, 0.25f);
    glVertex3f(0.2f, 0.2f, -0.25f);
    glVertex3f(0.2f, -0.8f, -0.25f);
    glVertex3f(0.2f, -0.8f, 0.25f);
    //back 四个点
    glVertex3f(0.2f, 0.2f, -0.25f);
    glVertex3f(-0.2f, 0.2f, -0.25f);
    glVertex3f(-0.2f, -0.8f, -0.25f);
    glVertex3f(0.2f, -0.8f, -0.25f);
    //right 四个点
    glVertex3f(-0.2f, 0.2f, 0.25f);
    glVertex3f(-0.2f, 0.2f, -0.25f);
    glVertex3f(-0.2f, -0.8f, -0.25f);
    glVertex3f(-0.2f, -0.8f, 0.25f);
    //top 四个点
    glVertex3f(0.2f, 0.2f, 0.25f);
    glVertex3f(0.2f, 0.2f, -0.25f);
    glVertex3f(-0.2f, 0.2f, -0.25f);
    glVertex3f(-0.2f, 0.2f, 0.25f);
    //bottom 四个点
    glVertex3f(0.2f, -0.2f, 0.25f);
    glVertex3f(0.2f, -0.2f, -0.25f);
    glVertex3f(-0.2f, -0.2f, -0.25f);
    glVertex3f(-0.2f, -0.2f, 0.25f);

    glEnd();//结束绘制
}

/*###################################################
##  函数: DrawArmB
##  函数描述： 画机器人的胳膊(包括左右两边的上半截胳膊)
##  参数描述： 无
#####################################################*/
void DrawArmB()
{
    glColor3f(0.87843f, 1.0f, 1.0f);//设置颜色为LightCyan
    glBegin(GL_QUADS);//绘制矩形

    //forward
    glVertex3f(-0.2f, 0.2f, 0.25f);
    glVertex3f(0.2f, 0.2f, 0.25f);
    glVertex3f(0.2f, -1.0f, 0.25f);
    glVertex3f(-0.2f, -1.0f, 0.25f);
    //left
    glVertex3f(0.2f, 0.2f, 0.25f);
    glVertex3f(0.2f, 0.2f, -0.25f);
    glVertex3f(0.2f, -1.0f, -0.25f);
    glVertex3f(0.2f, -1.0f, 0.25f);
    //back
    glVertex3f(0.2f, 0.2f, -0.25f);
    glVertex3f(-0.2f, 0.2f, -0.25f);
    glVertex3f(-0.2f, -1.0f, -0.25f);
    glVertex3f(0.2f, -1.0f, -0.25f);
    //right
    glVertex3f(-0.2f, 0.2f, 0.25f);
    glVertex3f(-0.2f, 0.2f, -0.25f);
    glVertex3f(-0.2f, -1.0f, -0.25f);
    glVertex3f(-0.2f, -1.0f, 0.25f);
    //top
    glVertex3f(0.2f, 0.2f, 0.25f);
    glVertex3f(0.2f, 0.2f, -0.25f);
    glVertex3f(-0.2f, 0.2f, -0.25f);
    glVertex3f(-0.2f, 0.2f, 0.25f);
    //bottom
    glVertex3f(-0.2f, -1.0f, 0.25f);
    glVertex3f(0.2f, -1.0f, 0.25f);
    glVertex3f(0.2f, -1.0f, -0.25f);
    glVertex3f(-0.2f, -1.0f, -0.25f);
    glEnd();//结束绘制
}

/*###################################################
##  函数: DrawLegB
##  函数描述： 画机器人的腿部(包括左右两边的上半截腿)
##  参数描述： 无
#####################################################*/
void DrawLegB()
{
    glColor3f(0.87843f, 1.0f, 1.0f);//设置颜色为LightCyan
    glBegin(GL_QUADS);//绘制矩形

    //forward
    glVertex3f(-0.3f, 0.25f, 0.255f);
    glVertex3f(0.3f, 0.25f, 0.255f);
    glVertex3f(0.3f, -1.25f, 0.255f);
    glVertex3f(-0.3f, -1.25f, 0.255f);
    //left
    glVertex3f(0.3f, 0.25f, 0.255f);
    glVertex3f(0.3f, 0.25f, -0.255f);
    glVertex3f(0.3f, -1.25f, -0.255f);
    glVertex3f(0.3f, -1.25f, 0.255f);
    //back
    glVertex3f(0.3f, 0.25f, -0.255f);
    glVertex3f(-0.3f, 0.25f, -0.255f);
    glVertex3f(-0.3f, -1.25f, -0.255f);
    glVertex3f(0.3f, -1.25f, -0.255f);
    //right
    glVertex3f(-0.3f, 0.25f, 0.255f);
    glVertex3f(-0.3f, 0.25f, -0.255f);
    glVertex3f(-0.3f, -1.25f, -0.255f);
    glVertex3f(-0.3f, -1.25f, 0.255f);
    //top
    glVertex3f(0.3f, 0.25f, 0.255f);
    glVertex3f(0.3f, 0.25f, -0.255f);
    glVertex3f(-0.3f, 0.25f, -0.255f);
    glVertex3f(-0.3f, 0.25f, 0.255f);
    //bottom
    glVertex3f(0.3f, -1.25f, 0.255f);
    glVertex3f(0.3f, -1.25f, -0.255f);
    glVertex3f(-0.3f, -1.25f, -0.255f);
    glVertex3f(-0.3f, -1.25f, 0.255f);

    glEnd();//结束绘制
}

/*###################################################
##  函数: DrawLegA
##  函数描述： 画机器人的腿部(包括左右两边的下半截腿)
##  参数描述： 无
#####################################################*/
void DrawLegA()
{
    glColor3f(1.0f, 0.93725f, 0.85882f);//设置颜色为AntiqueWhite1
    glBegin(GL_QUADS);//绘制矩形

    //forward
    glVertex3f(-0.3f, 0.25f, 0.255f);
    glVertex3f(0.3f, 0.25f, 0.255f);
    glVertex3f(0.3f, -1.0f, 0.255f);
    glVertex3f(-0.3f, -1.0f, 0.255f);
    //left
    glVertex3f(0.3f, 0.25f, 0.255f);
    glVertex3f(0.3f, 0.25f, -0.255f);
    glVertex3f(0.3f, -1.0f, -0.255f);
    glVertex3f(0.3f, -1.0f, 0.255f);
    //back
    glVertex3f(0.3f, 0.25f, -0.255f);
    glVertex3f(-0.3f, 0.25f, -0.255f);
    glVertex3f(-0.3f, -1.0f, -0.255f);
    glVertex3f(0.3f, -1.0f, -0.255f);
    //right
    glVertex3f(-0.3f, 0.25f, 0.255f);
    glVertex3f(-0.3f, 0.25f, -0.255f);
    glVertex3f(-0.3f, -1.0f, -0.255f);
    glVertex3f(-0.3f, -1.0f, 0.255f);
    //top
    glVertex3f(0.3f, 0.25f, 0.255f);
    glVertex3f(0.3f, 0.25f, -0.255f);
    glVertex3f(-0.3f, 0.25f, -0.255f);
    glVertex3f(-0.3f, 0.25f, 0.255f);
    //bottom
    glVertex3f(0.3f, -1.0f, 0.255f);
    glVertex3f(0.3f, -1.0f, -0.255f);
    glVertex3f(-0.3f, -1.0f, -0.255f);
    glVertex3f(-0.3f, -1.0f, 0.255f);

    glEnd();//结束绘制
}

/*###################################################
##  函数: DrawHead
##  函数描述： 画机器人的头部，包括头，眼睛，嘴巴，天线等
##  参数描述： 无
#####################################################*/
void DrawHead()
{
    glColor3f(1.0f, 0.93725f, 0.85882f);//设置颜色为AntiqueWhite1
    glBegin(GL_QUADS);//绘制矩形

    //forward
    glVertex3f(-0.5f, 0.5f, 0.35f);
    glVertex3f(0.5f, 0.5f, 0.35f);
    glVertex3f(0.5f, -0.5f, 0.35f);
    glVertex3f(-0.5f, -0.5f, 0.35f);
    //left
    glVertex3f(0.5f, 0.5f, 0.35f);
    glVertex3f(0.5f, 0.5f, -0.35f);
    glVertex3f(0.5f, -0.5f, -0.35f);
    glVertex3f(0.5f, -0.5f, 0.35f);
    //back
    glVertex3f(0.5f, 0.5f, -0.35f);
    glVertex3f(-0.5f, 0.5f, -0.35f);
    glVertex3f(-0.5f, -0.5f, -0.35f);
    glVertex3f(0.5f, -0.5f, -0.35f);
    //right
    glVertex3f(-0.5f, 0.5f, 0.35f);
    glVertex3f(-0.5f, 0.5f, -0.35f);
    glVertex3f(-0.5f, -0.5f, -0.35f);
    glVertex3f(-0.5f, -0.5f, 0.35f);
    //top
    glVertex3f(0.5f, 0.5f, 0.35f);
    glVertex3f(0.5f, 0.5f, -0.35f);
    glVertex3f(-0.5f, 0.5f, -0.35f);
    glVertex3f(-0.5f, 0.5f, 0.35f);
    //bottom
    glVertex3f(0.5f, -0.5f, 0.35f);
    glVertex3f(0.5f, -0.5f, -0.35f);
    glVertex3f(-0.5f, -0.5f, -0.35f);
    glVertex3f(-0.5f, -0.5f, 0.35f);
    glEnd();//结束绘制

    //绘制天线1
    glPushMatrix();//将矩阵压栈保存
    glTranslatef(0.2f, 0.5f, 0.0f);//将原点移至头顶偏右的位置
    glColor3f(0.87843f, 1.0f, 1.0f);//设置颜色为LightCyan
    float R = 0.02f, pi = 3.1415926536f;//赋值半径，pi值
    for (int i = 0; i < 100; i++)//100即绘制圆上100个点
    {
        glBegin(GL_LINE_STRIP);//绘制连线，将圆上的点与固定一点连线组成圆柱体
        glVertex3f(R * cos(2 * pi / 100 * i), 0.0f, R * sin(2 * pi / 100 * i));//圆上各点
        glVertex3f(0.2f, 0.5f, 0.0f);//天线顶点
        glEnd();//结束绘制
    }
    glPopMatrix();//矩阵出栈


    //绘制天线2
    glPushMatrix();//将矩阵压栈保存
    glTranslatef(-0.2f, 0.5f, 0.0f);//将原点移至头顶偏左的位置
    glColor3f(0.87843f, 1.0f, 1.0f);//设置颜色为LightCyan
    for (int i = 0; i < 100; i++)//100即绘制圆上100个点
    {
        glBegin(GL_LINE_STRIP);//绘制连线，将圆上的点与固定一点连线组成圆柱体
        glVertex3f(R * cos(2 * pi / 100 * i), 0.0f, R * sin(2 * pi / 100 * i));//圆上各点
        glVertex3f(-0.2f, 0.5f, 0.0f);//天线顶点
        glEnd();//结束绘制
    }
    glPopMatrix();//矩阵出栈


    //绘制眼睛1
    glPushMatrix();//将矩阵压栈保存
    glTranslatef(0.2f, 0.1f, 0.36f);//将原点移至前脸偏右的位置
    R = 0.1f;//赋值眼睛半径
    glBegin(GL_POLYGON);//绘制多边形，只要顶点够多就与圆相近
    glColor3f(0.72157f, 0.52549f, 0.04314f);//设置颜色为chocolate
    for (int i = 0; i < 100; i++)//100即绘制圆上100个点
    {
        glVertex2f(R * cos(2 * pi / 100 * i), R * sin(2 * pi / 100 * i));//圆上各点
    }
    glEnd();//结束绘制
    glPopMatrix();//矩阵出栈

    //绘制眼睛2
    glPushMatrix();//将矩阵压栈保存
    glTranslatef(-0.2f, 0.1f, 0.36f);//将原点移至前脸偏左的位置
    glBegin(GL_POLYGON);//绘制多边形，只要顶点够多就与圆相近
    glColor3f(0.72157f, 0.52549f, 0.04314f);//设置颜色为chocolate
    for (int i = 0; i < 100; i++)
    {
        glVertex2f(R * cos(2 * pi / 100 * i), R * sin(2 * pi / 100 * i));//圆上各点
    }
    glEnd();//结束绘制
    glPopMatrix();//矩阵出栈


    //绘制嘴巴
    glPushMatrix();//将矩阵压栈保存
    glTranslatef(0.0f, 0.0f, 0.36f);//将原点移至前脸的平面
    glBegin(GL_LINE_LOOP);//绘制连线
    glColor3f(0.72157f, 0.52549f, 0.04314f);//设置颜色为chocolate
    glVertex2f(-0.1f, -0.2f);//嘴巴的三个点
    glVertex2f(0.0f, -0.3f);
    glVertex2f(0.1f, -0.2f);
    glEnd();//结束绘制
    glPopMatrix();//矩阵出栈
}

/*###################################################
##  函数: DrawNeck
##  函数描述： 画机器人的脖子
##  参数描述： 无
#####################################################*/
void DrawNeck()
{
    glColor3f(0.87843f, 1.0f, 1.0f);//设置颜色为LightCyan
    glBegin(GL_QUADS);//绘制矩形

    //forward 四个点
    glVertex3f(-0.2f, 0.1f, 0.25f);
    glVertex3f(0.2f, 0.1f, 0.25f);
    glVertex3f(0.2f, -0.1f, 0.25f);
    glVertex3f(-0.2f, -0.1f, 0.25f);
    //left 四个点
    glVertex3f(0.2f, 0.1f, 0.25f);
    glVertex3f(0.2f, 0.1f, -0.25f);
    glVertex3f(0.2f, -0.1f, -0.25f);
    glVertex3f(0.2f, -0.1f, 0.25f);
    //back 四个点
    glVertex3f(0.2f, 0.1f, -0.25f);
    glVertex3f(-0.2f, 0.1f, -0.25f);
    glVertex3f(-0.2f, -0.1f, -0.25f);
    glVertex3f(0.2f, -0.1f, -0.25f);
    //right 四个点
    glVertex3f(-0.2f, 0.1f, 0.25f);
    glVertex3f(-0.2f, 0.1f, -0.25f);
    glVertex3f(-0.2f, -0.1f, -0.25f);
    glVertex3f(-0.2f, -0.1f, 0.25f);
    //top 四个点
    glVertex3f(0.2f, 0.1f, 0.25f);
    glVertex3f(0.2f, 0.1f, -0.25f);
    glVertex3f(-0.2f, 0.1f, -0.25f);
    glVertex3f(-0.2f, 0.1f, 0.25f);
    //bottom 四个点
    glVertex3f(0.2f, -0.1f, 0.25f);
    glVertex3f(0.2f, -0.1f, -0.25f);
    glVertex3f(-0.2f, -0.1f, -0.25f);
    glVertex3f(-0.2f, -0.1f, 0.25f);

    glEnd();//结束绘制
}

/*###################################################
##  函数: Drawflyhorse
##  函数描述： 画飞马
##  参数描述： 无
#####################################################*/
void Drawflyhorse()
{
    glPushMatrix();//将矩阵压栈保存
    glColor3f(1.0f, 0.84314f, 0.0f);//设置颜色为gold
    glTranslatef(-1, high, 0);//将原点移至机器人肩头
    glRotatef(270, 0, 1, 0);//将飞马旋转方向，面向前方
    glScalef(0.005, 0.005, 0.005);//缩放飞马至合适的大小
    objModel1.Draw();//调用类函数绘制飞马
    glPopMatrix();//矩阵出栈
}

/*###################################################
##  函数: Draweagle
##  函数描述： 画老鹰
##  参数描述： 无
#####################################################*/
void Draweagle()
{
    glPushMatrix();//将矩阵压栈保存
    glColor3f(0.0f, 1.0f, 1.0f);//设置颜色为青色
    glTranslatef(eagle_x, -1.0, eagle_y);//将老鹰坐标根据时间变换,通过改变eagle_x，eagle_y实现
    glRotatef(eagle_angle, 0, 1, 0);//将老鹰旋转方向，使其面向运动方向
    glScalef(0.3, 0.3, 0.3);//缩放老鹰至合适的大小
    objModel.Draw();//调用类函数绘制老鹰
    glPopMatrix();//矩阵出栈
}

/*###################################################
##  函数: change_eagle
##  函数描述： 改变老鹰的坐标以及角度，方向
##  参数描述： 无
#####################################################*/
void change_eagle()
{
    if(qian == 1.0)eagle_y += 0.05;//若现运动方向为前，则将y坐标缓增
    if(hou == 1.0)eagle_y -= 0.05;//若现运动方向为后，则将y坐标缓减
    if(zuo == 1.0)eagle_x -= 0.05;//若现运动方向为左，则将x坐标缓增
    if(you == 1.0)eagle_x += 0.05;//若现运动方向为右，则将y坐标缓减

    if(eagle_y >= 2)//若y坐标前进到一定位置，进行转向
    {
        zuo = 1.0;//由前进转为左转
        qian = 0.0;
        eagle_angle = 270;//改变老鹰运动朝向
    }
    if(eagle_x >= 3)//若x坐标前进到一定位置，进行转向
    {
        you = 0.0;//由右向转为前进
        qian = 1.0;
        eagle_angle = 0;//改变老鹰运动朝向
    }
    if(eagle_y <= -2)//若y坐标前进到一定位置，进行转向
    {
        you = 1.0;//由后走转为右转
        hou = 0.0;
        eagle_angle = 90;//改变老鹰运动朝向
    }
    if(eagle_x <= -2)//若x坐标前进到一定位置，进行转向
    {
        zuo = 0.0;//由左转改为后走
        hou = 1.0;
        eagle_angle = 180;//改变老鹰运动朝向
    }
}

/*###################################################
##  函数: change_flyhorse
##  函数描述： 改变飞马的坐标以及弹跳高度
##  参数描述： 无
#####################################################*/
void change_flyhorse()
{
    if(high >= 1.0)up = 0.0;//若跳至高度1，改运动方向为下
    if(high <= -0.6)up = 1.0;//若降至高度-0.6，改运动方向为上

    if(up == 1.0)//若向上运动，高度缓增
        high += 0.05;
    else//若向下运动，高度缓减
        high -= 0.05;
}

/*###################################################
##  函数: change_robot
##  函数描述： 改变机器人的运动方向及轨迹
##  参数描述： 无
#####################################################*/
void change_robot()
{
    //修改胳膊，腿的角度
    angleoflarma += la;//左下胳膊
    angleofrarma += ra;//右下胳膊
    angleoflarmb += a;//左上胳膊
    angleofrarmb -= a;//右下胳膊

    angleofllega += ll;//左下腿
    angleofrlega += rl;//右下腿
    angleofllegb -= b;//左上腿
    angleofrlegb += b;//右上腿
    angle += c;//机器人本身旋转角度

    //以一条胳膊的位置角度来推其他四肢的摆动角度

    //若左上胳膊在摆动范围0-50
    if (50 > angleoflarmb && angleoflarmb > 0 && a > 0)
    {
        //修改下次绘制摆动数值
        a = angle1;//赋值固定角度
        b = angle2;
        la = -angle1 * 0.75;//乘以一定比例使上下胳膊存在合适角度
        ra = -angle1;
        ll = 0.75 * angle2;//乘以一定比例使上下腿存在合适角度
        rl = angle2 * 1.5;
    }
    //若左上胳膊在摆动范围>=50
    if (angleoflarmb >= 50 && a > 0)
    {
        //修改摆动方向
        a = -angle1;
        b = -angle2;
    }
    //若左上胳膊在摆动范围>0
    if (angleoflarmb > 0 && a < 0)
    {
        //修改摆动方向
        a = -angle1;
        b = -angle2;
        la = angle1 * 0.75;//乘以一定比例使上下胳膊存在合适角度
        ra = angle1;
        ll = -angle2 * 0.75;//乘以一定比例使上下腿存在合适角度
        rl = -angle2 * 1.5;
    }
    //若左上胳膊在摆动范围-50-0
    if (-50 < angleoflarmb && angleoflarmb <= 0 && a < 0)
    {
        a = -angle1;
        b = -angle2;
        la = -angle1;
        ra = -angle1 * 0.75;//乘以一定比例使上下胳膊存在合适角度
        ll = angle2 * 1.5;//乘以一定比例使上下腿存在合适角度
        rl = angle2 * 0.75;
    }
    //若左上胳膊在摆动范围<=-50
    if (angleoflarmb <= -50 && a < 0)
    {
        a = angle1;//赋值旋转角度
        b = angle2;
    }
    //若左上胳膊在摆动范围<=0
    if (angleoflarmb < 0 && a > 0)
    {
        a = angle1;//赋值旋转角度
        b = angle2;
        la = angle1;
        ra = angle1 * 0.75;//乘以一定比例使上下胳膊存在合适角度
        ll = -angle2 * 1.5;//乘以一定比例使上下腿存在合适角度
        rl = -angle2 * 0.75;
    }
}

/*###################################################
##  函数: draw_circle
##  函数描述： 绘制机器人的行动路径
##  参数描述： 无
#####################################################*/
void draw_circle()
{
    glPushMatrix();//将矩阵压栈保存
    glTranslatef(0.0f, -1.0f, 25.5f);//将原点移至圆形路径中间
    glBegin(GL_LINE_LOOP);//绘制连线，只要顶点够多就与圆相近
    glColor3f(1.0f, 1.0f, 1.0f);//设置颜色为白色
    float R = 4.2f, pi = 3.1415926536f;
    for (int i = 0; i < 100; i++)
    {
        glVertex3f(R * cos(2 * pi / 100 * i), 0.0, R * sin(2 * pi / 100 * i)); //圆上各点
    }
    glEnd();//结束绘制
    glPopMatrix();//矩阵出栈
}

/*###################################################
##  函数: paintGL
##  函数描述： 绘图函数，实现图形绘制，会被update()函数调用
##           通过调用drawxxx函数和model.draw绘制机器人
##           同时按时间改变角度
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintGL()
{
    // Your Implementation
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓冲以及深度缓冲
    glLoadIdentity();//重置当前指定的矩阵为单位矩阵
    glTranslatef(0.0f, 0.0f, -35.0f);//移动原点

    draw_circle();//绘制圆形路径<--------

    glRotatef(angle, 0, 1, 0);//旋转坐标轴

    glPushMatrix();//将矩阵压栈保存
    glTranslatef(0.0f, 0.0f, -15.0f);//移动原点
    //glTranslatef(25.0, 0.0, 0.0);
    glRotatef(-90, 0, 1, 0);
    glPushMatrix();//将矩阵压栈保存
    glTranslatef(0.0f, 1.0, 0.0f);//移动原点
    glPushMatrix();//将矩阵压栈保存
    glTranslatef(0.8, 0.0, 0.0);//移动原点
    glTranslatef(0.0, -0.4, 0.0);
    glRotatef(angleoflarmb, 1, 0, 0);//旋转胳膊
    glPushMatrix();//将矩阵压栈保存
    glTranslatef(0.0, -0.8, 0.0);//移动原点
    glTranslatef(0.0, -0.3, 0.0);
    glRotatef(angleoflarma, 1, 0, 0);//旋转胳膊
    DrawArmA();//绘制下半胳膊
    glPopMatrix();//矩阵出栈
    DrawArmB();//绘制上半胳膊
    glPopMatrix();//矩阵出栈

    glPushMatrix();//将矩阵压栈保存
    glTranslatef(-0.8, 0.0, 0.0);//移动原点
    glTranslatef(0.0, -0.4, 0.0);
    glRotatef(angleofrarmb, 1, 0, 0);//旋转胳膊
    glPushMatrix();//将矩阵压栈保存
    glTranslatef(0.0, -0.8, 0.0);//移动原点
    glTranslatef(0.0, -0.3, 0.0);
    glRotatef(angleofrarma, 1, 0, 0);//旋转胳膊
    DrawArmA();//绘制下半胳膊
    glPopMatrix();//矩阵出栈
    DrawArmB();//绘制上半胳膊
    glPopMatrix();//矩阵出栈

    //回到 shoulder
    glPushMatrix();//将矩阵压栈保存
    glTranslatef(0.0, 0.3, 0.0);
    glPushMatrix();//将矩阵压栈保存
    glTranslatef(0.0, 0.6, 0.0);
    DrawHead();
    glPopMatrix();//矩阵出栈
    DrawNeck();//绘制脖子

    Drawflyhorse();//绘制飞马<--------

    glPopMatrix();//矩阵出栈
    DrawShoulder();
    glPopMatrix();//矩阵出栈
    //回到 body
    glPushMatrix();//将矩阵压栈保存
    glTranslatef(0.0f, -1.0f, 0.0f);//移动原点
    glPushMatrix();//将矩阵压栈保存
    glTranslatef(0.4, 0.0, 0.0);//移动原点
    glTranslatef(0.0, -0.4, 0.0);
    glRotatef(angleofllegb, 1, 0, 0);//旋转腿
    glPushMatrix();//将矩阵压栈保存
    glTranslatef(0.0, -1.5, 0.0);//移动原点
    glRotatef(angleofllega, 1, 0, 0);//旋转腿
    DrawLegA();//绘制上半腿
    glPopMatrix();//矩阵出栈
    DrawLegB();//绘制下半腿

    glPopMatrix();//矩阵出栈

    glPushMatrix();//将矩阵压栈保存
    glTranslatef(-0.4, 0.0, 0.0);//移动原点
    glTranslatef(0.0, -0.4, 0.0);
    glRotatef(angleofrlegb, 1, 0, 0);//旋转腿
    glPushMatrix();//将矩阵压栈保存
    glTranslatef(0.0, -1.5, 0.0);//移动原点
    glRotatef(angleofrlega, 1, 0, 0);//旋转腿
    DrawLegA();//绘制下半腿
    glPopMatrix();//矩阵出栈
    DrawLegB();//绘制上半腿

    Draweagle();//绘制老鹰<--------

    glPopMatrix();//矩阵出栈
    DrawHip();//绘制跨部
    glPopMatrix();//矩阵出栈
    DrawBody();//绘制身体
    glPopMatrix();//矩阵出栈
    glPopMatrix();//矩阵出栈
    glPopMatrix();//矩阵出栈

    //修改飞马位置
    change_flyhorse();
    //修改老鹰位置
    change_eagle();
    //修改机器人位置
    change_robot();

    glFlush();//清空缓冲区，将指令送往缓硬件立即执行
}

/*###################################################
##  函数: ObjLoader::ObjLoader
##  函数描述： ObjLoader类的构建函数，加载路径下的obj文件
##           读入到全局变量的类实例中
##  参数描述： filename（string）obj文件的路径
#####################################################*/
ObjLoader::ObjLoader(string filename)
{
    //简单的读取obj文件
    std::ifstream file(filename);//读取文件流
    std::string line;
    while (getline(file, line))//获取每一行
    {
        if (line.substr(0, 2) == "vt")
        {
            //暂不实现
        }
        else if (line.substr(0, 2) == "vn")
        {
            //暂不实现
        }
        else if (line.substr(0, 1) == "v")//若该行存储顶点
        {
            vector<GLfloat> Point;//存储点的vector
            GLfloat x, y, z;
            std::istringstream s(line.substr(2));
            s >> x;
            s >> y;
            s >> z;//利用stream读取将点坐标读入到x,y,z
            Point.push_back(x);
            Point.push_back(y);
            Point.push_back(z);//将点坐标放入vector
            vSets.push_back(Point);//将每个point的vector存储在点集的vector中

        }
        else if (line.substr(0, 1) == "f")//若改行存储面信息
        {
            //观察obj文件查看其面信息的格式发现为‘f x/x/x x/x/x x/x/x’格式
            std::replace(line.begin(), line.end(), '/', ' ');//将'/'去掉，替换为空格
            vector<GLint> vIndexSets;//存储构成面的点标号
            GLint u, u1, u2, v, v1, v2, w, w1, w2;
            std::istringstream vtns(line.substr(2));
            vtns >> u >> u1 >> u2;
            vtns >> v >> v1 >> v2;
            vtns >> w >> w1 >> w2;//读取点序号，存储于u,v,w
            vIndexSets.push_back(u - 1);
            vIndexSets.push_back(v - 1);
            vIndexSets.push_back(w - 1);//将序号记入vector
            fSets.push_back(vIndexSets);//将vector记入fsets面集
        }
        else if (line.substr(0, 1) == "#")
        {
            //暂不实现
        }
        else
        {
            //暂不实现
        }
    }
    file.close();//关闭读取
}

/*###################################################
##  函数: ObjLoader::Draw
##  函数描述： ObjLoader类的功能函数
##        将加载到类实例中的obj文件在OpenGL中绘制出来
##  参数描述： 无
#####################################################*/
void ObjLoader::Draw()
{
    glBegin(GL_TRIANGLES);//开始绘制
    for (int i = 0; i < fSets.size(); i++)
    {
        GLfloat VN[3];
        //三个顶点
        GLfloat SV1[3];
        GLfloat SV2[3];
        GLfloat SV3[3];

        if ((fSets[i]).size() != 3)//若该面没有三个顶点
        {
            cout << "the fSetsets_Size is not correct" << endl;
        }
        else
        {
            GLint firstVertexIndex = (fSets[i])[0];//取出顶点索引
            GLint secondVertexIndex = (fSets[i])[1];
            GLint thirdVertexIndex = (fSets[i])[2];

            SV1[0] = (vSets[firstVertexIndex])[0];//第一个顶点
            SV1[1] = (vSets[firstVertexIndex])[1];
            SV1[2] = (vSets[firstVertexIndex])[2];

            SV2[0] = (vSets[secondVertexIndex])[0]; //第二个顶点
            SV2[1] = (vSets[secondVertexIndex])[1];
            SV2[2] = (vSets[secondVertexIndex])[2];

            SV3[0] = (vSets[thirdVertexIndex])[0]; //第三个顶点
            SV3[1] = (vSets[thirdVertexIndex])[1];
            SV3[2] = (vSets[thirdVertexIndex])[2];


            GLfloat vec1[3], vec2[3], vec3[3];//计算法向量
            //(x2-x1,y2-y1,z2-z1)
            vec1[0] = SV1[0] - SV2[0];
            vec1[1] = SV1[1] - SV2[1];
            vec1[2] = SV1[2] - SV2[2];

            //(x3-x2,y3-y2,z3-z2)
            vec2[0] = SV1[0] - SV3[0];
            vec2[1] = SV1[1] - SV3[1];
            vec2[2] = SV1[2] - SV3[2];

            //(x3-x1,y3-y1,z3-z1)
            vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
            vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
            vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

            GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));

            VN[0] = vec3[0] / D;
            VN[1] = vec3[1] / D;
            VN[2] = vec3[2] / D;

            glNormal3f(VN[0], VN[1], VN[2]);//绘制法向量

            glVertex3f(SV1[0], SV1[1], SV1[2]);//绘制三角面片
            glVertex3f(SV2[0], SV2[1], SV2[2]);
            glVertex3f(SV3[0], SV3[1], SV3[2]);
        }
    }
    glEnd();//结束绘制
}
