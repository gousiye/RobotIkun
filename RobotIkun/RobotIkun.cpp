#include "Robot.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <gl/glut.h>   
#include <cmath> 
#include <iostream>

#define PI 3.14159

int w;
int h;

float xView = 0;  //视线
float yView = 0;  //视线
float zView = 10; //视线

// 生成一个机器人
Robot *robot = new Robot(0, 0, 0);

GLfloat vertices[][3] = { {1.0,-1.0,0},{1.0,1.0,0},{-1.0,1.0,0},{-1.0,-1.0,0} };
GLfloat colors[][3] = { {1.0,0.0,0.0},{0.0,1.0,1.0},{1.0,1.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},{1.0,0.0,1.0},{0.0,1.0,1.0},{1.0,1.0,1.0} };
int font = (int)GLUT_BITMAP_8_BY_13;


//光照  使用光源
GLfloat lightPos[] = { 1.0f, 0.0f, -1.0f, 0.0f };
GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, -1.0f };//反射光
GLfloat  specref[] = { 0.75f, 0.75f, 0.75f, 1.0f };//模拟太阳光照射形成光照效果
GLfloat  ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };//环绕光
GLfloat  spotDir[] = { 0.0f, 0.0f, 1.0f };
GLboolean bEdgeFlag = true;
void resetPerspectiveProjection();
void setOrthographicProjection();

// 画一个圆，主要用于封住圆柱的底和顶
void DrawCircle(GLfloat R) {
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    for (GLfloat r = 0; r < R; r += 0.01)
    {
        glBegin(GL_LINE_STRIP);
        for (GLfloat theta = 0; theta <= 360; theta += 0.1)
            glVertex3f(r * cos(theta * PI / 180), r * sin(theta * PI / 180), 0);
        glEnd();
    }
    glPopMatrix();
}

void SetupRC(void)
{
    glEnable(GL_DEPTH_TEST);     //开启深度缓冲，隐藏被遮挡的物体
    glEnable(GL_CULL_FACE);//只渲染正面
    glEnable(GL_LIGHTING);//开启光照
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);//设置全局环境光为白光
    glLightfv(GL_LIGHT0, GL_DIFFUSE, ambientLight);//光源中的散射光强度 
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular); //光源中的镜面反射光强度
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);//指定光源位置
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0f);//指定光源的最大散布角
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 20.0f);//指定聚焦光源指数。
    glEnable(GL_LIGHT0);//开启GL_LIGHT0光源
    glEnable(GL_COLOR_MATERIAL);//启用函数
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);//可以用颜色来贴物体。如果没有这行代码，纹理将始终保持原来的颜色，
    glMaterialfv(GL_FRONT, GL_SPECULAR, specref); // 设置多边形正面的镜面反射属性
    glMateriali(GL_FRONT, GL_SHININESS, 8);// 指定镜面指数
    glClearColor(0, 1, 1, 1);//设置背景色
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);//把窗口清除为当前颜色
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();//重置当前指定的矩阵为单位矩阵
    gluLookAt(xView, yView, zView, 0, 0, 0, 0, 1, zView - 10);//定义一个视图矩阵，并与当前矩阵相乘
    glShadeModel(GL_SMOOTH);//控制opengl中绘制指定两点间其他点颜色的过渡模式
    // glColor3f(1, 0, 0);
    // glutSolidCube(1);
    robot->Display();
    glutSwapBuffers();
}



void reshape(int w1, int h1)
{
    w = w1;
    h = h1;
    glViewport(0, 0, (GLsizei)w1, (GLsizei)h1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (GLfloat)w1 / (GLfloat)h1, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -8.0);
}
void keyboard(unsigned char key, int x, int y)
{
    switch (key) {

    case 'u'://眼睛前移
        zView = zView + 0.3;
        glutPostRedisplay();
        break;
    case'j'://眼睛后移
        zView = zView - 0.3;
        glutPostRedisplay();
        break;
    case 'h':// 视线左移
        xView = xView - 0.3;
        glutPostRedisplay();
        break;
    case 'k':// 视线右移
        xView = xView + 0.3;
        glutPostRedisplay();
        break;
    case 'n':// 视线上移
        yView = yView + 0.3;
        glutPostRedisplay();
        break;
    case 'm':// 视线下移
        yView = yView - 0.3;
        glutPostRedisplay();
        break;
    case 'w': // 当前方向往前走 
        glutPostRedisplay();
        break;
    case 's': // 当前方向往后走 
        glutPostRedisplay();
        break;
    case 'd': case 'D': // 顺时针旋转
        robot->ClockwiseRotate();
        glutPostRedisplay();
        break;
    case 'a': case 'A': // 逆时针旋转 
        robot->AntiClockwiseRotate();
        glutPostRedisplay();
        break;
    case 'l':
        glutPostRedisplay();
        break;
    case 'L':
        glutPostRedisplay();
        break;
    case 'e':
        exit(0);
        break;
    default:
        break;
    }
}
//设置视觉角度  
void setOrthographicProjection()
{
    // glMatrixMode(GL_PROJECTION);   
    glMatrixMode(GL_MODELVIEW);
    //glMatrixMode(GL_TEXTURE); 
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glScalef(1, -1, 1);
    glTranslatef(0, -h, 0);
    glMatrixMode(GL_MODELVIEW);
}
void resetPerspectiveProjection()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

//光源的坐标变化
void SpecialKeys(int key, int x, int y)
{
    if (key == GLUT_KEY_UP) {}
    if (key == GLUT_KEY_DOWN) {}
    if (key == GLUT_KEY_LEFT) {}
    if (key == GLUT_KEY_RIGHT) {}
    if (key > 356.0f) {}
    if (key < -1.0f) {}
    if (key > 356.0f) {}
    if (key < -1.0f) {}
    glutPostRedisplay();
}
//鼠标事件
void Mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)//鼠标按下
    {
        if (x < 0)
        {
            //向左旋转
           // yRot -= 50.0f;
        }
        else if (x >= 0)
        {
            //向右旋转
           // yRot += 50.0f;
        }
        else if (y >= 0)
        {
            //向上旋转
           // xRot -= 50.0f;
        }
        else if (y < 0)
        {
            //向下旋转
           // xRot += 50.0f;
        }
        //if (xRot > 356.0f)
        //    xRot = 0.0f;
        //if (xRot < -1.0f)
        //    xRot = 355.0f;
        //if (yRot > 356.0f)
        //    yRot = 0.0f;
        //if (yRot < -1.0f)
        //    yRot = 355.0f;
        glutPostRedisplay();
    }
}
//时间函数,定时刷新

void TimerFunction(int value)
{
    display();
    glutPostRedisplay();

    glutTimerFunc(50, TimerFunction, 1);
}


//主函数
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Robot");
    std::cout << "Please Enjoy the Robot" << std::endl;
    SetupRC();
    robot->Generate();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(SpecialKeys);
    glutMouseFunc(Mouse);
    glutTimerFunc(20, TimerFunction, 1);
    glutIdleFunc(display);
    glutMainLoop();
    delete robot;
    return 0;
}
