#pragma once
#include <GL/glut.h>
#include <stdlib.h>
#include <gl/glut.h>   
#include <cmath> 
#include <iostream>
#include <string>
#include <vector>
#include "DrawTool.h"

using std::string;
using std::vector;

// ���в����Ļ���
class Component
{
protected:
	float x = 0;    // x����
	float y = 0;    // y����
	float z = 0;    // z����
	float angles[3] = { 0,0,0 };   // ��x, y, z��ļн�
	GLubyte color[3] = { 0, 0, 0 }; //��ɫ

	void InitCoordinate();   //����opengl������ϵ����Ӧλ�úͽǶ�


public:
	
	const string label; // ����������һ������

	Component (string label1 , float x1 = 0, float y1 = 0, float z1 = 0,
		GLubyte r = 0, GLubyte g = 0, GLubyte b = 0): 
		label(label1), x(x1), y(y1), z(z1){
		color[0] = r;
		color[1] = g;
		color[2] = b;
	}
	
	Component(string label1, float position[3], GLubyte color[3]);
	Component(string label1, GLubyte color[3]);
	Component() {}

	virtual void Display() = 0;   // ��ʾ�������
	void SetPosition(float newX, float newY, float newZ);   // ��������
	void SetAngles(float angleX, float angleY, float angleZ);  // ���Ķ�Ӧ�н�
	void SetColor(GLubyte r, GLubyte g, GLubyte b); // ������ɫ
	void Flag(); // ������λ�û�һ�������ڶ�λ�͵���

	const float& getX() const { return x; }
	const float& getY() const { return y; }
	const float& getZ() const { return z; }
	const string& getLabel() const { return label; }
};

// ͷ
class Head : public Component {
private:
	float scale[3] = { 1, 1, 1 }; //ͷ�Ǹ������壬��Ӧ�ĳ����

public:
	Head(string label1, float position[3], float scale[3], GLubyte color[3]);
	const float* getScale() const { return &scale[0]; }
	void Display();
};

// ͷ��
class Hair : public Component{
private:
	float density = 0.01; //ѭ���Ĳ���
	float lineWidth = 0.5;  
	Head* head = nullptr;  //�������ĸ�ͷ������

public:
	Hair(Head* head, string label1, GLubyte color[3], float density1 = 0.01f, float linewidtdh1 = 0.5f);
	void Display();
};

// �۾��� ÿֻ�۾���Ϊһ������
class Eye : public Component {
private:
	float radius = 0.15;
	int accurate = 70;
	Head* head = nullptr; // �������ĸ�ͷ����

public:
	// num �ж��������ۣ�Ҳ���Լ��ϸ�����۾�
	Eye(Head* head, string label1, GLubyte color[3], int num, float r = 0.15, int accurate1 = 70);
	void Display();
};

// ��
class Mouth : public Component {
private:
	float radius = 0.15;
	int accurate = 70;
	Head* head = nullptr;
	float ratio = 1.0f; // x����ֱ����y����ֱ���ı���

public:
	Mouth(Head* head, string label1, GLubyte color[3], float r = 0.15,
		float ratio1 = 1.0f, int accurate1 = 70);
	void Display();
};

// ����
class Body : public Component {
private:
	float scale[3] = { 1, 1, 1 }; //�����Ǹ������壬��Ӧ�ĳ����

public:
	Body(string label1, float position[3], float scale[3], GLubyte color[3]);
	const float* getScale() const { return &scale[0]; }
	void Display();

};

// ��ɫ�ı����㣬 ����������Ϊһ������
class Strip : public Component {
private:
	float lineWidth = 5;
	Body* body = nullptr;   // �������ĸ�������

public:
	Strip(Body* body, string label1, GLubyte color[3], float linewidth1 = 5);

	void Display();
};

// �ؽڵĻ���
class Joint : public Component {
protected:
	Component* component = nullptr; // �ؽڳнӵĲ�������
	float radius = 0.15f;
	int slice = 20;

public:
	Joint(Component* component, string label1, GLubyte color[3], float r = 0.15f, int slice1 = 20);
	void Display();
	const float& getR() const { return this->radius; }
};

// �н�����Ĺؽڣ�һ���ؽ���Ϊһ������
class BodyJoint : public Joint {
public:
	BodyJoint(Body* body, string label1, GLubyte color[3], float r = 0.15f, int slice1 = 20);
};

// �н���֫�Ĺؽڣ�һ���ؽ�Ϊһ������
class Limb;
class LimbJoint : public Joint {
public:
	LimbJoint(Limb* limb, string label1, GLubyte color[3], float r = 0.15f, int slice1 = 20);
};

// �Źؽ�(����ʱ���Ȼ��ڵ�ס������·ʱ����϶)
class Coxa : public BodyJoint {
public:
	Coxa(Body* body, string label1, GLubyte color[3], int num, float r = 0.15f ,int slice1 = 20);
};

// ��ؽ�
class Shoulder : public BodyJoint {
public:
	Shoulder(Body* body, string label1, GLubyte color[3], int num, float r = 0.15f, int slice1 = 20);
};

// ֫��Ļ���
class Limb : public Component{
protected:
	Joint* joint = nullptr;   // �������ĸ��ؽ���
	float radiusTop = 0.15f;
	float radiusBottom = 0.15f;
	float height = 1.0f;
	int accurate = 70;
	int slice = 20;

public:
	Limb(Joint* joint, string label1, GLubyte color[3],
		float r = 0.15, float h = 1.0f, int accurate1 = 70, int slice1 = 20);
	void Display();
	const float& getRTop() const { return this->radiusTop; }
	const float& getRBottom() const { return this->radiusBottom; }
	const float& getH() const { return this->height; }
};

//����
class Thigh : public Limb {
public:
	Thigh(Joint* joint, string label1, GLubyte color[3],
		float r = 0.15, float h = 1.0f, int accurate1 = 70, int slice1 = 20);
};

class Knee : public LimbJoint {
public:
	Knee(Thigh* thigh, string label1, GLubyte color[3],  float r = 0.15, int slice = 20);
};

//С��
class Shank : public Limb {
public:
	Shank(Joint* joint, string label1, GLubyte color[3],
		float rTop = 0.15f, float rBottom = 0.1f, float h = 1.0f, int accurate1 = 70, int slice1 = 20);
};

//�׹ؽ�
class Ankle : public LimbJoint {
public:
	Ankle(Shank* shank, string label1, GLubyte color[3], float r = 0.15f, int slice = 20);
};

//��
class Foot : public Component {
private:
	float scale[3] = { 1, 1, 1 }; //�ſ�������һ���ܱ��������
	Ankle* ankle = nullptr;

public:
	Foot(Ankle* ankle, string label1, float scale[3], GLubyte color[3]);
	const float* getScale() const { return &scale[0]; }
	void Display();
};

// ���
class Boom : public Limb {
public:
	Boom(Joint* joint, string label1, GLubyte color[3], float r = 0.15f, float h = 1.0f,
		int accurate1 = 70, int slice1 = 20);
};

// ��ؽ�
class Elbow : public LimbJoint {
public:
	Elbow(Boom* boom, string label1, GLubyte color[3], float r = 0.15f, int slice = 20);
};

// С��
class Forearm : public Limb {
public:
	Forearm(Elbow* joint, string label1, GLubyte color[3],
		float rTop = 0.15f, float rBottom = 0.1f, float h = 1.0f, int accurate1 = 70, int slice1 = 20);
};

// ��ؽ�
class Wrist : public LimbJoint {
public:
	Wrist(Forearm* forearm, string label1, GLubyte color[3], float r = 0.15f, int slice = 20);
};


// ��
class Hand : public Component {
private:
	Wrist* wrist = nullptr;
	float ratio = 1.0f;     // x��z��ı�����x�᷽��Ϊ1
	float radius = 0.15f;
	float height = 0.1f;
	int accurate = 70;
	int slice = 20;

public:
	Hand(Wrist* wrist, string label1, GLubyte color[3], float r = 0.15f, float h = 0.1f,
		float ratio1 = 1.0f, int accurate1 = 70, int slice1 = 20);
	void Display();
};


