#include <GL/glut.h>
#include <gl/glut.h>   
#include "DrawTool.h"
#include <cmath> 

const float DrawTool::PI = 3.14159;
GLUquadricObj* DrawTool::gObj = nullptr;

void DrawTool::DrawCircle(float R, int accurate)
{
	glBegin(GL_POLYGON);
	for (int i = 0; i < accurate; i++)
		glVertex3f(R * cos(2 * PI * i / accurate), R * sin(2 * PI * i/ accurate), 0);
	glEnd();
}

void DrawTool::DrawCylinder(float Rtop, float Rbottom, float H, int accurate, int slice)
{
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);   //上面是外面
	DrawCircle(Rtop, accurate);
	gObj = gluNewQuadric();
	glTranslatef(0, 0, -H / 1.0f);
	gluQuadricDrawStyle(gObj, GLU_FILL);
	gluQuadricNormals(gObj, GLU_SMOOTH);
	gluCylinder(gObj, Rbottom, Rtop, H, slice, slice);

	glRotatef(180, 1, 0, 0); // 下面是外面
	DrawCircle(Rbottom, accurate);
	glPopMatrix();
}
