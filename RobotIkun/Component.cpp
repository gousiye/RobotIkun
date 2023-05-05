#include "Component.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <gl/glut.h>   
#include <cmath> 
#include <iostream>
#include "DrawTool.h"

const float Component::PI = 3.14159;

void Component::InitCoordinate()
{
	glTranslatef(this->x, this->y, this->z);
	glRotatef(this->angle, angleAxis[0], angleAxis[1], angleAxis[2]);
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

void Component::SetPosition(float newPosition[3]) {

	this->x = newPosition[0];
	this->y = newPosition[1];
	this->z = newPosition[2];
}

void Component::SetAngleAxis(float X, float Y, float Z)
{
	this->angleAxis[0] = X;
	this->angleAxis[1] = Y;
	this->angleAxis[2] = Z;
}

void Component::SetColor(GLubyte r, GLubyte g, GLubyte b)
{
	this->color[0] = r;
	this->color[1] = g;
	this->color[2] = b;
}

void Component::Flag(float r)
{
	glPushMatrix();
	InitCoordinate();
	glutWireSphere(r, 30, 30);
	glPopMatrix();
}

void Component::WholeRotate(float angle, float x, float y, float z, int accumulate) {
	this->SetAngleAxis(x, y, z);
	this->SetAngle(this->angle + angle * accumulate);
}

void Component::AddSufPart(Component* suffix) {
	if (this->sufPart.count(suffix->GetLabel())) return;
	this->sufPart[suffix->GetLabel()] = suffix;
}

Head::Head(string label1, Body* body, float scale[3], GLubyte color[3]):
	Component(label1, color)
{
	if (body == nullptr) return;
	if (scale != nullptr) {
		this->scale[0] = scale[0];
		this->scale[1] = scale[1];
		this->scale[2] = scale[2];
	}
	this->x = 0;
	this->y = body->getScale()[1] / 2 + this->scale[1] / 2;
	this->z = 0;
	body->AddSufPart(this);
}

void Head::Display()
{
	glPushMatrix();
	InitCoordinate();
	for (auto i : this->sufPart) {
		i.second->Display();
	}
	glScalef(this->scale[0], this->scale[1], this->scale[2]);
	glColor3ub(this->color[0], this->color[1], this->color[2]);
	glutSolidCube(1.0);
	glPopMatrix();
}

Hair::Hair(Head* head, string label1, GLubyte color[3], float density1, float linewidtdh1) :
	Component(label1, color), density(density1), lineWidth(linewidtdh1) 
{
	if (head == nullptr) return;
	this->head = head;
	this->x = 0;
	this->y = head->getScale()[1] / 2;
	this->z = 0;
	head->AddSufPart(this);
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
	this->x = 1.0f / 4.0f * num * head->getScale()[0];
	this->y = 1.0f / 4 * head->getScale()[1];
	this->z = head->getScale()[2]/ 2 + 0.01;
	head->AddSufPart(this);
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
	this->x = 0;
	this->y = - 1.0f / 4 * head->getScale()[1];
	this->z =  head->getScale()[2] / 2 + 0.01;
	head->AddSufPart(this);
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
	glTranslatef(0, this->scale[1] / 2, 0);
	InitCoordinate();
	for (auto i : sufPart) {
		i.second->Display();
	}

	glScalef(this->scale[0], this->scale[1], this->scale[2]);
	glColor3ub(this->color[0], this->color[1], this->color[2]);


	glutSolidCube(1.0);
	glPopMatrix();
}

Strip::Strip(Body * body, string label1, GLubyte color[3], float linewidth1):
	Component(label1, color), lineWidth(linewidth1)
{	
	if (body == nullptr) return;
	this->body = body;
	// 四条带子的中心坐标是躯体的中心坐标
	this->x = 0;
	this->y = 0;
	this->z = 0;
	body->AddSufPart(this);
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

Joint::Joint(Component* preComponent, string label1, GLubyte color[3], float r, int slice1):
	Component(label1, color), slice(slice1), radius(r)
{
	if (preComponent == nullptr) return;
}

void Joint::Display() {
	glPushMatrix();
	InitCoordinate();
	for (auto i : this->sufPart) {
		i.second->Display();
	}
	glColor3ub(this->color[0], this->color[1], this->color[2]);
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
	this->x = - num * body->getScale()[0] / 2.0f + num * this->radius / 1.0f;
	this->y = - body->getScale()[1] / 2.0f;
	this->z = 0;
	void ChangeAngles(int variation[3]);
	body->AddSufPart(this);
}

Shoulder::Shoulder(Body* body, string label1, GLubyte color[3], int num, float r, int slice1):
	BodyJoint(body, label1, color, r, slice1)
{
	if (body == nullptr) return;
	this->x = - num * body->getScale()[0] / 2.0f - num * this->radius;
	this->y = body->getScale()[1] / 2.0f - this->radius / 2.0f;
	this->z = 0;
	body->AddSufPart(this);
}

Limb::Limb(Joint* preJoint, string label1, GLubyte color[3], float r, float h,
	int accurate1, int slice1):
	Component(label1, color), radiusTop(r), radiusBottom(r), accurate(accurate1), height(h), slice(slice1)
{
	if (preJoint == nullptr) return;
}

void Limb::Display()
{
	glPushMatrix();
	InitCoordinate();
	for (auto i : this->sufPart) {
		i.second->Display();
	}
	glColor3ub(this->color[0], this->color[1], this->color[2]);
	DrawTool::DrawCylinder(this->radiusTop, this->radiusBottom, this->height, this->accurate);

	glPopMatrix();
}

Thigh::Thigh(Coxa* coxa, string label1, GLubyte color[3], float r, float h, int accurate1, int slice1):
	Limb(coxa, label1, color, r, h, accurate1, slice1)
{
	if (coxa == nullptr) return;
	this->x = 0;
	this->y = 0;
	this->z = 0;
	coxa->AddSufPart(this);
}

Knee::Knee(Thigh* thigh, string label1, GLubyte color[3], float r, int slice1):
	LimbJoint(thigh, label1, color, r, slice1)
{
	if (thigh == nullptr) return;
	this->x = 0;
	this->y = -thigh->GetH() - this->radius;
	this->z = 0;
	thigh->AddSufPart(this);
}

Shank::Shank(Knee* knee, string label1, GLubyte color[3], float rTop, float rBottom,
	float h, int accurate1, int slice1) :
	Limb(knee, label1, color, rTop, h, accurate1, slice1)
{
	this->radiusBottom = rBottom;
	if (knee == nullptr) return;
	this->x = 0;
	this->y = - knee->GetR();
	this->z = 0;
	knee->AddSufPart(this);
}

Ankle::Ankle(Shank* shank, string label1, GLubyte color[3], float r, int slice):
	LimbJoint(shank, label1, color, r, slice)
{
	if (shank == nullptr) return;
	this->x = 0;
	this->y = - shank->GetH() - this->radius;
	this->z = 0;
	shank->AddSufPart(this);
}

Foot::Foot(Ankle* ankle, string label1, float scale[3], GLubyte color[3]):
	Component(label1, color)
{
	if (scale != nullptr) {
		this->scale[0] = scale[0];
		this->scale[1] = scale[1];
		this->scale[2] = scale[2];
	}
	this->x = 0;
	this->y = - ankle->GetR() / 2.0f - this->scale[1] / 2.0f;
	this->z = this->scale[2] / 2.0f - ankle->GetR();
	ankle->AddSufPart(this);
}

void Foot::Display()
{
	glPushMatrix();
	InitCoordinate();
	glScalef(this->scale[0], this->scale[1], this->scale[2]);
	glutSolidCube(1.0f);
	glPopMatrix();
}

Boom::Boom(Shoulder* shoulder, string label1, GLubyte color[3], float r, float h,
	int accurate1, int slice1) :
	Limb(shoulder, label1, color, r, h, accurate1, slice1) 
{

	if (shoulder == nullptr) return;
	this->x = 0;
	this->y = - shoulder->GetR() / 2.0f;
	this->z = 0;
	shoulder->AddSufPart(this);

}

Elbow::Elbow(Boom* boom, string label1, GLubyte color[3], float r, int slice):
	LimbJoint(boom, label1, color, r, slice)
{
	if (boom == nullptr) return;
	this->x = 0;
	this->y = - boom->GetH() - this->radius;
	this->z = 0;
	boom->AddSufPart(this);
}

Forearm::Forearm(Elbow* elbow, string label1, GLubyte color[3], float rTop, float rBottom, 
	float h, int accurate1, int slice1):
	Limb(elbow, label1, color, rTop, h, accurate1, slice1)
{
	this->radiusBottom = rBottom;
	if (elbow == nullptr) return;
	this->x = 0;
	this->y = - elbow->GetR();
	this->z = 0;
	elbow->AddSufPart(this);
}

Wrist::Wrist(Forearm* forearm, string label1, GLubyte color[3], float r, int slice):
	LimbJoint(forearm, label1, color, r, slice)
{
	if (forearm == nullptr) return;
	this->x = 0;
	this->y = - forearm->GetH() - this->radius;
	this->z = 0;
	forearm->AddSufPart(this);

}

Hand::Hand(Wrist* wrist, string label1, GLubyte color[3], float r, float h, 
	float ratio1, int accurate1, int slice1):
	Component(label1, color), wrist(wrist), radius(r), height(h), accurate(accurate1), ratio(ratio1), slice(slice1)
{
	if (wrist == nullptr) return;
	this->x = 0;
	this->y = - wrist->GetR() - this->radius;
	this->z = 0;
	wrist->AddSufPart(this);
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
