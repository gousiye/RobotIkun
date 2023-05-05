#include "Robot.h"
#include "Component.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <cmath>

const float Robot::PI = 3.14159;

Robot::Robot(int newX, int newY, int newZ):x(newX), y(newY), z(newZ)
{
}

Robot::~Robot()
{
	for (auto i : this->assembly) {
		delete i.second;
	}
}

void Robot::AddComponet(const string& label, Component* part)
{
	if (this->assembly.count(label) == 0) {
		this->assembly[label] = part;
	}
}

template<typename T> 
void Robot::SetPartProperty(T* arr, T x, T y, T z) {
	arr[0] = x;
	arr[1] = y;
	arr[2] = z;
}

void Robot::SetPartRadius(float& r, float value) {
	r = value;
}

// 生成各部件
void Robot::Generate()
{
	float partPosition[3] = { 0,0,0 };
	float partScale[3] = { 1.4f, 2.0f, 0.5f };
	float partRadius = 0.0f;
	GLubyte partColor[3] = { 23, 24, 35 };

	/*--------------生成躯体----------------*/
	SetPartProperty(partScale, 1.4f, 2.0f, 0.5f);
	SetPartProperty(partPosition, 0.0f, partScale[1] / 2 + this->y, 0.0f);
	SetPartProperty(partColor, (GLubyte)23, (GLubyte)24, (GLubyte)35);
	Body* body = new Body("Body", partPosition, partScale, partColor);
	this->AddComponet(body->GetLabel(), body);
	/*-------------------------------------*/

	/*-----------生成背带裤-------------------*/
	SetPartProperty(partColor, (GLubyte)215, (GLubyte)215, (GLubyte)222);
	Strip* strip = new Strip(body, "Strip", partColor);
	this->AddComponet(strip->GetLabel(), strip);
	/*---------------------------------------*/

	/*---------------生成头----------------*/
	SetPartProperty(partScale, 0.7f, 0.7f, 0.7f);
	SetPartProperty(partPosition, 0.0f, partScale[1] / 2.0f, 0.0f);
	SetPartProperty(partColor, (GLubyte)220, (GLubyte)184, (GLubyte)188);
	Head* head = new Head("Head", body, partScale, partColor);
	this->AddComponet(head->GetLabel(), head);
	/*--------------------------------------*/

	/*-----------------生成头发------------------*/
	SetPartProperty(partColor, (GLubyte)103, (GLubyte)95, (GLubyte)109);
	Hair* hair = new Hair(head, "Hair", partColor);
	this->AddComponet(hair->GetLabel(), hair);
	/*------------------------------------------*/

	/*-----------------生成眼睛------------------*/
	SetPartRadius(partRadius, 1.0f / 8.0f * head->getScale()[0]);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	// 生成左眼, num = -1
	Eye* leftEye = new Eye(head, "LeftEye", partColor, -1, partRadius);
	this->AddComponet(leftEye->GetLabel(), leftEye);
	//生成右眼，num = 1
	Eye* rightEye = new Eye(head, "RightEye", partColor, 1, partRadius);
	this->AddComponet(rightEye->GetLabel(), rightEye);
	/*------------------------------------------*/

	/*-----------------生成嘴巴------------------*/
	SetPartRadius(partRadius, 1.0f / 4.0f * head->getScale()[0]);
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)115, (GLubyte)143);
	Mouth* mouth = new Mouth(head, "Mouth", partColor, partRadius);
	this->AddComponet(mouth->GetLabel(), mouth);
	/*------------------------------------------*/


	/*-----------------生成髋关节------------------*/
	// float tmp = std::max(body->getScale()[2] / 2.0f, 1.0f / 6.0f * body->getScale()[0]);
	SetPartRadius(partRadius, body->getScale()[2] / 2.0f);
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	// 生成左髋关节
	Coxa* leftCoxa = new Coxa(body, "LeftCoxa", partColor, -1, partRadius);
	this->AddComponet(leftCoxa->GetLabel(), leftCoxa);
	// 生成右髋关节
	Coxa* rightCoxa = new Coxa(body, "RightCoxa", partColor, 1, partRadius);
	this->AddComponet(rightCoxa->GetLabel(), rightCoxa);
	/*------------------------------------------*/

	/*-----------------生成肩关节------------------*/
	SetPartRadius(partRadius, body->getScale()[2] / 2.0f);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)255);
	// 生成左肩关节
	Shoulder* leftShoulder = new Shoulder(body, "LeftShoulder", partColor, -1, partRadius);
	this->AddComponet(leftShoulder->GetLabel(), leftShoulder);
	// 生成右肩关节
	Shoulder* rightShoulder = new Shoulder(body, "RightShoulder", partColor, 1, partRadius);
	this->AddComponet(rightShoulder->GetLabel(), rightShoulder);
	/*----------------------------------------------*/


	/*-----------------生成左大腿------------------*/
	SetPartRadius(partRadius, leftCoxa->GetR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)255, (GLubyte)255);
	Thigh* leftThigh = new Thigh(leftCoxa, "LeftThigh", partColor, partRadius, body->getScale()[1] * 0.5 + leftCoxa->GetR());
	this->AddComponet(leftThigh->GetLabel(), leftThigh);
	/*--------------------------------------------*/

	/*-----------------生成右大腿------------------*/
	SetPartRadius(partRadius, rightCoxa->GetR());
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)255);
	Thigh* rightThigh = new Thigh(rightCoxa, "RightThigh", partColor, partRadius, body->getScale()[1] * 0.5 + rightCoxa->GetR());
	this->AddComponet(rightThigh->GetLabel(), rightThigh);
	/*--------------------------------------------*/

	/*-----------------生成左膝关节------------------*/
	SetPartRadius(partRadius, leftThigh->GetRBottom());
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Knee* leftKnee = new Knee(leftThigh, "LeftKnee", partColor, partRadius);
	this->AddComponet(leftKnee->GetLabel(), leftKnee);
	/*--------------------------------------------*/

	/*-----------------生成右膝关节------------------*/
	SetPartRadius(partRadius, rightThigh->GetRBottom());
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Knee* rightKnee = new Knee(rightThigh, "RightKnee", partColor, partRadius);
	this->AddComponet(rightKnee->GetLabel(), rightKnee);
	/*--------------------------------------------*/

	/*-----------------生成左小腿------------------*/
	SetPartRadius(partRadius, leftKnee->GetR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Shank* leftShank = new Shank(leftKnee, "LeftShank", partColor, partRadius * 0.9, partRadius * 0.6, body->getScale()[1] * 0.5);
	this->AddComponet(leftShank->GetLabel(), leftShank);
	/*--------------------------------------------*/

	/*-----------------生成右小腿------------------*/
	SetPartRadius(partRadius, rightKnee->GetR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Shank* rightShank = new Shank(rightKnee, "RightShank", partColor, partRadius * 0.9, partRadius * 0.6, body->getScale()[1] * 0.5);
	this->AddComponet(rightShank->GetLabel(), rightShank);
	/*--------------------------------------------*/

	/*-----------------生成左脚踝------------------*/
	SetPartRadius(partRadius, leftShank->GetRBottom() * 0.8);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Ankle* leftAnkle = new Ankle(leftShank, "LeftAnkle", partColor, partRadius);
	this->AddComponet(leftAnkle->GetLabel(), leftAnkle);
	/*--------------------------------------------*/

	/*-----------------生成右脚踝------------------*/
	SetPartRadius(partRadius, rightShank->GetRBottom() * 0.8);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Ankle* rightAnkle = new Ankle(rightShank, "RightAnkle", partColor, partRadius);
	this->AddComponet(rightAnkle->GetLabel(), rightAnkle);
	/*--------------------------------------------*/

	/*-----------------生成左脚------------------*/
	SetPartProperty(partScale, 1.5f * leftThigh->GetRTop(), 0.05f, body->getScale()[2] * 1.2f);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)255);
	Foot* leftFoot = new Foot(leftAnkle, "LeftFoot", partScale, partColor);
	this->AddComponet(leftFoot->GetLabel(), leftFoot);
	/*--------------------------------------------*/

	/*-----------------生成右脚------------------*/
	SetPartProperty(partScale, 1.5f * rightThigh->GetRTop(), 0.05f, body->getScale()[2] * 1.2f);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)255);
	Foot* rightFoot = new Foot(rightAnkle, "RightFoot", partScale, partColor);
	this->AddComponet(rightFoot->GetLabel(), rightFoot);
	/*--------------------------------------------*/

	/*-----------------生成左大臂------------------*/
	SetPartRadius(partRadius, 0.6 * leftShoulder->GetR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Boom* leftBoom = new Boom(leftShoulder, "LeftBoom", partColor, partRadius, body->getScale()[1] * 0.25 + leftShoulder->GetR());
	this->AddComponet(leftBoom->GetLabel(), leftBoom);
	/*--------------------------------------------*/

	/*-----------------生成右大臂------------------*/
	SetPartRadius(partRadius, 0.6 * rightShoulder->GetR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Boom* rightBoom = new Boom(rightShoulder, "RightBoom", partColor, partRadius, body->getScale()[1] * 0.25 + rightShoulder->GetR());
	this->AddComponet(rightBoom->GetLabel(), rightBoom);
	/*--------------------------------------------*/

	/*-----------------生成左肘关节------------------*/
	SetPartRadius(partRadius, leftShoulder->GetR() * 0.8);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Elbow* leftElbow = new Elbow(leftBoom, "LeftElbow", partColor, partRadius);
	this->AddComponet(leftElbow->GetLabel(), leftElbow);
	/*--------------------------------------------*/

	/*-----------------生成右肘关节------------------*/
	SetPartRadius(partRadius, rightShoulder->GetR() * 0.8);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Elbow* rightElbow = new Elbow(rightBoom, "RightElbow", partColor, partRadius);
	this->AddComponet(rightElbow->GetLabel(), rightElbow);
	/*--------------------------------------------*/


	/*-----------------生成左小臂------------------*/
	SetPartRadius(partRadius, 0.6 * leftShoulder->GetR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Forearm* leftForearm = new Forearm(leftElbow, "LeftForearm", partColor, partRadius * 0.9, partRadius * 0.6, body->getScale()[1] * 0.35);
	this->AddComponet(leftForearm->GetLabel(), leftForearm);
	/*--------------------------------------------*/


	/*-----------------生成右小臂-----------------*/
	SetPartRadius(partRadius, 0.6 * rightShoulder->GetR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Forearm* rightForearm = new Forearm(rightElbow, "RightForearm", partColor, partRadius * 0.9, partRadius * 0.6, body->getScale()[1] * 0.35);
	this->AddComponet(rightForearm->GetLabel(), rightForearm);
	/*--------------------------------------------*/

	/*-----------------生成左手腕------------------*/
	SetPartRadius(partRadius, leftForearm->GetRBottom() * 0.9);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Wrist* leftWrist = new Wrist(leftForearm, "LeftWrist", partColor, partRadius);
	this->AddComponet(leftWrist->GetLabel(), leftWrist);
	/*--------------------------------------------*/

	/*-----------------生成右手腕------------------*/
	SetPartRadius(partRadius, rightForearm->GetRBottom() * 0.9);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Wrist* rightWrist = new Wrist(rightForearm, "RightWrist", partColor, partRadius);
	this->AddComponet(rightWrist->GetLabel(), rightWrist);
	/*--------------------------------------------*/

	/*-----------------生成左手------------------*/
	SetPartRadius(partRadius, leftWrist->GetR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Hand* leftHand = new Hand(leftWrist, "LeftHand", partColor, partRadius * 2, 1.5 * leftWrist->GetR(), 0.5f);
	this->AddComponet(leftHand->GetLabel(), leftHand);
	/*--------------------------------------------*/

	/*-----------------生成右手-------------------*/
	SetPartRadius(partRadius, rightWrist->GetR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Hand* rightHand = new Hand(rightWrist, "RightHand", partColor, partRadius * 2, 1.5 * rightWrist->GetR(), 0.5f);
	this->AddComponet(rightHand->GetLabel(), rightHand);
	/*-------------------------------------------*/



}


void Robot::Display()
{
	// 遍历显示每个部件
	glPushMatrix();
	glTranslatef(this->x, this->y, this->z);    // 机器人当前的位置
	glRotatef(this->turn, 0, 1, 0);   // 机器人当前的朝向
	assembly["Body"]->Display();    //通过身体绘制整个机器人
	glPopMatrix();
}


void Robot::ClockwiseRotate()
{
	turn = (turn + 360 - 5) % 360;
}

void Robot::AntiClockwiseRotate()
{
	turn = (turn + 5) % 360;
}

void Robot::WalkForward() {
	z = z + forwardRate * cos(GLfloat(turn) * PI / 180.0f);
	x = x + forwardRate * sin(GLfloat(turn) * PI / 180.0f);
	static int walkStep = -1;   // 表示走路该腿在前还是在后，-1在前
	float armLiftRate = armLiftLimit / legLiftLimit * legLiftRate; // 保持手腿同步
	auto leftShoulder = assembly["LeftShoulder"];
	auto rightShoulder = assembly["RightShoulder"];
	auto leftElbow = assembly["LeftElbow"];
	auto rightElbow = assembly["RightElbow"];
	auto leftCoxa = assembly["LeftCoxa"];
	auto leftKnee = assembly["LeftKnee"];
	auto rightCoxa = assembly["RightCoxa"];
	auto rightKnee = assembly["RightKnee"];
	auto body = assembly["Body"];

	if (leftCoxa->GetAngle() <= -this->legLiftLimit) {
		walkStep = 1;
	}
	if (leftCoxa->GetAngle() >= this->legLiftLimit) {
		walkStep = -1;
	}
	//左腿在身前
	if (leftCoxa->GetAngle() < 0) {
		leftShoulder->WholeRotate(-walkStep * armLiftRate, 1, 0, 0, 1);  
		rightShoulder->WholeRotate(walkStep * armLiftRate, 1, 0, 0, 1); 
		leftElbow->SetAngle(0);  // 保证每次走路是同样的，防止摆动角度没有正好到0导致角度逐渐错位
		rightElbow->WholeRotate(walkStep * armLiftRate * 1, 1, 0, 0, 1);
		leftCoxa->WholeRotate(walkStep * this->legLiftRate, 1, 0, 0, 1);
		rightCoxa->WholeRotate(-walkStep * this->legLiftRate, 1, 0, 0, 1);
		leftKnee->WholeRotate(-walkStep * this->legLiftRate * 0.7, 1, 0, 0, 1);
		rightKnee->SetAngle(0);

	}
	else {
		leftShoulder->WholeRotate(-walkStep * armLiftRate, 1, 0, 0, 1);
		rightShoulder->WholeRotate(walkStep * armLiftRate, 1, 0, 0, 1);
		leftElbow->WholeRotate(-walkStep * armLiftRate * 1, 1, 0, 0, 1);
		rightElbow->SetAngle(0);
		leftCoxa->WholeRotate(walkStep * this->legLiftRate, 1, 0, 0, 1);
		rightCoxa->WholeRotate(-walkStep * this->legLiftRate, 1, 0, 0, 1);
		leftKnee->SetAngle(0);
		rightKnee->WholeRotate(walkStep * this->legLiftRate * 0.7, 1, 0, 0, 1);
	}
	body->SetY(1 - body->GetY()* (1 - cos(leftCoxa->GetAngle() * PI / 180)));
}

void Robot::WalkBackward()
{
	z = z - forwardRate * cos(GLfloat(turn) * PI / 180.0f);
	x = x - forwardRate * sin(GLfloat(turn) * PI / 180.0f);
	static int walkStepb = 1;   // 表示走路该腿在前还是在后，1在后
	float armLiftRate = armLiftLimit / legLiftLimit * legLiftRate; 
	auto leftShoulder = assembly["LeftShoulder"];
	auto rightShoulder = assembly["RightShoulder"];
	auto leftElbow = assembly["LeftElbow"];
	auto rightElbow = assembly["RightElbow"];
	auto leftCoxa = assembly["LeftCoxa"];
	auto leftKnee = assembly["LeftKnee"];
	auto rightCoxa = assembly["RightCoxa"];
	auto rightKnee = assembly["RightKnee"];
	auto body = assembly["Body"];

	if (leftCoxa->GetAngle() >= this->legLiftLimit) {
		walkStepb = -1;
	}
	if (leftCoxa->GetAngle() <= -this->legLiftLimit) {
		walkStepb = 1;
	}
	//左腿在身后
	if (leftCoxa->GetAngle() < 0) {
		leftShoulder->WholeRotate(-walkStepb * armLiftRate, 1, 0, 0, 1);
		rightShoulder->WholeRotate(walkStepb * armLiftRate, 1, 0, 0, 1);
		leftElbow->SetAngle(0);  
		rightElbow->WholeRotate(walkStepb * armLiftRate * 1, 1, 0, 0, 1);
		leftCoxa->WholeRotate(walkStepb * this->legLiftRate, 1, 0, 0, 1);
		rightCoxa->WholeRotate(-walkStepb * this->legLiftRate, 1, 0, 0, 1);
		leftKnee->WholeRotate(-walkStepb * this->legLiftRate * 0.7, 1, 0, 0, 1);
		rightKnee->SetAngle(0);

	}
	else {
		leftShoulder->WholeRotate(-walkStepb * armLiftRate, 1, 0, 0, 1);
		rightShoulder->WholeRotate(walkStepb * armLiftRate, 1, 0, 0, 1);
		leftElbow->WholeRotate(-walkStepb * armLiftRate * 1, 1, 0, 0, 1);
		rightElbow->SetAngle(0);
		leftCoxa->WholeRotate(walkStepb * this->legLiftRate, 1, 0, 0, 1);
		rightCoxa->WholeRotate(-walkStepb * this->legLiftRate, 1, 0, 0, 1);
		leftKnee->SetAngle(0);
		rightKnee->WholeRotate(walkStepb * this->legLiftRate * 0.7, 1, 0, 0, 1);
	}
	body->SetY(1 - body->GetY() * (1 - cos(leftCoxa->GetAngle() * PI / 180)));
}

void Robot::Greeting() {
	std::cout << "this is hello" << std::endl;
}