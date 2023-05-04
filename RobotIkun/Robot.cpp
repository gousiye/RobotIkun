#include "Robot.h"
#include "Component.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>

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
	this->AddComponet(body->getLabel(), body);
	/*-------------------------------------*/

	/*-----------生成背带裤-------------------*/
	SetPartProperty(partColor, (GLubyte)215, (GLubyte)215, (GLubyte)222);
	Strip* strip = new Strip(body, "Strip", partColor);
	this->AddComponet(strip->getLabel(), strip);
	/*---------------------------------------*/

	/*---------------生成头----------------*/
	SetPartProperty(partScale, 0.7f, 0.7f, 0.7f);
	SetPartProperty(partPosition, 0.0f, body->getScale()[1] + partScale[1] / 2.0f, 0.0f);
	SetPartProperty(partColor, (GLubyte)220, (GLubyte)184, (GLubyte)188);
	Head* head = new Head("Head", partPosition, partScale, partColor);
	this->AddComponet(head->getLabel(), head);
	/*--------------------------------------*/

	/*-----------------生成头发------------------*/
	SetPartProperty(partColor, (GLubyte)103, (GLubyte)95, (GLubyte)109);
	Hair* hair = new Hair(head, "Hair", partColor);
	this->AddComponet(hair->getLabel(), hair);
	/*------------------------------------------*/

	/*-----------------生成眼睛------------------*/
	SetPartRadius(partRadius, 1.0f / 8.0f * head->getScale()[0]);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	// 生成左眼, num = -1
	Eye* leftEye = new Eye(head, "LeftEye", partColor, -1, partRadius);
	this->AddComponet(leftEye->getLabel(), leftEye);
	//生成右眼，num = 1
	Eye* rightEye = new Eye(head, "RightEye", partColor, 1, partRadius);
	this->AddComponet(rightEye->getLabel(), rightEye);
	/*------------------------------------------*/

	/*-----------------生成嘴巴------------------*/
	SetPartRadius(partRadius, 1.0f / 4.0f * head->getScale()[0]);
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)115, (GLubyte)143);
	Mouth* mouth = new Mouth(head, "Mouth", partColor, partRadius);
	this->AddComponet(mouth->getLabel(), mouth);
	/*------------------------------------------*/


	/*-----------------生成髋关节------------------*/
	// float tmp = std::max(body->getScale()[2] / 2.0f, 1.0f / 6.0f * body->getScale()[0]);
	SetPartRadius(partRadius, body->getScale()[2] / 2.0f);
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	// 生成左髋关节
	Coxa* leftCoxa = new Coxa(body, "LeftCoxa", partColor, -1, partRadius);
	this->AddComponet(leftCoxa->getLabel(), leftCoxa);
	// 生成右髋关节
	Coxa* rightCoxa = new Coxa(body, "RightCoxa", partColor, 1, partRadius);
	this->AddComponet(rightCoxa->getLabel(), rightCoxa);
	/*------------------------------------------*/

	/*-----------------生成肩关节------------------*/
	SetPartRadius(partRadius, body->getScale()[2] / 2.0f);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)255);
	// 生成左肩关节
	Shoulder* leftShoulder = new Shoulder(body, "LeftShoudler", partColor, -1, partRadius);
	this->AddComponet(leftShoulder->getLabel(), leftShoulder);
	// 生成右肩关节
	Shoulder* rightShoulder = new Shoulder(body, "RightShoudler", partColor, 1, partRadius);
	this->AddComponet(rightShoulder->getLabel(), rightShoulder);
	/*----------------------------------------------*/


	/*-----------------生成左大腿------------------*/
	SetPartRadius(partRadius, leftCoxa->getR());
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)255);
	Thigh* leftThigh = new Thigh(leftCoxa, "LeftThigh", partColor,  partRadius, body->getScale()[1] * 0.5 + leftCoxa->getR());
	this->AddComponet(leftThigh->getLabel(), leftThigh);
	/*--------------------------------------------*/

	/*-----------------生成右大腿------------------*/
	SetPartRadius(partRadius, rightCoxa->getR());
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)255);
	Thigh* rightThigh = new Thigh(rightCoxa, "RightThigh", partColor, partRadius, body->getScale()[1] * 0.5 + rightCoxa->getR());
	this->AddComponet(rightThigh->getLabel(), rightThigh);
	/*--------------------------------------------*/

	/*-----------------生成左膝关节------------------*/
	SetPartRadius(partRadius, leftThigh->getRBottom());
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Knee* leftKnee = new Knee(leftThigh, "LeftKnee", partColor,  partRadius);
	this->AddComponet(leftKnee->getLabel(), leftKnee);
	/*--------------------------------------------*/

	/*-----------------生成右膝关节------------------*/
	SetPartRadius(partRadius, rightThigh->getRBottom());
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Knee* rightKnee = new Knee(rightThigh, "RighttKnee", partColor, partRadius);
	this->AddComponet(rightKnee->getLabel(), rightKnee);
	/*--------------------------------------------*/

	/*-----------------生成左小腿------------------*/
	SetPartRadius(partRadius, leftKnee->getR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Shank * leftShank = new Shank(leftKnee, "LeftShank", partColor, partRadius * 0.9, partRadius * 0.6, body->getScale()[1] * 0.5);
	this->AddComponet(leftShank->getLabel(), leftShank);
	/*--------------------------------------------*/

	/*-----------------生成右小腿------------------*/
	SetPartRadius(partRadius, rightKnee->getR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Shank* rightShank = new Shank(rightKnee, "RightShank", partColor, partRadius * 0.9, partRadius * 0.6, body->getScale()[1] * 0.5);
	this->AddComponet(rightShank->getLabel(), rightShank);
	/*--------------------------------------------*/

	/*-----------------生成左脚踝------------------*/
	SetPartRadius(partRadius, leftShank->getRBottom() * 0.8);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Ankle* leftAnkle = new Ankle(leftShank, "LeftAnkle", partColor, partRadius);
	this->AddComponet(leftAnkle->getLabel(), leftAnkle);
	/*--------------------------------------------*/

	/*-----------------生成右脚踝------------------*/
	SetPartRadius(partRadius, rightShank->getRBottom() * 0.8);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Ankle* rightAnkle = new Ankle(rightShank, "RightAnkle", partColor, partRadius);
	this->AddComponet(rightAnkle->getLabel(), rightAnkle);
	/*--------------------------------------------*/

	/*-----------------生成左脚------------------*/
	SetPartProperty(partScale, 1.5f * leftThigh->getRTop() , 0.05f, body->getScale()[2] * 1.2f);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)255);
	Foot* leftFoot = new Foot(leftAnkle, "LeftFoot", partScale, partColor);
	this->AddComponet(leftFoot->getLabel(), leftFoot);
	/*--------------------------------------------*/

	/*-----------------生成右脚------------------*/
	SetPartProperty(partScale, 1.5f * rightThigh->getRTop(), 0.05f, body->getScale()[2] * 1.2f);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)255);
	Foot* rightFoot = new Foot(rightAnkle, "RightFoot", partScale, partColor);
	this->AddComponet(rightFoot->getLabel(), rightFoot);
	/*--------------------------------------------*/

	/*-----------------生成左大臂------------------*/
	SetPartRadius(partRadius, 0.6 * leftShoulder->getR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Boom* leftBoom = new Boom(leftShoulder, "LeftBoom", partColor, partRadius, body->getScale()[1] * 0.25 + leftShoulder->getR());
	this->AddComponet(leftBoom->getLabel(), leftBoom);
	/*--------------------------------------------*/

	/*-----------------生成右大臂------------------*/
	SetPartRadius(partRadius, 0.6 * rightShoulder->getR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Boom* rightBoom = new Boom(rightShoulder, "RightBoom", partColor, partRadius, body->getScale()[1] * 0.25 + rightShoulder->getR());
	this->AddComponet(rightBoom->getLabel(), rightBoom);
	/*--------------------------------------------*/

	/*-----------------生成左肘关节------------------*/
	SetPartRadius(partRadius, leftShoulder->getR() * 0.8);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Elbow* leftElbow = new Elbow(leftBoom, "LeftElbow", partColor, partRadius);
	this->AddComponet(leftElbow->getLabel(), leftElbow);
	/*--------------------------------------------*/

	/*-----------------生成右肘关节------------------*/
	SetPartRadius(partRadius, rightShoulder->getR() * 0.8);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Elbow* rightElbow = new Elbow(rightBoom, "RightElbow", partColor, partRadius);
	this->AddComponet(rightElbow->getLabel(), rightElbow);
	/*--------------------------------------------*/


	/*-----------------生成左小臂------------------*/
	SetPartRadius(partRadius, 0.6 * leftShoulder->getR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Forearm* leftForearm = new Forearm(leftElbow, "LeftForearm", partColor, partRadius * 0.9, partRadius * 0.6, body->getScale()[1] * 0.35);
	this->AddComponet(leftForearm->getLabel(), leftForearm);
	/*--------------------------------------------*/


	/*-----------------生成右小臂-----------------*/
	SetPartRadius(partRadius, 0.6 * rightShoulder->getR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Forearm* rightForearm = new Forearm(rightElbow, "RightForearm", partColor, partRadius * 0.9, partRadius * 0.6, body->getScale()[1] * 0.35);
	this->AddComponet(rightForearm->getLabel(), rightForearm);
	/*--------------------------------------------*/

	/*-----------------生成左手腕------------------*/
	SetPartRadius(partRadius, leftForearm->getRBottom() * 0.9);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Wrist* leftWrist = new Wrist(leftForearm, "LeftWrist", partColor, partRadius);
	this->AddComponet(leftWrist->getLabel(), leftWrist);
	/*--------------------------------------------*/

	/*-----------------生成右手腕------------------*/
	SetPartRadius(partRadius, rightForearm->getRBottom() * 0.9);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Wrist* rightWrist = new Wrist(rightForearm, "RightWrist", partColor, partRadius);
	this->AddComponet(rightWrist->getLabel(), rightWrist);
	/*--------------------------------------------*/

	/*-----------------生成左手------------------*/
	SetPartRadius(partRadius, leftWrist->getR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Hand* leftHand = new Hand(leftWrist, "LeftHand", partColor, partRadius * 2, 1.5 * leftWrist->getR(), 0.5f);
	this->AddComponet(leftHand->getLabel(), leftHand);
	/*--------------------------------------------*/

	/*-----------------生成右手-------------------*/
	SetPartRadius(partRadius, rightWrist->getR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Hand* rightHand = new Hand(rightWrist, "RightHand", partColor, partRadius * 2, 1.5 * rightWrist->getR(), 0.5f);
	this->AddComponet(rightHand->getLabel(), rightHand);
	/*--------------------------------------------*/
}




void Robot::Display()
{
	// 遍历显示每个部件
	glPushMatrix();
	glTranslatef(this->x, this->y, this->z);    // 机器人当前的位置
	glRotatef(this->turn,0,1,0);   // 机器人当前的朝向
 	for (auto i : this->assembly) {
		// if(i.first == "LeftShank")
			i.second->Display();
	}
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


void Robot::Greeting() {
	std::cout << "this is hello" << std::endl;
}