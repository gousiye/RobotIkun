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

// 所有部件的基类
class Component
{
protected:
	float x = 0;    // x坐标
	float y = 0;    // y坐标
	float z = 0;    // z坐标

	float angleAxis[3] = { 0,0,0 };  //对应的旋转轴 
	float angle = 0.0f; //对应的旋转角度
	GLubyte color[3] = { 0, 0, 0 }; //颜色
	float D2R(float angle) { return angle * PI / 180; }
	void InitCoordinate();   //处理opengl的坐标系到对应位置和角度
	std::map<string, Component*> sufPart;  // 有些小的部件依附于大的部件


public:
	void AddSufPart(Component*);
	const string label; // 标明这是哪一个部件
	const static float PI;	
	Component(string label1, float position[3], GLubyte color[3]);
	Component(string label1, GLubyte color[3]);
	Component() {}

	virtual void Display() = 0;   // 显示这个部件
	void SetPosition(float newX, float newY, float newZ);   // 更改坐标
	void SetPosition(float newPosition[3]);   // 更改坐标

	virtual void SetAngleAxis(float X, float Y, float Z);  // 更改旋转轴
	void SetAngle(float angle1) { this->angle = angle1; }  // 更换旋转角
	void WholeRotate(float angle, float x, float y, float z, int accumulate);
	void SetColor(GLubyte r, GLubyte g, GLubyte b); // 更改颜色
	void Flag(float r = 1); // 在坐标位置画一个球，用于定位和调试

	const float& GetX() const { return x; }
	const float& GetY() const { return y; }
	const float& GetZ() const { return z; }
	const float& GetAngle() const { return this->angle; }
	void SetX(float x1) { this->x = x1; }
	void SetY(float y1) { this->y = y1; }
	void SetZ(float z1) { this->z = z1; }

	const float* GetAngleAxis() const { return &angleAxis[0]; }
	const string& GetLabel() const { return label; }


};

// 头
class Body;
class Head : public Component {
private:
	float scale[3] = { 1, 1, 1 }; //头是个立方体，对应的长宽高
	Body* body = nullptr;

public:
	Head(string label1, Body* body, float scale[3], GLubyte color[3]);
	const float* getScale() const { return &scale[0]; }
	void Display() override;
};

// 头发
class Hair : public Component{
private:
	float density = 0.01; //循环的步长
	float lineWidth = 0.5;  
	Head* head = nullptr;  //依附在哪个头对象上

public:
	Hair(Head* head, string label1, GLubyte color[3], float density1 = 0.01f, float linewidtdh1 = 0.5f);
	void Display() override;
};

// 眼睛， 每只眼睛作为一个对象
class Eye : public Component {
private:
	float radius = 0.15;
	int accurate = 70;
	Head* head = nullptr; // 依附在哪个头对象

public:
	// num 判断是左右眼，也可以加上更多的眼睛
	Eye(Head* head, string label1, GLubyte color[3], int num, float r = 0.15, int accurate1 = 70);
	void Display() override;
};

// 嘴
class Mouth : public Component {
private:
	float radius = 0.15;
	int accurate = 70;
	Head* head = nullptr;
	float ratio = 1.0f; // x方向直径和y方向直径的比例

public:
	Mouth(Head* head, string label1, GLubyte color[3], float r = 0.15,
		float ratio1 = 1.0f, int accurate1 = 70);
	void Display() override;
};

// 躯体
class Body : public Component {
private:
	float scale[3] = { 1, 1, 1 }; //躯体是个立方体，对应的长宽高

public:
	Body(string label1, float position[3], float scale[3], GLubyte color[3]);
	const float* getScale() const { return &scale[0]; }
	void Display() override;

};

// 白色的背带裤， 四条带子作为一个对象
class Strip : public Component {
private:
	float lineWidth = 5;
	Body* body = nullptr;   // 依附在哪个躯体上

public:
	Strip(Body* body, string label1, GLubyte color[3], float linewidth1 = 5);

	void Display() override;
};

// 关节的基类
class Joint : public Component {
protected:
	float radius = 0.15f;
	int slice = 20;

public:
	Joint(Component* preJoint, string label1, GLubyte color[3], float r = 0.15f, int slice1 = 20);
	void Display() override;
	const float& GetR() const { return this->radius; }
};

// 承接躯体的关节，一个关节作为一个对象
class BodyJoint : public Joint {
public:
	BodyJoint(Body* body, string label1, GLubyte color[3], float r = 0.15f, int slice1 = 20);
};

// 承接四肢的关节，一个关节为一个对象
class Limb;
class LimbJoint : public Joint {
public:
	LimbJoint(Limb* limb, string label1, GLubyte color[3], float r = 0.15f, int slice1 = 20);
};

// 髋关节(立正时大腿会遮挡住，在走路时填充缝隙)
class Coxa : public BodyJoint {
public:
	Coxa(Body* body, string label1, GLubyte color[3], int num, float r = 0.15f ,int slice1 = 20);
};

// 肩关节
class Shoulder : public BodyJoint {
public:
	Shoulder(Body* body, string label1, GLubyte color[3], int num, float r = 0.15f, int slice1 = 20);
};

// 肢体的基类
class Limb : public Component{
protected:
	float radiusTop = 0.15f;
	float radiusBottom = 0.15f;
	float height = 1.0f;
	int accurate = 70;
	int slice = 20;

public:
	Limb(Joint* precursor, string label1, GLubyte color[3],
		float r = 0.15, float h = 1.0f, int accurate1 = 70, int slice1 = 20);
	void Display() override;
	const float& GetRTop() const { return this->radiusTop; }
	const float& GetRBottom() const { return this->radiusBottom; }
	const float& GetH() const { return this->height; }
};

//大腿
class Thigh : public Limb {
public:
	Thigh(Coxa* coxa, string labdel1, GLubyte color[3],
		float r = 0.15, float h = 1.0f, int accurate1 = 70, int slice1 = 20);
};

class Knee : public LimbJoint {
public:
	Knee(Thigh* thigh, string label1, GLubyte color[3],  float r = 0.15, int slice = 20);
};

//小腿
class Shank : public Limb {
public:
	Shank(Knee* knee, string label1, GLubyte color[3],
		float rTop = 0.15f, float rBottom = 0.1f, float h = 1.0f, int accurate1 = 70, int slice1 = 20);
};

//踝关节
class Ankle : public LimbJoint {
public:
	Ankle(Shank* shank, string label1, GLubyte color[3], float r = 0.15f, int slice = 20);
};

//脚
class Foot : public Component {
private:
	float scale[3] = { 1, 1, 1 }; //脚可以视作一个很扁的立方体
	Ankle* ankle = nullptr;

public:
	Foot(Ankle* ankle, string label1, float scale[3], GLubyte color[3]);
	const float* GetScale() const { return &scale[0]; }
	void Display() override;
};

// 大臂
class Boom : public Limb {
public:
	Boom(Shoulder* shouler, string label1, GLubyte color[3], float r = 0.15f, float h = 1.0f,
		int accurate1 = 70, int slice1 = 20);
};

// 肘关节
class Elbow : public LimbJoint {
public:
	Elbow(Boom* boom, string label1, GLubyte color[3], float r = 0.15f, int slice = 20);
};

// 小臂
class Forearm : public Limb {
public:
	Forearm(Elbow* elbow, string label1, GLubyte color[3],
		float rTop = 0.15f, float rBottom = 0.1f, float h = 1.0f, int accurate1 = 70, int slice1 = 20);
};

// 腕关节
class Wrist : public LimbJoint {
public:
	Wrist(Forearm* forearm, string label1, GLubyte color[3], float r = 0.15f, int slice = 20);
};


// 手
class Hand : public Component {
private:
	Wrist* wrist = nullptr;
	float ratio = 1.0f;     // x和z轴的比例，x轴方向为1
	float radius = 0.15f;
	float height = 0.1f;
	int accurate = 70;
	int slice = 20;

public:
	Hand(Wrist* wrist, string label1, GLubyte color[3], float r = 0.15f, float h = 0.1f,
		float ratio1 = 1.0f, int accurate1 = 70, int slice1 = 20);
	void Display() override;
};


