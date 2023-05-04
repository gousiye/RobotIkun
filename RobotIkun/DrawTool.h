#pragma once
#include <GL/glut.h>
#include <gl/glut.h>   
static class DrawTool    // 画图的一些快速工具
{

private:
	const static float PI;
	static GLUquadricObj* gObj;

public:
	void static DrawCircle(float r, int accurate = 70);
	void static DrawCylinder(float Rtop, float Rbottom, float H, int accurate = 70, int slice = 20);
};

