#pragma once
#include <GL/glut.h>
#include <stdlib.h>
#include <gl/glut.h>   
#include <cmath> 
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "DrawTool.h"


using std::string;
using std::vector;

// ���в����Ļ���
class Component
{
protected:

	struct RotateStruct {
	public:
		float x = 0;
		float y = 0;
		float z = 0;
		float angle = 0;

		RotateStruct(float angle1, float x1, float y1, float z1) {
			angle = angle1;
			x = x1;
			y = y1;
			z = z1;
		}
	};

	// ��ǰ��
	float x = 0;    // x����
	float y = 0;    // y����
	float z = 0;    // z����
	// ��ʼ�̶���
	float initX = 0;
	float initY = 0;
	float initZ = 0;
	
	float savedAngle = 0;
	GLubyte color[3] = { 0, 0, 0 }; //��ɫ
	vector<RotateStruct> RotateMemo;  //��¼������ת����
	float D2R(float angle) { return angle * PI / 180; }
	void InitCoordinate();   //����opengl������ϵ����Ӧλ�úͽǶ�
	void Memo() {    // ��¼��ʼ�����꣬���ھ�ȷ�ظ�
		initX = x;
		initX = y;
		initZ = z;
	} 
	std::map<string, Component*> sufPart;  // ��ЩС�Ĳ��������ڴ�Ĳ���
	unsigned int  traceBack = -1;  // ���ڻ��ݵ�ָ��Rotate matrix

public:

	void AddSufPart(Component*);
	const string label; // ����������һ������
	const static float PI;	
	Component(string label1, float position[3], GLubyte color[3]);
	Component(string label1, GLubyte color[3]);
	Component() {}

	virtual void Display() = 0;   // ��ʾ�������
	void SetPosition(float newX, float newY, float newZ);   // ��������
	void SetPosition(float newPosition[3]);   // ��������
	void Translate(float x, float y, float z) {
		this->x += x;
		this->y += y;
		this->z += z;
	}
	void SaveRotateMatrix() {
		savedAngle = RotateMemo.back().angle;
		if (RotateMemo.size() == 0) return;
		traceBack = RotateMemo.size() - 1;
	}
	//���ݵ�֮ǰ�����RotateMatrix
	void Trace2Saved() {
		if (traceBack == -1 || traceBack >= RotateMemo.size()) {
			RotateMemo.clear();
			return;
		}
		int m = RotateMemo.size() - 1 - traceBack;
		while (m--) {
			RotateMemo.pop_back();
		}
		RotateMemo.back().angle = savedAngle;
	}
	void WholeRotate(float angle, float x, float y, float z, int accumulate);
	void PopLastRotate() {
		if (RotateMemo.size() == 0) return;
		RotateMemo.pop_back();
	}
	void SetColor(GLubyte r, GLubyte g, GLubyte b); // ������ɫ
	void Flag(float r = 1); // ������λ�û�һ�������ڶ�λ�͵���

	const float& GetX() const { return x; }
	const float& GetY() const { return y; }
	const float& GetZ() const { return z; }

	const float& GetInitX() const { return initX; }
	const float& GetInitY() const { return initY; }
	const float& GetInitZ() const { return initZ; }

	const float GetAngle() const { 
		if (RotateMemo.size() == 0) return -999;
		else return RotateMemo.back().angle; }

	const float GetRotateX() const {	
		if (this->RotateMemo.size() == 0) return 0;
		return RotateMemo.back().x;
	}

	const float GetRotateY() const {
		if (this->RotateMemo.size() == 0) return 0;
		return RotateMemo.back().y;
	}

	const float GetRotateZ() const {
		if (this->RotateMemo.size() == 0) return 0;
		return RotateMemo.back().z;
	}
	
	const int& GetRotateSize() const { return this->RotateMemo.size(); }

	void SetX(float x1) { this->x = x1; }
	void SetY(float y1) { this->y = y1; }
	void SetZ(float z1) { this->z = z1; }

	void SetCurrAngle(float angle1) { 
		if (RotateMemo.size() == 0) return;
		RotateMemo.back().angle = angle1; 
	}
	void SetCurrAxis(float x1, float y1, float z1) {
		RotateMemo.back().x = x1;
		RotateMemo.back().y = y1;
		RotateMemo.back().z = z1;
	}

	const string& GetLabel() const { return label; }


};

// ͷ
class Body;
class Head : public Component {
private:
	float scale[3] = { 1, 1, 1 }; //ͷ�Ǹ������壬��Ӧ�ĳ����
	Body* body = nullptr;

public:
	Head(string label1, Body* body, float scale[3], GLubyte color[3]);
	const float* getScale() const { return &scale[0]; }
	void Display() override;
};

// ͷ��
class Hair : public Component{
private:
	float density = 0.01; //ѭ���Ĳ���
	float lineWidth = 0.5;  
	Head* head = nullptr;  //�������ĸ�ͷ������

public:
	Hair(Head* head, string label1, GLubyte color[3], float density1 = 0.01f, float linewidtdh1 = 0.5f);
	void Display() override;
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
	void Display() override;
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
	void Display() override;
};

// ����
class Body : public Component {
private:
	float scale[3] = { 1, 1, 1 }; //�����Ǹ������壬��Ӧ�ĳ����

public:
	Body(string label1, float position[3], float scale[3], GLubyte color[3]);
	const float* getScale() const { return &scale[0]; }
	void Display() override;

};

// ��ɫ�ı����㣬 ����������Ϊһ������
class Strip : public Component {
private:
	float lineWidth = 5;
	Body* body = nullptr;   // �������ĸ�������

public:
	Strip(Body* body, string label1, GLubyte color[3], float linewidth1 = 5);

	void Display() override;
};

// �ؽڵĻ���
class Joint : public Component {
protected:
	float radius = 0.15f;
	int slice = 20;

public:
	Joint(Component* preJoint, string label1, GLubyte color[3], float r = 0.15f, int slice1 = 20);
	void Display() override;
	const float& GetR() const { return this->radius; }
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
	float radiusTop = 0.15f;
	float radiusBottom = 0.15f;
	float height = 1.0f;
	int accurate = 70;
	int slice = 70;

public:
	Limb(Joint* precursor, string label1, GLubyte color[3],
		float r = 0.15, float h = 1.0f, int accurate1 = 70, int slice1 = 20);
	void Display() override;
	const float& GetRTop() const { return this->radiusTop; }
	const float& GetRBottom() const { return this->radiusBottom; }
	const float& GetH() const { return this->height; }
};

//����
class Thigh : public Limb {
public:
	Thigh(Coxa* coxa, string labdel1, GLubyte color[3],
		float r = 0.15, float h = 1.0f, int accurate1 = 70, int slice1 = 20);
};

//ϥ��
class Knee : public LimbJoint {
public:
	Knee(Thigh* thigh, string label1, GLubyte color[3],  float r = 0.15, int slice = 20);
};

//С��
class Shank : public Limb {
public:
	Shank(Knee* knee, string label1, GLubyte color[3],
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
	const float* GetScale() const { return &scale[0]; }
	void Display() override;
};

// ���
class Boom : public Limb {
public:
	Boom(Shoulder* shouler, string label1, GLubyte color[3], float r = 0.15f, float h = 1.0f,
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
	Forearm(Elbow* elbow, string label1, GLubyte color[3],
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
	void Display() override;
};


