#include "Component.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <gl/glut.h>   
#include <cmath> 
#include <iostream>
#include "DrawTool.h"

#define PI 3.14159

void Component::InitCoordinate()
{
	glTranslatef(this->x, this->y, this->z);
	glRotatef(this->angles[0], 1, 0, 0);
	glRotatef(this->angles[1], 0, 1, 0);
	glRotatef(this->angles[2], 0, 0, 1);
	glColor3ub(this->color[0], this->color[1], this->color[2]);
}

Component::Component(string label1, float position[3], GLubyte color[3]): label(label1) {
	if (position != nullptr) {
		x = position[0];
		y = position[1];
		z = position[2];
	}
	if (color != nullptr) {
		this->color[0] = color[0];
		this->color[1] = color[1];
		this->color[2] = color[2];
	}
}

Component::Component(string label1, GLubyte color[3]) : label(label1) {
	if (color != nullptr) {
		this->color[0] = color[0];
		this->color[1] = color[1];
		this->color[2] = color[2];
	}
}

void Component::SetPosition(float newX, float newY, float newZ)
{
	this->x = newX;
	this->y = newY;
	this->z = newZ;
}

void Component::SetAngles(float angleX, float angleY, float angleZ)
{
	this->angles[0] = angleX;
	this->angles[1] = angleY;
	this->angles[2] = angleZ;
}

void Component::SetColor(GLubyte r, GLubyte g, GLubyte b)
{
	this->color[0] = r;
	this->color[1] = g;
	this->color[2] = b;
}

void Component::Flag()
{
	glPushMatrix();
	InitCoordinate();
	glutWireSphere(1, 30, 30);
	glPopMatrix();
}




Head::Head(string label1, float position[3], float scale[3], GLubyte color[3]):
	Component(label1, position, color)
{
	if (scale != nullptr) {
		this->scale[0] = scale[0];
		this->scale[1] = scale[1];
		this->scale[2] = scale[2];
	}
}

void Head::Display()
{
	glPushMatrix();
	InitCoordinate();
	glScalef(this->scale[0], this->scale[1], this->scale[2]);
	glutSolidCube(1.0);
	glPopMatrix();
}

Hair::Hair(Head* head, string label1, GLubyte color[3], float density1, float linewidtdh1) :
	Component(label1, color), density(density1), lineWidth(linewidtdh1) 
{
	if (head == nullptr) return;
	this->head = head;
	this->x = head->getX();
	this->y = head->getY() + head->getScale()[1] / 2;
	this->z = head->getZ();
}


void Hair::Display()
{
	glPushMatrix();
	InitCoordinate();
	glLineWidth(this->lineWidth);
	const GLfloat length = this->head->getScale()[2] / 2;
	const GLfloat width = this->head->getScale()[0];
	for (GLfloat j = -length; j <= length; j += density) {
		glBegin(GL_LINE_STRIP);
		for (GLfloat i = -width; i <= width; i += density) {
			GLfloat height = 0.15 * cos(PI / width * i);
			for (GLfloat h = 0; h <= height; h += density) {
				glVertex3f(i, h, j);
			}
			glVertex3f(i, height, j);
		}
		glEnd();
	}
	glLineWidth(1);
	glPopMatrix();
}


Eye::Eye(Head* head, string label1, GLubyte color[3], int num, float r, int accurate1):
	Component(label1, color), radius(r), accurate(accurate1)
{
	if (head == nullptr) return;
	this->head = head;
	this->x = head->getX() + 1.0f / 4.0f * num * head->getScale()[0];
	this->y = head->getY() + 1.0f / 4 * head->getScale()[1];
	this->z = head->getZ() + head->getScale()[2]/ 2 + 0.01;
}

void Eye::Display()
{
	glPushMatrix();
	InitCoordinate();
	DrawTool::DrawCircle(this->radius, this->accurate);
	glPopMatrix();
}



Mouth::Mouth(Head* head, string label1, GLubyte color[3], float r, float ratio1, int accurate1):
	Component(label1, color), radius(r), ratio(ratio1), accurate(accurate1)
{
	if (head == nullptr) return;
	this->head = head;
	this->x = head->getX();
	this->y = head->getY() - 1.0f / 4 * head->getScale()[1];
	this->z = head->getZ() + head->getScale()[2] / 2 + 0.01;
}

void Mouth::Display()
{
	glPushMatrix();
	InitCoordinate();
	glScalef(1, 0.5, 1);
	DrawTool::DrawCircle(this->radius, this->accurate);
	glPopMatrix();

}



Body::Body(string label1, float position[3], float scale[3], GLubyte color[3]) :
	Component(label1, position, color)
{
	if (scale != nullptr) {
		this->scale[0] = scale[0];
		this->scale[1] = scale[1];
		this->scale[2] = scale[2];
	}
}

void Body::Display()
{
	glPushMatrix();
	InitCoordinate();
	glScalef(this->scale[0], this->scale[1], this->scale[2]);
	glutSolidCube(1.0);
	glPopMatrix();
}



Strip::Strip(Body * body, string label1, GLubyte color[3], float linewidth1):
	Component(label1, color), lineWidth(linewidth1)
{	
	if (body == nullptr) return;
	this->body = body;
	// 四条带子的中心坐标是躯体的中心坐标
	this->x = body->getX();
	this->y = body->getY();
	this->z = body->getZ();
}

void Strip::Display()
{
	glPushMatrix();
	InitCoordinate();
	glLineWidth(this->lineWidth);
	for (int i = -1; i <= 1; i += 2) {
		for (int j = -1; j <= 1; j += 2) {
			float tmpX = i * 5.0f / 14.0f * this->body->getScale()[0];
			float tmpY = this->body->getScale()[1] / 2.0f;
			float tmpZ = j * this->body->getScale()[2] / 2.0f + 0.01 * j;
			glBegin(GL_LINES);
			glVertex3f(tmpX, tmpY, tmpZ);
			glVertex3f(tmpX, tmpY -1 * this->body->getScale()[1], tmpZ);
			glEnd();
		}
	}
	glLineWidth(1);
	glPopMatrix();
}


Joint::Joint(Component* component, string label1, GLubyte color[3], float r, int slice1):
	Component(label1, color), slice(slice1), radius(r)
{
	if (component == nullptr) return;
	this->component = component;
}

void Joint::Display() {
	glPushMatrix();
	InitCoordinate();
	glutSolidSphere(this->radius, this->slice, this->slice);
	glPopMatrix();
}


BodyJoint::BodyJoint(Body* body, string label1, GLubyte color[3], float r, int slice1):
	Joint(body, label1, color, r, slice1)
{

}


LimbJoint::LimbJoint(Limb* limb, string label1, GLubyte color[3], float r, int slice1) :
	Joint(limb, label1, color, r, slice1)
{

}


Coxa::Coxa(Body* body, string label1, GLubyte color[3], int num, float r, int slice1) :
	BodyJoint(body, label1, color, r, slice1)
{
	if (body == nullptr) return;
	this->component = body;
	this->x = body->getX() - num * body->getScale()[0] / 2.0f + num * this->radius / 1.0f;
	this->y = body->getY() - body->getScale()[1] / 2.0f;
	this->z = body->getZ();
}


Shoulder::Shoulder(Body* body, string label1, GLubyte color[3], int num, float r, int slice1):
	BodyJoint(body, label1, color, r, slice1)
{
	if (body == nullptr) return;
	this->component = body;
	this->x = body->getX() - num * body->getScale()[0] / 2.0f - num * this->radius;
	this->y = body->getY() + body->getScale()[1] / 2.0f - this->radius / 2.0f;
	this->z = body->getZ();
}



Limb::Limb(Joint* joint, string label1, GLubyte color[3], float r, float h,
	int accurate1, int slice1):
	Component(label1, color), joint(joint), radiusTop(r), radiusBottom(r), accurate(accurate1), height(h), slice(slice1)
{
	if (joint == nullptr) return;
	this->joint = joint;
	//this->x = coxa->getX();
	//this->y = coxa->getY();
	//this->z = coxa->getZ();
}

void Limb::Display()
{
	glPushMatrix();
	InitCoordinate();
	DrawTool::DrawCylinder(this->radiusTop, this->radiusBottom, this->height, this->accurate);
	glPopMatrix();
}

Thigh::Thigh(Joint* joint, string label1, GLubyte color[3], float r, float h, int accurate1, int slice1):
	Limb(joint, label1, color, r, h, accurate1, slice1)
{
	if (joint == nullptr) return;
	this->joint = joint;
	this->x = joint->getX();
	this->y = joint->getY();
	this->z = joint->getZ();
}


Knee::Knee(Thigh* thigh, string label1, GLubyte color[3], float r, int slice1):
	LimbJoint(thigh, label1, color, r, slice1)
{
	if (thigh == nullptr) return;
	this->x = thigh->getX();
	this->y = thigh->getY() - thigh->getH() - this->radius;
	this->z = thigh->getZ();
}

Shank::Shank(Joint* joint, string label1, GLubyte color[3], float rTop, float rBottom,
	float h, int accurate1, int slice1) :
	Limb(joint, label1, color, rTop, h, accurate1, slice1)
{
	this->radiusBottom = rBottom;
	if (joint == nullptr) return;
	this->joint = joint;
	this->x = joint->getX();
	this->y = joint->getY() - joint->getR();
	this->z = joint->getZ();
}


Ankle::Ankle(Shank* shank, string label1, GLubyte color[3], float r, int slice):
	LimbJoint(shank, label1, color, r, slice)
{
	if (shank == nullptr) return;
	this->x = shank->getX();
	this->y = shank->getY() - shank->getH() - this->radius;
	this->z = shank->getZ();
}


Foot::Foot(Ankle* ankle, string label1, float scale[3], GLubyte color[3]):
	Component(label1, color)
{
	if (scale != nullptr) {
		this->scale[0] = scale[0];
		this->scale[1] = scale[1];
		this->scale[2] = scale[2];
	}
	this->x = ankle->getX();
	this->y = ankle->getY() - ankle->getR() / 2.0f - this->scale[1] / 2.0f;
	this->z = ankle->getZ() + this->scale[2] / 2.0f - ankle->getR();

}

void Foot::Display()
{
	glPushMatrix();
	InitCoordinate();
	glScalef(this->scale[0], this->scale[1], this->scale[2]);
	glutSolidCube(1.0f);
	glPopMatrix();
}

Boom::Boom(Joint* joint, string label1, GLubyte color[3], float r, float h,
	int accurate1, int slice1) :
	Limb(joint, label1, color, r, h, accurate1, slice1) 
{

	if (joint == nullptr) return;
	this->x = joint->getX();
	this->y = joint->getY() - joint->getR() / 2.0f;
	this->z = joint->getZ();
}

Elbow::Elbow(Boom* boom, string label1, GLubyte color[3], float r, int slice):
	LimbJoint(boom, label1, color, r, slice)
{
	if (boom == nullptr) return;
	this->x = boom->getX();
	this->y = boom->getY() - boom->getH() - this->radius;
	this->z = boom->getZ();
}

Forearm::Forearm(Elbow* joint, string label1, GLubyte color[3], float rTop, float rBottom, 
	float h, int accurate1, int slice1):
	Limb(joint, label1, color, rTop, h, accurate1, slice1)
{
	this->radiusBottom = rBottom;
	if (joint == nullptr) return;
	this->joint = joint;
	this->x = joint->getX();
	this->y = joint->getY() - joint->getR();
	this->z = joint->getZ();
}

Wrist::Wrist(Forearm* forearm, string label1, GLubyte color[3], float r, int slice):
	LimbJoint(forearm, label1, color, r, slice)
{
	if (forearm == nullptr) return;
	this->x = forearm->getX();
	this->y = forearm->getY() - forearm->getH() - this->radius;
	this->z = forearm->getZ();
}

Hand::Hand(Wrist* wrist, string label1, GLubyte color[3], float r, float h, 
	float ratio1, int accurate1, int slice1):
	Component(label1, color), wrist(wrist), radius(r), height(h), accurate(accurate1), ratio(ratio1), slice(slice1)
{
	if (wrist == nullptr) return;
	this->x = wrist->getX();
	this->y = wrist->getY() - wrist->getR() - this->radius;
	this->z = wrist->getZ();
}

void Hand::Display()
{
	glPushMatrix();
	InitCoordinate();
	glRotatef(90, 0, 0, 1);
	glTranslatef(0.0f, this->height / 2.0f, 0.0f);
	glScalef(1.0, 1.0f, this->ratio);
	DrawTool::DrawCylinder(this->radius, this->radius, this->height, this->accurate, this->slice);
	glPopMatrix();
}
