#include "Robot.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <gl/glut.h>   
#include <cmath> 
#include <iostream>
#include <Windows.h>
#include <thread>
#include <condition_variable>
#include <semaphore>

#define PI 3.14159
#define ENTER 13
#define SPACE 32


// 窗口
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


// 简单渲染
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
    robot->Display();
    glPushMatrix();

    glTranslatef(0, -3, 0);
    glScalef(999, 0.05, 0.05f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidCube(1);   // 视角的参考线
    glPopMatrix();
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
    display();
}


void keyboard(unsigned char key, int x, int y)
{

    switch (key) {

    case 'u': case 'U'://眼睛前移
        zView = zView + 0.3;
        glutPostRedisplay();
        break;
    case'h': case'H'://眼睛后移
        zView = zView - 0.3;
        glutPostRedisplay();
        break;
    case 'j': case'J':// 视线左移
        xView = xView - 0.3;
        glutPostRedisplay();
        break;
    case 'l': case 'L':// 视线右移
        xView = xView + 0.3;
        glutPostRedisplay();
        break;
    case 'i': case 'I':// 视线上移
        yView = yView + 0.3;
        glutPostRedisplay();
        break;
    case 'k': case 'K':// 视线下移
        yView = yView - 0.3;
        glutPostRedisplay();
        break;
    case 'w': case 'W':// 当前方向往前走
        if(!robot->IsLock() && !robot->IsPause())   // 铁山靠期间不能移动，不能阻塞主线程，因为行走
            robot->WalkForward();
        glutPostRedisplay();
        break;
    case 's': case 'S': // 当前方向往后走 
        if (!robot->IsLock() && !robot->IsPause())
            robot->WalkBackward();
        robot->WalkBackward();
        glutPostRedisplay();
        break;
    case ENTER:   // 回车键开始铁山靠
    {
        robot->LockMyMutex(); // 上锁
        std::thread dance(&Robot::Dance, robot);  // 在做铁山靠的时候能够转动视角
        dance.detach();  // 主线程可以先结束的
        glutPostRedisplay();
        break;
    }
    case SPACE:  //空格暂停， 开始, 通过条件变量来实现
    {
        std::condition_variable cv;
        robot->ChangePauseState(); 
        robot->Notify(); // 条件变量的判断
        glutPostRedisplay();
        break;
    }
    case 'r': case'R':  // 这轮摇摆后退出
        robot->StopSwing();
        break;
    case 'd': case 'D': // 顺时针旋转
        robot->ClockwiseRotate();
        glutPostRedisplay();
        break;
    case 'a': case 'A': // 逆时针旋转 
        robot->AntiClockwiseRotate();
        glutPostRedisplay();
        break;
    case 'e': case 'E': //退出
        exit(0);
        break;
    default:
        break;
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
    robot->SetWindowDisplay(display);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(20, TimerFunction, 1);
    glutIdleFunc(display);
    glutMainLoop();
    delete robot;
    return 0;
}
