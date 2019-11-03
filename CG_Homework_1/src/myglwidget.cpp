/*
 * 姓名：郭梓煜
 * 文件说明：实现myglwidget.h中MyGLWidget类的功能函数
 *         并实现展示场景scene_0,scene_1,scene_2
 *         同时，三个scene可由按键0,1,2调用切换
*/


#include "myglwidget.h"
MyGLWidget::MyGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      scene_id(0)
{
}

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::initializeGL()
{
    glViewport(0, 0, width(), height());
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);
}

void MyGLWidget::paintGL()
{
    if (scene_id == 0)
    {
        scene_0();
    }
    else if(scene_id == 1)
    {
        scene_1();
    }
    else
    {
        scene_2();
    }
}

void MyGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    update();
}

void MyGLWidget::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_0)
    {
        scene_id = 0;
        update();
    }
    else if (e->key() == Qt::Key_1)
    {
        scene_id = 1;
        update();
    }
    else
    {
        scene_id = 2;
        update();
    }
}

void MyGLWidget::scene_0()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 100.0f, 0.0f, 100.0f, -1000.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(50.0f, 50.0f, 0.0f);

    //draw a triangle
    glPushMatrix();
    glColor3f(0.839f, 0.153f, 0.157f);

    glTranslatef(-20.0f, -10.0f, 0.0f);
    glRotatef(45.0f, 1.0f, 0.0f, 1.0f);
    glTranslatef(-50.0f, -30.0f, 0.0f);

    glBegin(GL_LINE_LOOP);
    glVertex2f(10.0f, 10.0f);
    glVertex2f(50.0f, 50.0f);
    glVertex2f(80.0f, 10.0f);
    glEnd();
    glPopMatrix();

    //draw a quad
    glPushMatrix();
    glColor3f(0.122f, 0.467f, 0.706f);
    glTranslatef(20.0f, 20.0f, 0.0f);
    glRotatef(30.0f, 1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-20.0f, -20.0f);
    glVertex2f(20.0f, -20.0f);
    glVertex2f(20.0f, 20.0f);
    glVertex2f(-20.0f, 20.0f);
    glEnd();
    glPopMatrix();
}

void MyGLWidget::scene_1()
{
    /*
     * 函数：scene_1
     * 函数描述：实现scene_0的画三角形以及矩形
     * 参数描述：无
     */
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width(), 0.0f, height(), -1000.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //my implementation
    //声明一个单位矩阵4*4
    GLfloat my_Matrix[4][4];
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            if(i == j)
                my_Matrix[i][j] = 1.0f;
            else
                my_Matrix[i][j] = 0.0f;
    //Translate and Rotate
    my_Translate(my_Matrix, 50.0f, 50.0f, 0.0f);
    glColor3f(0.839f, 0.153f, 0.157f);
    my_Translate(my_Matrix, -20.0f, -10.0f, 0.0f);
    //常规旋转
    my_Rotate(my_Matrix, 1.0f, 0.0f, 1.0f, 45.0f);
    //四元数旋转
    //my_quaternion_Rotate(my_Matrix, 1.0f, 0.0f, 1.0f, 45.0f);
    my_Translate(my_Matrix, -50.0f, -30.0f, 0.0f);
    //获取三角形各点
    GLfloat *a = my_change_point(my_Matrix, 10.0f, 10.0f, 0.0f);
    GLfloat *b = my_change_point(my_Matrix, 50.0f, 50.0f, 0.0f);
    GLfloat *c = my_change_point(my_Matrix, 80.0f, 10.0f, 0.0f);
    //连接各点画线
    my_GLlines_for_scene1(a[0], a[1], b[0], b[1]);
    my_GLlines_for_scene1(a[0], a[1], c[0], c[1]);
    my_GLlines_for_scene1(b[0], b[1], c[0], c[1]);

    //重新初始化单位矩阵
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            if(i == j)
                my_Matrix[i][j] = 1.0f;
            else
                my_Matrix[i][j] = 0.0f;
    //Translate and Rotate
    my_Translate(my_Matrix, 50.0f, 50.0f, 0.0f);
    glColor3f(0.122f, 0.467f, 0.706f);
    my_Translate(my_Matrix, 20.0f, 20.0f, 0.0f);
    //常规旋转
    my_Rotate(my_Matrix, 1.0f, 1.0f, 1.0f, 30.0f);
    //四元数旋转
    //my_quaternion_Rotate(my_Matrix, 1.0f, 0.0f, 1.0f, 30.0f);
    //获取矩形各点
    GLfloat *d = my_change_point(my_Matrix, -20.0f, -20.0f, 0.0f);
    GLfloat *e = my_change_point(my_Matrix, 20.0f, -20.0f, 0.0f);
    GLfloat *f = my_change_point(my_Matrix, 20.0f, 20.0f, 0.0f);
    GLfloat *g = my_change_point(my_Matrix, -20.0f, 20.0f, 0.0f);
    //连接各点画线
    my_GLlines_for_scene1(d[0], d[1], e[0], e[1]);
    my_GLlines_for_scene1(e[0], e[1], f[0], f[1]);
    my_GLlines_for_scene1(f[0], f[1], g[0], g[1]);
    my_GLlines_for_scene1(g[0], g[1], d[0], d[1]);
}

void MyGLWidget::scene_2()
{
    /*
     * 函数：scene_2
     * 函数描述：实现scene_0的画三角形以及矩形
     *          并且实现四元数旋转以及抗锯齿
     * 参数描述：无
     */
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width(), 0.0f, height(), -1000.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //my implementation
    //声明一个单位矩阵4*4
    GLfloat my_Matrix[4][4];
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            if(i == j)
                my_Matrix[i][j] = 1.0f;
            else
                my_Matrix[i][j] = 0.0f;
    //Translate and Rotate
    my_Translate(my_Matrix, 50.0f, 50.0f, 0.0f);
    glColor3f(0.839f, 0.153f, 0.157f);
    my_Translate(my_Matrix, -20.0f, -10.0f, 0.0f);
    //常规旋转
    //my_Rotate(my_Matrix, 1.0f, 0.0f, 1.0f, 45.0f);
    //四元数旋转
    my_quaternion_Rotate(my_Matrix, 1.0f, 0.0f, 1.0f, 45.0f);
    my_Translate(my_Matrix, -50.0f, -30.0f, 0.0f);
    //获取三角形各点
    GLfloat *a = my_change_point(my_Matrix, 10.0f, 10.0f, 0.0f);
    GLfloat *b = my_change_point(my_Matrix, 50.0f, 50.0f, 0.0f);
    GLfloat *c = my_change_point(my_Matrix, 80.0f, 10.0f, 0.0f);
    //连接各点画线
    my_GLlines_for_scene2(a[0], a[1], b[0], b[1]);
    my_GLlines_for_scene2(a[0], a[1], c[0], c[1]);
    my_GLlines_for_scene2(b[0], b[1], c[0], c[1]);

    //重新初始化单位矩阵
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            if(i == j)
                my_Matrix[i][j] = 1.0f;
            else
                my_Matrix[i][j] = 0.0f;
    //Translate and Rotate
    my_Translate(my_Matrix, 50.0f, 50.0f, 0.0f);
    glColor3f(0.122f, 0.467f, 0.706f);
    my_Translate(my_Matrix, 20.0f, 20.0f, 0.0f);
    //常规旋转
    //my_Rotate(my_Matrix, 1.0f, 1.0f, 1.0f, 30.0f);
    //四元数旋转
    my_quaternion_Rotate(my_Matrix, 1.0f, 1.0f, 1.0f, 30.0f);
    //获取矩形各点
    GLfloat *d = my_change_point(my_Matrix, -20.0f, -20.0f, 0.0f);
    GLfloat *e = my_change_point(my_Matrix, 20.0f, -20.0f, 0.0f);
    GLfloat *f = my_change_point(my_Matrix, 20.0f, 20.0f, 0.0f);
    GLfloat *g = my_change_point(my_Matrix, -20.0f, 20.0f, 0.0f);
    //连接各点画线
    my_GLlines_for_scene2(d[0], d[1], e[0], e[1]);
    my_GLlines_for_scene2(e[0], e[1], f[0], f[1]);
    my_GLlines_for_scene2(f[0], f[1], g[0], g[1]);
    my_GLlines_for_scene2(g[0], g[1], d[0], d[1]);
}

void MyGLWidget::my_GLlines_for_scene1(int x_1, int y_1, int x_2, int y_2)
{
    /*
     * 函数： my_GLlines_for_scene1
     * 函数描述：在(x_1,y_1),(x_2,y_2)两点间画一条线段
     *          采用整数点运算
     *          用于scene_1中
     * 参数描述：
     * x_1:第一个点的横坐标
     * y_1:第一个点的纵坐标
     * x_2:第二个点的横坐标
     * y_2:第二个点的纵坐标
     * 返回：void
     */

    //适应窗口大小
    x_1 *= width() / 100.0f;
    y_1 *= height() / 100.0f;
    x_2 *= width() / 100.0f;
    y_2 *= height() / 100.0f;
    //考虑两点重合与否
    int dx = abs(x_2 - x_1), dy = abs(y_2 - y_1);
    if(dx == 0 && dy == 0)
    {
        glBegin(GL_POINTS);
        glVertex2i(x_1, y_1);
        glEnd();
        glFlush();
        return;
    }
    //将斜率绝对值变换到[0,1]区间
    //翻转xy轴
    int flag = 0;
    if(dx < dy)
    {
        flag = 1;
        std::swap(x_1, y_1);
        std::swap(x_2, y_2);
        std::swap(dx, dy);
    }
    //考虑两点位置关系
    int tmp_x = (x_2 - x_1) > 0 ? 1 : -1;
    int tmp_y = (y_2 - y_1) > 0 ? 1 : -1;
    //记录当前x,y值
    int cur_x = x_1;
    int cur_y = y_1;
    //计算2*deta(y),2(deta(y)-deta(x)),2*deta(y)-deta(x)
    int ds = 2 * dy;
    int dt = 2 * (dy - dx);
    int p = 2 * dy - dx;
    //画点成线
    while(cur_x != x_2)
    {
        if(p < 0)
            p += ds;
        else
        {
            cur_y += tmp_y;
            p += dt;
        }
        //抗锯齿
        //glEnable(GL_POINT_SMOOTH);
        //glHint(GL_POINT_SMOOTH, GL_NICEST);
        glBegin(GL_POINTS);
        //画点
        if(flag)
            glVertex2i(cur_y, cur_x);
        else
            glVertex2i(cur_x, cur_y);
        glEnd();
        glFlush();
        glDisable(GL_POINT_SMOOTH);
        cur_x += tmp_x;
    }
}

void MyGLWidget::my_GLlines_for_scene2(int x_1, int y_1, int x_2, int y_2)
{
    /*
     * 函数： my_GLlines_for_scene2
     * 函数描述：在(x_1,y_1),(x_2,y_2)两点间画一条线段
     *          采用整数点运算
     *          用于scene_2中
     *          抗锯齿处理
     * 参数描述：
     * x_1:第一个点的横坐标
     * y_1:第一个点的纵坐标
     * x_2:第二个点的横坐标
     * y_2:第二个点的纵坐标
     * 返回：void
     */

    //适应窗口大小
    x_1 *= width() / 100.0f;
    y_1 *= height() / 100.0f;
    x_2 *= width() / 100.0f;
    y_2 *= height() / 100.0f;
    //考虑两点重合与否
    int dx = abs(x_2 - x_1), dy = abs(y_2 - y_1);
    if(dx == 0 && dy == 0)
    {
        glBegin(GL_POINTS);
        glVertex2i(x_1, y_1);
        glEnd();
        glFlush();
        return;
    }
    //将斜率绝对值变换到[0,1]区间
    //翻转xy轴
    int flag = 0;
    if(dx < dy)
    {
        flag = 1;
        std::swap(x_1, y_1);
        std::swap(x_2, y_2);
        std::swap(dx, dy);
    }
    //考虑两点位置关系
    int tmp_x = (x_2 - x_1) > 0 ? 1 : -1;
    int tmp_y = (y_2 - y_1) > 0 ? 1 : -1;
    //记录当前x,y值
    int cur_x = x_1;
    int cur_y = y_1;
    //计算2*deta(y),2(deta(y)-deta(x)),2*deta(y)-deta(x)
    int ds = 2 * dy;
    int dt = 2 * (dy - dx);
    int p = 2 * dy - dx;
    //画点成线
    while(cur_x != x_2)
    {
        if(p < 0)
            p += ds;
        else
        {
            cur_y += tmp_y;
            p += dt;
        }
        //抗锯齿
        glEnable(GL_POINT_SMOOTH);
        glHint(GL_POINT_SMOOTH, GL_NICEST);
        glBegin(GL_POINTS);
        //画点
        if(flag)
            glVertex2i(cur_y, cur_x);
        else
            glVertex2i(cur_x, cur_y);
        glEnd();
        glFlush();
        glDisable(GL_POINT_SMOOTH);
        cur_x += tmp_x;
    }
}

void MyGLWidget::my_MultMatrix(GLfloat (*matrix_1)[4], GLfloat (*matrix_2)[4])
{
    /*
     * 函数:  my_MultMatrix
     * 函数描述: 实现对两个4*4矩阵的乘法运算
     *          并把结果存在第一个矩阵中
     * 参数描述:
     * matrix_1:第一个矩阵，以二维数组表示
     *          同时也是结果矩阵
     * matrix_2:第二个矩阵，以二维数组表示
     * 返回：void
     */

    //初始化一个0矩阵，用于充当中间变量存储结果
    GLfloat tmp[4][4] =
    {
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f}
    };
    //计算结果存储于中间矩阵
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            for(int k = 0; k < 4; ++k)
                tmp[i][j] += matrix_1[i][k] * matrix_2[k][j];
    //将tmp矩阵中值赋予matrix_1
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            matrix_1[i][j] = tmp[i][j];
}

void MyGLWidget::my_Translate(GLfloat (*my_Matrix)[4], GLfloat x, GLfloat y, GLfloat z)
{
    /*
     * 函数:  my_Translate
     * 函数描述：通过构造平移矩阵与变换矩阵相乘得到平移后的旋转矩阵
     * 参数描述：
     * my_Matrix:变换矩阵
     * x:x轴上的平移单位数
     * y:y轴上的平移单位数
     * z:z轴上的平移单位数
     * 返回：void
     */

    //构造平移矩阵
    GLfloat tran_Matrix[4][4] =
    {
        {1.0f, 0.0f, 0.0f, x},
        {0.0f, 1.0f, 0.0f, y},
        {0.0f, 0.0f, 1.0f, z},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };
    my_MultMatrix(my_Matrix, tran_Matrix);
}

void MyGLWidget::my_Rotate(GLfloat (*my_Matrix)[4], GLfloat x, GLfloat y, GLfloat z, GLfloat angle)
{
    /*
     * 函数：my_Rotate
     * 函数描述：通过单位化轴向量以及构造旋转矩阵
     *         再与原变换矩阵相乘得到旋转后的变换矩阵
     * 参数描述：
     * my_Matrix:变换矩阵
     * x:旋转轴向量的x轴坐标
     * y:旋转轴向量的y轴坐标
     * z:旋转轴向量的z轴坐标
     * angle:旋转的角度,单位为度
     * 返回：void
     */
    //首先单位化旋转轴
    GLfloat denominator = sqrtf(x * x + y * y + z * z);
    x /= denominator;
    y /= denominator;
    z /= denominator;
    //转换角度单位
    double k = -angle * M_PI / 180.0f;
    //构造旋转矩阵
    GLfloat rotate_Matrix[4][4] =
    {
        {static_cast<GLfloat>(x*x * (1 - cos(k)) + cos(k)), static_cast<GLfloat>(x*y * (1 - cos(k)) + z * sin(k)), static_cast<GLfloat>(x*z * (1 - cos(k)) - y * sin(k)), 0.0f},
        {static_cast<GLfloat>(x*y * (1 - cos(k)) - z * sin(k)), static_cast<GLfloat>(y*y * (1 - cos(k)) + cos(k)), static_cast<GLfloat>(y*z * (1 - cos(k)) + y * sin(k)), 0.0f},
        {static_cast<GLfloat>(x*z * (1 - cos(k)) + y * sin(k)), static_cast<GLfloat>(z*y * (1 - cos(k)) - x * sin(k)), static_cast<GLfloat>(z*z * (1 - cos(k)) + cos(k)), 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };
    my_MultMatrix(my_Matrix, rotate_Matrix);
}

GLfloat* MyGLWidget::my_change_point(GLfloat (*my_Matrix)[4], GLfloat x, GLfloat y, GLfloat z)
{
    /*
     * 函数：my_change_point
     * 函数描述：将变换矩阵应用于各个点
     *         即将变换矩阵乘以点坐标
     *         得到点变换后的坐标
     * 参数描述：
     * my_Matrix:变换矩阵
     * x:点的x轴坐标
     * y:点的y轴坐标
     * z:点的z轴坐标
     * 返回：result_points
     *      即变换后的点坐标，以一维数组表示
     */

    //申请并初始化结果点坐标
    GLfloat *result_points = new GLfloat[4];
    for(int i = 0; i < 4; ++i)
        result_points[i] = 0.0f;
    //变换前点坐标
    GLfloat before_points[4] = {x, y, z, 1.0f};
    //变换
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            result_points[i] += (before_points[j] * my_Matrix[i][j]);
    return result_points;
}

void MyGLWidget::my_quaternion_Rotate(GLfloat (*my_Matrix)[4], GLfloat a, GLfloat b, GLfloat c, GLfloat angle)
{
    /*
     * 函数：my_quaternion_Rotate
     * 函数描述：通过单位化旋转轴向量以及构造并计算四元数
     *         得到新的变换矩阵
     * 参数描述：
     * my_Matrix:变换矩阵
     * a:旋转轴向量的x轴坐标
     * b:旋转轴向量的y轴坐标
     * c:旋转轴向量的z轴坐标
     * angle:旋转的角度,单位为度
     */

    //首先单位化旋转轴
    GLfloat denominator = sqrtf(a * a + b * b + c * c);
    a /= denominator;
    b /= denominator;
    c /= denominator;
    //将角度除以2并转换单位
    double k = angle * M_PI / 360.0f;
    //计算Quaternion的x,y,z,w
    GLfloat x = static_cast<GLfloat>(a * sin(k));
    GLfloat y = static_cast<GLfloat>(b * sin(k));
    GLfloat z = static_cast<GLfloat>(c * sin(k));
    GLfloat w = static_cast<GLfloat>(cos(k));
    //利用四元数构造矩阵
    GLfloat rotate_Matrix[4][4] =
    {
        {1 - 2 * y * y - 2 * z * z, 2 * (x * y - w * z), 2 * (x * z + w * y), 0.0f},
        {2 * (x * y + w * z), 1 - 2 * x * x - 2 * z * z, 2 * (y * z - w * x), 0.0f},
        {2 * (x * z - w * y), 2 * (y * z + w * x), 1 - 2 * x *x - 2 * y * y, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    };
    my_MultMatrix(my_Matrix, rotate_Matrix);
}


