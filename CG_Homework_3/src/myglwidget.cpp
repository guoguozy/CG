#include "myglwidget.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace  std;
float angle = 0;//全局变量，控制小球旋转角度
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
    vertex_shader_source = get_source("E:\\code\\Qt\\CGTemplate3\\vertexshadersource.vert");
    fragment_shader_source = get_source("E:\\code\\Qt\\CGTemplate3\\fragmentshadersource.frag");

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
    initializeOpenGLFunctions();
    glViewport(0, 0, width(), height());
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

}

/*###################################################
##  函数: resizeGL
##  函数描述： 当窗口大小改变时调整视窗尺寸
##  参数描述： 无
#####################################################*/
void MyGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    update();
}

/*###################################################
##  函数: get_program
##  函数描述： 返回program值
##  参数描述： 无
#####################################################*/
GLuint MyGLWidget::get_program()
{
    return this->program;
}


/*###################################################
##  函数: get_source
##  函数描述： 返回用string表示的source文件
##  参数描述： 无
#####################################################*/
string MyGLWidget::get_source(const char* filename)
{
    ifstream infile;
    infile.open(filename, ios::in);
    if(!infile.is_open())
    {
        cout << filename << endl;
        cout << "Can not find this file" << endl;
        return "";
    }
    string source;
    while(!infile.eof())
    {
        string line;
        getline(infile, line);
        source += line;
        source += '\n';
    }
    source += '\0';
    return source;
}


/*###################################################
##  函数: compile
##  函数描述： 着色器编译，返回program即着色器程序id
##  参数描述： 无
#####################################################*/
GLuint MyGLWidget::compile()
{
    const char*  vsource = vertex_shader_source.c_str();
    const char* fsource = fragment_shader_source.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建顶点着色器程序容器
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//创建片段着色器程序容器

    glShaderSource(vertexShader, 1, &vsource, 0);//将顶点着色程序的源代码字符数组绑定到顶点着色器对象
    glShaderSource(fragmentShader, 1, &fsource, 0);//将片段着色程序的源代码字符数组绑定到片段着色器对象

    glCompileShader(vertexShader);//编译顶点着色器对象
    glCompileShader(fragmentShader);//编译片段着色器对象
    program = glCreateProgram();//创建一个（着色）程序对象；
    glAttachShader(program, vertexShader);//将顶点着色器对象附加到（着色）程序对象上
    glAttachShader(program, fragmentShader);//将片段着色器对象附加到（着色）程序对象上
    glLinkProgram(program);//对（着色）程序对象执行链接操作

    return program;
}


/*###################################################
##  函数: paintGL
##  函数描述： 绘图函数，实现图形绘制，会被update()函数调用
##  参数描述： 无
#####################################################*/


void MyGLWidget::paintGL()
{
    // Your Implementation
    angle += 1;
    if(angle >= 360)angle = 0;
    program = this->compile();

    glEnable(GL_PROGRAM_POINT_SIZE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);                            //切换到投影矩阵堆栈
    glLoadIdentity();
    glOrtho(-2.0f, 2.0f, -2.0f, 2.0f, -4.0f, 4.0f);   //设置正交投影视体
    glm::mat4 projection_matrix = glm::ortho(-2.0, 2.0, -2.0, 2.0, -4.0, 4.0);

    glMatrixMode(GL_MODELVIEW);                             //切换MODEL_VIEW矩阵堆栈
    glLoadIdentity();

    glUseProgram(program);//将OpenGL渲染管道切换到着色器模式，并使用刚才做好的（着色）程序对象

    glUniform1f(glGetUniformLocation(program, "radius"), 600.0f);//半径
    glUniform3f(glGetUniformLocation(program, "eye_position"), 0, 0, 4);
    glUniform3f(glGetUniformLocation(program, "light_position"), 0, - 2, 4);
    glUniform1f(glGetUniformLocation(program, "a"), 100); //高光系数

    float wind_r = 1.5;

    glTranslatef(wind_r * cos(glm::radians(angle)), 0.0, wind_r * sin(glm::radians(angle)));
    glm::mat4 model_matrix = glm::mat4(1.0);
    model_matrix = glm::translate(model_matrix, glm::vec3(wind_r * cos(glm::radians(angle)), 0.0, wind_r * sin(glm::radians(angle))));

    glm::mat4 view_matrix = glm::mat4(1.0);//单位矩阵

    GLuint model_loc = glGetUniformLocation(program, "model");
    GLuint view_loc = glGetUniformLocation(program, "view");
    GLuint projection_loc = glGetUniformLocation(program, "projection");

    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));

    glEnable(GL_POINT_SPRITE_ARB);//点精灵
    glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_NV);

    unsigned int VBO;
    GLfloat point_vbo[3] = {-0.0f, -0.0f, 0.0f};//点坐标

    glGenBuffers(1, &VBO);//设置1为缓冲ID，生成一个VBO对象

    glBindBuffer(GL_ARRAY_BUFFER, VBO);//顶点缓冲对象缓冲类型，把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3,//把用户定义的顶点数据复制到缓冲的内存中（当前绑定缓冲）
                 point_vbo, GL_STATIC_DRAW);//第四个参数：数据绘制的改变情况


    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE,
                          sizeof(float), 0);//告诉OpenGL如何解析顶点数据（应用到逐个顶点上）
    //第一个参数对应顶点着色器position顶点属性的位置值（location）
    glDrawArrays(GL_POINTS, 0, 1);//绘制点


}
