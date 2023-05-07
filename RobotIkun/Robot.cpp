#include "Robot.h"
#include "Component.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <Windows.h>
#include "MathOperation.h"
#include <mutex>

const float Robot::PI = 3.14159;

Robot::Robot(int newX, int newY, int newZ):x(newX), y(newY), z(newZ)
{
	std::unique_lock<std::mutex> nothing1(m);   // unique_lock ����mutex��Ҫ��Ȼ��������
	nothing = std::move(nothing1);
}

Robot::~Robot()
{
	for (auto i : this->assembly) {
		delete i.second;
	}
}

void Robot::SetWindowDisplay(void(*displayFunc)())
{
	this->windowDisplay = displayFunc;
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

// ���ɸ�����
void Robot::Generate()
{
	float partPosition[3] = { 0,0,0 };
	float partScale[3] = { 1.4f, 2.0f, 0.5f };
	float partRadius = 0.0f;
	GLubyte partColor[3] = { 23, 24, 35 };

	/*--------------��������----------------*/
	SetPartProperty(partScale, 1.4f, 2.0f, 0.5f);
	SetPartProperty(partPosition, 0.0f, partScale[1] / 2 + this->y, 0.0f);
	SetPartProperty(partColor, (GLubyte)23, (GLubyte)24, (GLubyte)35);
	Body* body = new Body("Body", partPosition, partScale, partColor);
	this->AddComponet(body->GetLabel(), body);
	/*-------------------------------------*/

	/*-----------���ɱ�����-------------------*/
	SetPartProperty(partColor, (GLubyte)215, (GLubyte)215, (GLubyte)222);
	Strip* strip = new Strip(body, "Strip", partColor);
	this->AddComponet(strip->GetLabel(), strip);
	/*---------------------------------------*/

	/*---------------����ͷ----------------*/
	SetPartProperty(partScale, 0.7f, 0.7f, 0.7f);
	SetPartProperty(partPosition, 0.0f, partScale[1] / 2.0f, 0.0f);
	SetPartProperty(partColor, (GLubyte)220, (GLubyte)184, (GLubyte)188);
	Head* head = new Head("Head", body, partScale, partColor);
	this->AddComponet(head->GetLabel(), head);
	/*--------------------------------------*/

	/*-----------------����ͷ��------------------*/
	SetPartProperty(partColor, (GLubyte)103, (GLubyte)95, (GLubyte)109);
	Hair* hair = new Hair(head, "Hair", partColor);
	this->AddComponet(hair->GetLabel(), hair);
	/*------------------------------------------*/

	/*-----------------�����۾�------------------*/
	SetPartRadius(partRadius, 1.0f / 8.0f * head->getScale()[0]);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	// ��������, num = -1
	Eye* leftEye = new Eye(head, "LeftEye", partColor, -1, partRadius);
	this->AddComponet(leftEye->GetLabel(), leftEye);
	//�������ۣ�num = 1
	Eye* rightEye = new Eye(head, "RightEye", partColor, 1, partRadius);
	this->AddComponet(rightEye->GetLabel(), rightEye);
	/*------------------------------------------*/

	/*-----------------�������------------------*/
	SetPartRadius(partRadius, 1.0f / 4.0f * head->getScale()[0]);
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)115, (GLubyte)143);
	Mouth* mouth = new Mouth(head, "Mouth", partColor, partRadius);
	this->AddComponet(mouth->GetLabel(), mouth);
	/*------------------------------------------*/


	/*-----------------�����Źؽ�------------------*/
	// float tmp = std::max(body->getScale()[2] / 2.0f, 1.0f / 6.0f * body->getScale()[0]);
	SetPartRadius(partRadius, body->getScale()[2] / 2.0f);
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	// �������Źؽ�
	Coxa* leftCoxa = new Coxa(body, "LeftCoxa", partColor, -1, partRadius);
	this->AddComponet(leftCoxa->GetLabel(), leftCoxa);
	// �������Źؽ�
	Coxa* rightCoxa = new Coxa(body, "RightCoxa", partColor, 1, partRadius);
	this->AddComponet(rightCoxa->GetLabel(), rightCoxa);
	/*------------------------------------------*/

	/*-----------------���ɼ�ؽ�------------------*/
	SetPartRadius(partRadius, body->getScale()[2] / 2.0f);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)255);
	// �������ؽ�
	Shoulder* leftShoulder = new Shoulder(body, "LeftShoulder", partColor, -1, partRadius);
	this->AddComponet(leftShoulder->GetLabel(), leftShoulder);
	// �����Ҽ�ؽ�
	Shoulder* rightShoulder = new Shoulder(body, "RightShoulder", partColor, 1, partRadius);
	this->AddComponet(rightShoulder->GetLabel(), rightShoulder);
	/*----------------------------------------------*/


	/*-----------------���������------------------*/
	SetPartRadius(partRadius, leftCoxa->GetR());
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)255);
	Thigh* leftThigh = new Thigh(leftCoxa, "LeftThigh", partColor, partRadius, body->getScale()[1] * 0.5 + leftCoxa->GetR());
	this->AddComponet(leftThigh->GetLabel(), leftThigh);
	/*--------------------------------------------*/

	/*-----------------�����Ҵ���------------------*/
	SetPartRadius(partRadius, rightCoxa->GetR());
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)255);
	Thigh* rightThigh = new Thigh(rightCoxa, "RightThigh", partColor, partRadius, body->getScale()[1] * 0.5 + rightCoxa->GetR());
	this->AddComponet(rightThigh->GetLabel(), rightThigh);
	/*--------------------------------------------*/

	/*-----------------������ϥ�ؽ�------------------*/
	SetPartRadius(partRadius, leftThigh->GetRBottom());
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Knee* leftKnee = new Knee(leftThigh, "LeftKnee", partColor, partRadius);
	this->AddComponet(leftKnee->GetLabel(), leftKnee);
	/*--------------------------------------------*/

	/*-----------------������ϥ�ؽ�------------------*/
	SetPartRadius(partRadius, rightThigh->GetRBottom());
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Knee* rightKnee = new Knee(rightThigh, "RightKnee", partColor, partRadius);
	this->AddComponet(rightKnee->GetLabel(), rightKnee);
	/*--------------------------------------------*/

	/*-----------------������С��------------------*/
	SetPartRadius(partRadius, leftKnee->GetR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Shank* leftShank = new Shank(leftKnee, "LeftShank", partColor, partRadius * 0.9, partRadius * 0.6, body->getScale()[1] * 0.5);
	this->AddComponet(leftShank->GetLabel(), leftShank);
	/*--------------------------------------------*/

	/*-----------------������С��------------------*/
	SetPartRadius(partRadius, rightKnee->GetR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Shank* rightShank = new Shank(rightKnee, "RightShank", partColor, partRadius * 0.9, partRadius * 0.6, body->getScale()[1] * 0.5);
	this->AddComponet(rightShank->GetLabel(), rightShank);
	/*--------------------------------------------*/

	/*-----------------���������------------------*/
	SetPartRadius(partRadius, leftShank->GetRBottom() * 0.8);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Ankle* leftAnkle = new Ankle(leftShank, "LeftAnkle", partColor, partRadius);
	this->AddComponet(leftAnkle->GetLabel(), leftAnkle);
	/*--------------------------------------------*/

	/*-----------------�����ҽ���------------------*/
	SetPartRadius(partRadius, rightShank->GetRBottom() * 0.8);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Ankle* rightAnkle = new Ankle(rightShank, "RightAnkle", partColor, partRadius);
	this->AddComponet(rightAnkle->GetLabel(), rightAnkle);
	/*--------------------------------------------*/

	/*-----------------�������------------------*/
	SetPartProperty(partScale, 1.5f * leftThigh->GetRTop(), 0.05f, body->getScale()[2] * 1.2f);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)255);
	Foot* leftFoot = new Foot(leftAnkle, "LeftFoot", partScale, partColor);
	this->AddComponet(leftFoot->GetLabel(), leftFoot);
	/*--------------------------------------------*/

	/*-----------------�����ҽ�------------------*/
	SetPartProperty(partScale, 1.5f * rightThigh->GetRTop(), 0.05f, body->getScale()[2] * 1.2f);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)255);
	Foot* rightFoot = new Foot(rightAnkle, "RightFoot", partScale, partColor);
	this->AddComponet(rightFoot->GetLabel(), rightFoot);
	/*--------------------------------------------*/

	/*-----------------��������------------------*/
	SetPartRadius(partRadius, 0.6 * leftShoulder->GetR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Boom* leftBoom = new Boom(leftShoulder, "LeftBoom", partColor, partRadius, body->getScale()[1] * 0.25 + leftShoulder->GetR());
	this->AddComponet(leftBoom->GetLabel(), leftBoom);
	/*--------------------------------------------*/

	/*-----------------�����Ҵ��------------------*/
	SetPartRadius(partRadius, 0.6 * rightShoulder->GetR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Boom* rightBoom = new Boom(rightShoulder, "RightBoom", partColor, partRadius, body->getScale()[1] * 0.25 + rightShoulder->GetR());
	this->AddComponet(rightBoom->GetLabel(), rightBoom);
	/*--------------------------------------------*/

	/*-----------------��������ؽ�------------------*/
	SetPartRadius(partRadius, leftShoulder->GetR() * 0.8);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Elbow* leftElbow = new Elbow(leftBoom, "LeftElbow", partColor, partRadius);
	this->AddComponet(leftElbow->GetLabel(), leftElbow);
	/*--------------------------------------------*/

	/*-----------------��������ؽ�------------------*/
	SetPartRadius(partRadius, rightShoulder->GetR() * 0.8);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Elbow* rightElbow = new Elbow(rightBoom, "RightElbow", partColor, partRadius);
	this->AddComponet(rightElbow->GetLabel(), rightElbow);
	/*--------------------------------------------*/


	/*-----------------������С��------------------*/
	SetPartRadius(partRadius, 0.6 * leftShoulder->GetR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Forearm* leftForearm = new Forearm(leftElbow, "LeftForearm", partColor, partRadius * 0.9, partRadius * 0.6, body->getScale()[1] * 0.35);
	this->AddComponet(leftForearm->GetLabel(), leftForearm);
	/*--------------------------------------------*/


	/*-----------------������С��-----------------*/
	SetPartRadius(partRadius, 0.6 * rightShoulder->GetR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Forearm* rightForearm = new Forearm(rightElbow, "RightForearm", partColor, partRadius * 0.9, partRadius * 0.6, body->getScale()[1] * 0.35);
	this->AddComponet(rightForearm->GetLabel(), rightForearm);
	/*--------------------------------------------*/

	/*-----------------����������------------------*/
	SetPartRadius(partRadius, leftForearm->GetRBottom() * 0.9);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Wrist* leftWrist = new Wrist(leftForearm, "LeftWrist", partColor, partRadius);
	this->AddComponet(leftWrist->GetLabel(), leftWrist);
	/*--------------------------------------------*/

	/*-----------------����������------------------*/
	SetPartRadius(partRadius, rightForearm->GetRBottom() * 0.9);
	SetPartProperty(partColor, (GLubyte)0, (GLubyte)0, (GLubyte)0);
	Wrist* rightWrist = new Wrist(rightForearm, "RightWrist", partColor, partRadius);
	this->AddComponet(rightWrist->GetLabel(), rightWrist);
	/*--------------------------------------------*/

	/*-----------------��������------------------*/
	SetPartRadius(partRadius, leftWrist->GetR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Hand* leftHand = new Hand(leftWrist, "LeftHand", partColor, partRadius * 2, 1.5 * leftWrist->GetR(), 0.5f);
	this->AddComponet(leftHand->GetLabel(), leftHand);
	/*--------------------------------------------*/

	/*-----------------��������-------------------*/
	SetPartRadius(partRadius, rightWrist->GetR());
	SetPartProperty(partColor, (GLubyte)255, (GLubyte)0, (GLubyte)0);
	Hand* rightHand = new Hand(rightWrist, "RightHand", partColor, partRadius * 2, 1.5 * rightWrist->GetR(), 0.5f);
	this->AddComponet(rightHand->GetLabel(), rightHand);
	/*-------------------------------------------*/

}

void Robot::Display()
{
	// ������ʾÿ������
	glPushMatrix();
	glTranslatef(this->x, this->y, this->z);    // �����˵�ǰ��λ��
	glRotatef(this->turn, 0, 1, 0);   // �����˵�ǰ�ĳ���
	assembly["Body"]->Display();    //ͨ�������������������
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
	static int walkStep = -1;   // ��ʾ��·������ǰ�����ں�-1��ǰ
	float armLiftRate = armLiftLimit / legLiftLimit * legLiftRate; // ��������ͬ��
	auto leftShoulder = assembly["LeftShoulder"];
	auto rightShoulder = assembly["RightShoulder"];
	auto leftElbow = assembly["LeftElbow"];
	auto rightElbow = assembly["RightElbow"];
	auto leftCoxa = assembly["LeftCoxa"];
	auto leftKnee = assembly["LeftKnee"];
	auto rightCoxa = assembly["RightCoxa"];
	auto rightKnee = assembly["RightKnee"];
	Body* body = (Body*)assembly["Body"];

	if (leftCoxa->GetAngle() <= -this->legLiftLimit) {
		walkStep = 1;
	}
	if (leftCoxa->GetAngle() >= this->legLiftLimit) {
		walkStep = -1;
	}
	//��������ǰ
	if (leftCoxa->GetAngle() < 0) {
		leftShoulder->WholeRotate(-walkStep * armLiftRate, 1, 0, 0, 1);  
		rightShoulder->WholeRotate(walkStep * armLiftRate, 1, 0, 0, 1); 
		leftElbow->SetCurrAngle(0);  // ��֤ÿ����·��ͬ���ģ���ֹ�ڶ��Ƕ�û�����õ�0���½Ƕ��𽥴�λ
		rightElbow->WholeRotate(walkStep * armLiftRate * 1, 1, 0, 0, 1);
		leftCoxa->WholeRotate(walkStep * this->legLiftRate, 1, 0, 0, 1);
		rightCoxa->WholeRotate(-walkStep * this->legLiftRate, 1, 0, 0, 1);
		leftKnee->WholeRotate(-walkStep * this->legLiftRate * 0.7, 1, 0, 0, 1);
		rightKnee->SetCurrAngle(0);

	}
	else {
		leftShoulder->WholeRotate(-walkStep * armLiftRate, 1, 0, 0, 1);
		rightShoulder->WholeRotate(walkStep * armLiftRate, 1, 0, 0, 1);
		leftElbow->WholeRotate(-walkStep * armLiftRate * 1, 1, 0, 0, 1);
		rightElbow->SetCurrAngle(0);
		leftCoxa->WholeRotate(walkStep * this->legLiftRate, 1, 0, 0, 1);
		rightCoxa->WholeRotate(-walkStep * this->legLiftRate, 1, 0, 0, 1);
		leftKnee->SetCurrAngle(0);
		rightKnee->WholeRotate(walkStep * this->legLiftRate * 0.7, 1, 0, 0, 1);
	}
	body->SetY(body->GetInitY() - body->getScale()[1]* (1 - cos(leftCoxa->GetAngle() * PI / 180)));
	//body->Translate(0, -body->getScale()[1] * (1 - cos(leftCoxa->GetAngle() * PI / 180)), 0);
}

void Robot::WalkBackward()
{
	z = z - forwardRate * cos(GLfloat(turn) * PI / 180.0f);
	x = x - forwardRate * sin(GLfloat(turn) * PI / 180.0f);
	static int walkStepb = 1;   // ��ʾ��·������ǰ�����ں�1�ں�
	float armLiftRate = armLiftLimit / legLiftLimit * legLiftRate; 
	auto leftShoulder = assembly["LeftShoulder"];
	auto rightShoulder = assembly["RightShoulder"];
	auto leftElbow = assembly["LeftElbow"];
	auto rightElbow = assembly["RightElbow"];
	auto leftCoxa = assembly["LeftCoxa"];
	auto leftKnee = assembly["LeftKnee"];
	auto rightCoxa = assembly["RightCoxa"];
	auto rightKnee = assembly["RightKnee"];
	Body* body = (Body*)assembly["Body"];

	if (leftCoxa->GetAngle() >= this->legLiftLimit) {
		walkStepb = -1;
	}
	if (leftCoxa->GetAngle() <= -this->legLiftLimit) {
		walkStepb = 1;
	}
	//���������
	if (leftCoxa->GetAngle() < 0) {
		leftShoulder->WholeRotate(-walkStepb * armLiftRate, 1, 0, 0, 1);
		rightShoulder->WholeRotate(walkStepb * armLiftRate, 1, 0, 0, 1);
		leftElbow->SetCurrAngle(0);  
		rightElbow->WholeRotate(walkStepb * armLiftRate * 1, 1, 0, 0, 1);
		leftCoxa->WholeRotate(walkStepb * this->legLiftRate, 1, 0, 0, 1);
		rightCoxa->WholeRotate(-walkStepb * this->legLiftRate, 1, 0, 0, 1);
		leftKnee->WholeRotate(-walkStepb * this->legLiftRate * 0.7, 1, 0, 0, 1);
		rightKnee->SetCurrAngle(0);

	}
	else {
		leftShoulder->WholeRotate(-walkStepb * armLiftRate, 1, 0, 0, 1);
		rightShoulder->WholeRotate(walkStepb * armLiftRate, 1, 0, 0, 1);
		leftElbow->WholeRotate(-walkStepb * armLiftRate * 1, 1, 0, 0, 1);
		rightElbow->SetCurrAngle(0);
		leftCoxa->WholeRotate(walkStepb * this->legLiftRate, 1, 0, 0, 1);
		rightCoxa->WholeRotate(-walkStepb * this->legLiftRate, 1, 0, 0, 1);
		leftKnee->SetCurrAngle(0);
		rightKnee->WholeRotate(walkStepb * this->legLiftRate * 0.7, 1, 0, 0, 1);
	}
	body->SetY(body->GetInitY() - body->getScale()[1] * (1 - cos(leftCoxa->GetAngle() * PI / 180)));
}

void Robot::TurnFrontBack(bool toward) {



	if (this->turn <= 180) {
		while (this->turn <= 180) {
			cv.wait(nothing, [this] {return !this->IsPause(); });
			if (toward) this->ClockwiseRotate();
			else this->AntiClockwiseRotate();
			this->WindowDisplay();
			Sleep(10);
		}
		turn = 180;
	}
	else {
		while (this->turn >= 180) {
			cv.wait(nothing, [this] {return !this->IsPause(); });
			// cv.wait(nothing, [this] {return !this->IsPause(); });
			if (toward) this->AntiClockwiseRotate();
			else this->ClockwiseRotate();
			this->WindowDisplay();
			Sleep(10);
		}
		turn = 180;
	}
}

void Robot::WalkStand()
{
	auto leftCoxa = this->assembly["LeftCoxa"];
	auto rightCoxa = this->assembly["RightCoxa"];
	float orignAngle = leftCoxa->GetAngle();
	while (orignAngle * leftCoxa->GetAngle() > 0) {
		cv.wait(nothing, [this] {return !this->IsPause(); });
		this->WalkBackward();
		this->WindowDisplay();
		Sleep(20);
	}
	rightCoxa->SetCurrAngle(0);
	leftCoxa->SetCurrAngle(0);
}

void Robot::HorseStep()
{
	auto leftShoulder = this->assembly["LeftShoulder"];
	auto rightShoulder = this->assembly["RightShoulder"];
	auto leftElbow = this->assembly["LeftElbow"];
	auto rightElbow = this->assembly["RightElbow"];
	auto leftCoxa = this->assembly["LeftCoxa"];
	Thigh* leftThigh = (Thigh*)this->assembly["LeftThigh"];
	auto rightCoxa = this->assembly["RightCoxa"];
	Knee* leftKnee = (Knee*)this->assembly["LeftKnee"];
	auto rightKnee = this->assembly["RightKnee"];
	auto body = this->assembly["Body"];

	// ����ֻ�����˴��������������½���С�ȵ����̫������
	float posThigh[3] = { 0, -leftThigh->GetH(), 0 };
	float newPosThigh[3] = { 0, 0, 0 };
	while (rightCoxa->GetAngle() <= this->HorseStepLimit) {
		cv.wait(nothing, [this] {return !this->IsPause(); });
		leftShoulder->WholeRotate(-this->HorseStepRate * 0.5, 1, 0.5, -1, 1);
		leftElbow->WholeRotate(-this->HorseStepRate * 3, 1, 2, 0, 1);

		rightShoulder->WholeRotate(this->HorseStepRate * 0.5, -1, 0.5, -1, 1);
		rightElbow->WholeRotate(this->HorseStepRate * 3, -1, 2, 0, 1);
		rightCoxa->WholeRotate(this->HorseStepRate, -1, 0.5, -1, 1);
		leftCoxa->WholeRotate(-this->HorseStepRate, 1, 0.5, -1, 1);
		float axis[3] = { 1, 0.5 , -1 };
		MathOperation::RotateAnyAxis(posThigh, axis, -this->HorseStepRate, newPosThigh);
		body->SetY(body->GetY() + posThigh[1] - newPosThigh[1]);
		this->HorseStepHeight.push(newPosThigh[1] - posThigh[1]);  //ѹջ���ڻָ�
		posThigh[0] = newPosThigh[0];
		posThigh[1] = newPosThigh[1];
		posThigh[2] = newPosThigh[2];
		rightKnee->WholeRotate(-this->HorseStepRate, -1, 0.5, -1, 1);
		leftKnee->WholeRotate(this->HorseStepRate, 1, 0.5, -1, 1);
		body->WholeRotate(this->HorseStepRate / 3, 1, 0, 0, 1);
		this->WindowDisplay();
		Sleep(20);
	}
}

// ��������ѧ����֪ʶ��û�о������㣬ֻ��ƾ�о�����, �������ܲ���׼
void Robot::Swing() {
	auto leftCoxa = this->assembly["LeftCoxa"];
	auto rightCoxa = this->assembly["RightCoxa"];
	auto leftKnee = this->assembly["LeftKnee"];
	auto rightKnee = this->assembly["RightKnee"];
	auto body = this->assembly["Body"];
	auto rightElbow = this->assembly["RightElbow"];


	Sleep(50);
	bool isFirst = 1;
	// ����
	while (isFirst || abs(body->GetAngle()) <= this->SwingLimit) {
		cv.wait(nothing, [this] {return !this->IsPause(); });
		isFirst = 0;
		body->WholeRotate(1, 0, 0, 1, 1);  //����ת��
		rightCoxa->WholeRotate(-0.8, 0, 0, 1, 1);
		leftCoxa->WholeRotate(-1.5, 0, 0, 1, 1);
		leftKnee->WholeRotate(2, -2, 1, 0, 1);
		this->WindowDisplay();
		Sleep(20);
	}
	Sleep(50);

	// ��ʼ״̬�������϶�
	isFirst = 1;
	float moveVector[3] = { 0, 0, 0 };
	float originVector[3] = { 0, 0, 0 };
	while (isFirst || abs(leftCoxa->GetAngle()) <= this->SwingLimit) {
		cv.wait(nothing, [this] {return !this->IsPause(); });
		isFirst = 0;
		leftCoxa->WholeRotate(-3, 0, 1, 1, 1);
		leftKnee->WholeRotate(-this->HorseStepRate, 1, 0.5, -1, 1);
		rightCoxa->WholeRotate(5, 0, 1, 1, 1);
		rightKnee->WholeRotate(this->HorseStepRate, -1, 0.5, -1, 1);
		body->Translate(-0.03, 0.05, 0);
		this->WindowDisplay();
		Sleep(20);
	}
	originVector[0] = body->GetX();
	originVector[1] = body->GetY();
	originVector[2] = body->GetZ();
	Sleep(50);

	// ��Ϊ��׼����ÿ�����ϽǶ�����Ҫͬ������Щ��ת����
	leftCoxa->SaveRotateMatrix(leftCoxa->GetAngle());
	rightCoxa->SaveRotateMatrix(rightCoxa->GetAngle());
	leftKnee->SaveRotateMatrix(leftKnee->GetAngle());
	rightKnee->SaveRotateMatrix(rightKnee->GetAngle());

	// int m = 2;
	while (1) {
		// �����»�
		isFirst = 1;
		while (isFirst || abs(leftCoxa->GetAngle()) <= this->SwingLimit) {
			cv.wait(nothing, [this] {return !this->IsPause(); });
			isFirst = 0;
			leftCoxa->WholeRotate(3, 0, 1, 1, 1);
			leftKnee->WholeRotate(this->HorseStepRate, 1, 0.5, -1, 1);
			rightCoxa->WholeRotate(-5, 0, 1, 1, 1);
			rightKnee->WholeRotate(-this->HorseStepRate, -1, 0.5, -1, 1);
			body->Translate(0.05, -0.06, 0);
			this->WindowDisplay();
			Sleep(20);
		}
		Sleep(50);

		// �����¶�
		isFirst = 1;
		while (isFirst || abs(leftCoxa->GetAngle()) <= 3 * this->SwingLimit) {
			cv.wait(nothing, [this] {return !this->IsPause(); });
			isFirst = 0;
			leftCoxa->WholeRotate(3.5, 0.5, 1, 0, 1);
			leftKnee->WholeRotate(-this->HorseStepRate * 1.4, 1, 0.5, -1, 1);
			rightCoxa->WholeRotate(-1.5, 0, 0, 1, 1);
			rightKnee->WholeRotate(this->HorseStepRate * 0.3, -1, 0.5, -1, 1);
			body->Translate(-0.06, -0.02, 0);
			this->WindowDisplay();
			Sleep(20);
		}

		Sleep(50);
		// �����϶�
		isFirst = 1;
		while (isFirst || abs(leftCoxa->GetAngle()) <= this->SwingLimit) {
			cv.wait(nothing, [this] {return !this->IsPause(); });
			isFirst = 0;
			leftCoxa->WholeRotate(-3.5f / 3, 0.5, 0, -3, 1);
			leftKnee->WholeRotate(this->HorseStepRate * 0.5, 1, 0.5, -1, 1);
			rightCoxa->WholeRotate(2, 0, 0, 1, 1);
			//rightKnee->WholeRotate(-* this->HorseStepRate , -1, 0.5, -1, 1);
			body->Translate(0.06, 0.02, 0);
			this->WindowDisplay();
			Sleep(20);
		}

		// �����϶�, body�͸�����������ת����Ҫ����һ�����϶�����ͬ����֤���׶�����һֱ�ظ���ȥ
		isFirst = 1;
		moveVector[0] = originVector[0] - body->GetX();
		moveVector[1] = originVector[1] - body->GetY();
		moveVector[2] = originVector[2] - body->GetZ();
		float step = 0.1;
		while (1) {
			cv.wait(nothing, [this] {return !this->IsPause(); });
			isFirst = 0;
			leftCoxa->WholeRotate(-4, -1, 0, 2, 1);
			leftKnee->WholeRotate(-this->HorseStepRate * 0.5, 1, 0.5, -1, 1);
			rightCoxa->WholeRotate(3, 1, 0, 1, 1);
			rightKnee->WholeRotate(this->HorseStepRate * 0.5, -1, 0.5, -1, 1);
			body->Translate(moveVector[0] * step, moveVector[1] * step, moveVector[2] * step);
			if (abs(originVector[1] - body->GetY()) <= 0.1) break;
			this->WindowDisplay();
			Sleep(20);
		}

		leftCoxa->Trace2Saved();
		rightCoxa->Trace2Saved();
		leftKnee->Trace2Saved();
		rightKnee->Trace2Saved();
		this->WindowDisplay();

		if (this->stopSwing) {
			this->stopSwing = 0;
			return;
		}
	}
}

void Robot::ResetSwing() {
	bool isFirst = 1;
	auto leftCoxa = this->assembly["LeftCoxa"];
	auto rightCoxa = this->assembly["RightCoxa"];
	auto leftKnee = this->assembly["LeftKnee"];
	auto rightKnee = this->assembly["RightKnee"];
	auto body = this->assembly["Body"];
	auto rightElbow = this->assembly["RightElbow"];


	// �ظ���������̬
	while (isFirst || abs(leftCoxa->GetAngle()) >= 0.0001) {
		cv.wait(nothing, [this] {return !this->IsPause(); });
		body->Translate(0.03, -0.05, 0);
		isFirst = 0;
		rightKnee->WholeRotate(-this->HorseStepRate, -1, 0.5, -1, 1);
		rightCoxa->WholeRotate(-5, 0, 1, 1, 1);
		leftKnee->WholeRotate(this->HorseStepRate, 1, 0.5, -1, 1);
		leftCoxa->WholeRotate(3, 0, 1, 1, 1);
		this->WindowDisplay();
		Sleep(20);
	}


	Component* container[5] = { leftCoxa, leftKnee, rightCoxa, rightKnee, body };
	for (Component* i : container) {
		while (abs(i->GetAngle()) <= 0.01) i->PopLastRotate();
	}

	Sleep(50);


	// �ظ���������̬
	isFirst = 1;
	while (isFirst || abs(body->GetAngle()) >= 0.0001) {
		cv.wait(nothing, [this] {return !this->IsPause(); });
		isFirst = 0;
		leftKnee->WholeRotate(-2, -2, 1, 0, 1);
		leftCoxa->WholeRotate(1.5, 0, 0, 1, 1);
		rightCoxa->WholeRotate(0.8, 0, 0, 1, 1);
		body->WholeRotate(-1, 0, 0, 1, 1);
		this->WindowDisplay();
		Sleep(20);
	}

	for (Component* i : container) {
		while (abs(i->GetAngle()) <= 0.01) i->PopLastRotate();
	}

	Sleep(50);
}

void Robot::ResetHorseStep()
{
	auto leftCoxa = this->assembly["LeftCoxa"];
	auto rightCoxa = this->assembly["RightCoxa"];
	auto leftKnee = this->assembly["LeftKnee"];
	auto rightKnee = this->assembly["RightKnee"];
	auto body = this->assembly["Body"];
	auto leftShoulder = this->assembly["LeftShoulder"];
	auto rightShoulder = this->assembly["RightShoulder"];
	auto leftElbow = this->assembly["LeftElbow"];
	auto rightElbow = this->assembly["RightElbow"];
	//std::cout << rightCoxa->GetRotateX() << "   " << rightCoxa->GetRotateY() << "   " << rightCoxa->GetRotateZ() << "     " << rightCoxa->GetAngle() << std::endl;

	while (rightCoxa->GetAngle() > 0) {
		cv.wait(nothing, [this] {return !this->IsPause(); });
		body->WholeRotate(-this->HorseStepRate / 3, body->GetRotateX(), body->GetRotateY(), body->GetRotateZ(), 1);

		leftShoulder->WholeRotate(this->HorseStepRate * 0.5, leftShoulder->GetRotateX(), leftShoulder->GetRotateY(), leftShoulder->GetRotateZ(), 1);
		leftElbow->WholeRotate(this->HorseStepRate * 3, leftElbow->GetRotateX(), leftElbow->GetRotateY(), leftElbow->GetRotateZ(), 1);

		rightShoulder->WholeRotate(-this->HorseStepRate * 0.5, rightShoulder->GetRotateX(), rightShoulder->GetRotateY(), rightShoulder->GetRotateZ(), 1);
		rightElbow->WholeRotate(-this->HorseStepRate * 3, rightElbow->GetRotateX(), rightElbow->GetRotateY(), rightElbow->GetRotateZ(), 1);

		rightCoxa->WholeRotate(-this->HorseStepRate, rightCoxa->GetRotateX(), rightCoxa->GetRotateY(), rightCoxa->GetRotateZ(), 1);
		leftCoxa->WholeRotate(this->HorseStepRate, 1, 0.5, -1, 1);

		rightKnee->WholeRotate(this->HorseStepRate, rightKnee->GetRotateX(), rightKnee->GetRotateY(), rightKnee->GetRotateZ(), 1);
		leftKnee->WholeRotate(-this->HorseStepRate, leftKnee->GetRotateX(), leftKnee->GetRotateY(), leftKnee->GetRotateZ(), 1);

		float h = this->HorseStepHeight.top();
		HorseStepHeight.pop();
		body->Translate(0, h ,0);
		this->WindowDisplay();
		Sleep(20);
	}
	// �ֲ�Swing������Ϊ���ȵ��µ���������ȫ�ָ�״̬
	body->SetY(body->GetInitY());;
	this->WindowDisplay();
	Sleep(20);

	// ������ͬ����ת���Ե�����
	leftShoulder->PopLastRotate(); //leftShoulder->PopLastRotate();
	leftElbow->PopLastRotate(); //leftElbow->PopLastRotate();
	rightShoulder->PopLastRotate();  //rightShoulder->PopLastRotate(); 
	leftCoxa->PopLastRotate(); //leftCoxa->PopLastRotate();
	rightCoxa->PopLastRotate(); //rightCoxa->PopLastRotate();
	leftKnee->PopLastRotate(); //le/ftKnee->PopLastRotate(); 
	rightKnee->PopLastRotate(); //rightKnee->PopLastRotate();
	body->PopLastRotate();  //body->PopLastRotate();
}

void Robot::Reset() {
	this->ResetSwing();
	this->ResetHorseStep();
}

void Robot::Dance() {
	this->TurnFrontBack(0);   // ��ɽ��Ҫ�ȱ�����Ļ
	this->WalkStand();   // Ȼ��תΪ��������
	Sleep(500);   // 0.5s ��׼��ʱ�� 
	this->HorseStep();  //������̧�ֱۣ�����׼������
    this->Swing();   //��ɽ����ҡ��
	Sleep(500);
	this->Reset();
	this->UnlockMyMutex();  // ������������ɽ������������
}

void Robot::Greeting() {
	std::cout << "this is hello" << std::endl;
}