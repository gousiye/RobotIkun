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
	map<string, Component*> assembly; // �����˵ĸ�������
	int turn = 0;   // ����ĽǶ�
	int turnRate = 5; // ת����ٶ�
	float forwardRate = 0.04; // ǰ�����ٶ�
	int x = 0;   // x����, Ϊ�����˵�����ײ�ƽ�������λ��
	int y = 0;   // y����
	int z = 0;   // z����
	void AddComponet(const string& label, Component* part);  // �������еĲ�����Ӳ���

	template<typename T> 
	void SetPartProperty(T* arr, T x, T y, T z);
	
	void SetPartRadius(float& r, float value);
public:
	Robot(int newX = 0, int newY = 0, int newZ = 0);
	~Robot();
	void SetTurnRate(int rate) { this->turnRate = rate; }
	void SetForwardRate(float rate) { this->forwardRate = rate; }
	void Generate();   // ���ɸ�����Ҫ�Ĳ���
	void Display();  // ��ʾ���еĲ���
	void ClockwiseRotate(); // ˳ʱ����ת
	void AntiClockwiseRotate(); //��ʱ����ת
	void Greeting();   // �ʺ���
};





