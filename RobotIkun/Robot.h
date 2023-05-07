#pragma once
#include "Component.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <condition_variable>
#include <mutex>
using std::vector;
using std::string;
using std::map;


class Robot{
private:
	const static float PI;
	int angleRange = 30;
	map<string, Component*> assembly; // 机器人的各个部件
	int turn = 0;   // 朝向的角度
	int turnRate = 5; // 转身的速度
	float forwardRate = 0.04; // 前进的速度
	float armLiftLimit = 30;  // 抬手的极限角度
	float legLiftLimit = 45;  //走路抬腿的极限角度
	float legLiftRate = 3;   // 走路抬腿的速度
	float HorseStepRate = 5;  //扎马步各部件旋转的速度
	float HorseStepLimit = 45; // 扎马步大腿的极限角度
	float SwingLimit = 10; //铁山靠身体角度的幅度
	float SwingRate = 0.05; // 铁山靠的动作速度
	float SwingDLimit = 0.1;
	std::stack<float> HorseStepHeight; // 记录扎马步导致身体的下降的栈，用于还原
	float ResetRate = 5;  // 恢复成立正姿势各个部件还原的转动速度
	// 相对坐标，只有躯体的是绝对坐标
	float x = 0;   // x坐标, 为机器人的躯体底部平面的中心位置
	float y = 0;   // y坐标
	float z = 0;   // z坐标

	bool mutex = 0; //不能在铁山靠的时候走路
	bool pause = 0; //机器人暂停了
	bool stopSwing= 0;  //这轮铁山靠完停止

	std::mutex m;
	std::unique_lock<std::mutex> nothing; 	  // wait需要一个锁，因为这里不是传统的互斥，用变量代替这个锁了

	void AddComponet(const string& label, Component* part);  // 根据已有的部件添加部件

	template<typename T> 
	void SetPartProperty(T* arr, T x, T y, T z);

	void SetPartRadius(float& r, float value);
	void TurnFrontBack(bool toward = 1);  // 使机器人正面或者背面朝向屏幕
	void WalkStand(); // 从走路姿态回归到立正姿态
	void HorseStep();  // 从立正姿态转为马步姿态
	void ResetSwing();   // 从摇摆中回复到马步姿态
	void ResetHorseStep();  // 从马步姿态回复到立正姿态
	void Reset();  // 回复到立正姿态
	void Swing();
	void(*windowDisplay)();    // 指向整个屏幕的display函数
	std::condition_variable cv;  //用于处理暂停
	

public:
	Robot(int newX = 0, int newY = 0, int newZ = 0);
	~Robot();
	const float& GetTurn() const { return this->turn; }
	void SetWindowDisplay(void(* displayFunc)());
	// void SetKeyBoard(void(*Keyboard)(unsigned char, int, int));
	void SetLegLiftRate(float rate) { this->legLiftRate = rate; }
	void SetLegLiftLimit(float limit) { this->legLiftLimit = limit; }
	void SetTurnRate(int rate) { this->turnRate = rate; }
	void SetForwardRate(float rate) { this->forwardRate = rate; }
	void SetAngleRange(int range) { this->angleRange = range; }
	void Generate();   // 生成各个需要的部件
	void Display();  // 显示所有的部件
	void ClockwiseRotate(); // 顺时针旋转
	void AntiClockwiseRotate(); //逆时针旋转
	void WalkForward(); //向前行走
	void WalkBackward(); //向后走
	void Dance(); // 模仿蔡徐坤的铁山靠
	void WindowDisplay() { this->windowDisplay(); }
	void Greeting();   // 问候语
	// 铁山靠的时候要上锁，期间不能前后移动
	void LockMyMutex() { mutex = 1; }
	void UnlockMyMutex () { mutex = 0; }
	bool IsLock() { return this->mutex; }
	bool IsPause() { return this->pause; }
	void ChangePauseState() {
		if (pause) pause = 0;
		else pause = 1;
	}
	void Notify() { this->cv.notify_all(); }
	void StopSwing() { this->stopSwing = 1; }
};





