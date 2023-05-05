#pragma once
#include "Component.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
using std::vector;
using std::string;
using std::map;


class Robot{
private:
	const static float PI;
	int angleRange = 30;
	map<string, Component*> assembly; // �����˵ĸ�������
	int turn = 0;   // ����ĽǶ�
	int turnRate = 5; // ת����ٶ�
	float forwardRate = 0.04; // ǰ�����ٶ�
	float armLiftLimit = 30;  // ̧�ֵļ��޽Ƕ�
	float legLiftLimit = 45;  //��·̧�ȵļ��޽Ƕ�
	float legLiftRate = 3;   // ��·̧�ȵ��ٶ�
	// ������ֻ꣬��������Ǿ�������
	float x = 0;   // x����, Ϊ�����˵�����ײ�ƽ�������λ��
	float y = 0;   // y����
	float z = 0;   // z����

	void AddComponet(const string& label, Component* part);  // �������еĲ�����Ӳ���

	template<typename T> 
	void SetPartProperty(T* arr, T x, T y, T z);
	
	void SetPartRadius(float& r, float value);

public:
	Robot(int newX = 0, int newY = 0, int newZ = 0);
	~Robot();
	void SetLegLiftRate(float rate) { this->legLiftRate = rate; }
	void SetLegLiftLimit(float limit) { this->legLiftLimit = limit; }
	void SetTurnRate(int rate) { this->turnRate = rate; }
	void SetForwardRate(float rate) { this->forwardRate = rate; }
	void SetAngleRange(int range) { this->angleRange = range; }
	void Generate();   // ���ɸ�����Ҫ�Ĳ���
	void Display();  // ��ʾ���еĲ���
	void ClockwiseRotate(); // ˳ʱ����ת
	void AntiClockwiseRotate(); //��ʱ����ת
	void WalkForward(); //��ǰ����
	void WalkBackward(); //�����
	void Greeting();   // �ʺ���
};





