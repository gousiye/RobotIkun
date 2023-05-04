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
	map<string, Component*> assembly; // 机器人的各个部件
	int turn = 0;   // 朝向的角度
	int turnRate = 5; // 转身的速度
	float forwardRate = 0.04; // 前进的速度
	int x = 0;   // x坐标, 为机器人的躯体底部平面的中心位置
	int y = 0;   // y坐标
	int z = 0;   // z坐标
	void AddComponet(const string& label, Component* part);  // 根据已有的部件添加部件

	template<typename T> 
	void SetPartProperty(T* arr, T x, T y, T z);
	
	void SetPartRadius(float& r, float value);
public:
	Robot(int newX = 0, int newY = 0, int newZ = 0);
	~Robot();
	void SetTurnRate(int rate) { this->turnRate = rate; }
	void SetForwardRate(float rate) { this->forwardRate = rate; }
	void Generate();   // 生成各个需要的部件
	void Display();  // 显示所有的部件
	void ClockwiseRotate(); // 顺时针旋转
	void AntiClockwiseRotate(); //逆时针旋转
	void Greeting();   // 问候语
};





