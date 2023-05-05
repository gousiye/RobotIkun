#pragma once
#include <GL/glut.h>
#include <gl/glut.h>   
static class DrawTool    // 画图的一些快速工具
{

private:
	static const float PI;
	static GLUquadricObj* gObj;

public:
	static void DrawCircle(float r, int accurate = 70);
	static void DrawCylinder(float Rtop, float Rbottom, float H, int accurate = 70, int slice = 20);
};

