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
	map<string, Component*> assembly; // �����˵ĸ�������
	int turn = 0;   // ����ĽǶ�
	int turnRate = 5; // ת����ٶ�
	float forwardRate = 0.04; // ǰ�����ٶ�
	float armLiftLimit = 30;  // ̧�ֵļ��޽Ƕ�
	float legLiftLimit = 45;  //��·̧�ȵļ��޽Ƕ�
	float legLiftRate = 3;   // ��·̧�ȵ��ٶ�
	float HorseStepRate = 5;  //������������ת���ٶ�
	float HorseStepLimit = 45; // �������ȵļ��޽Ƕ�
	float SwingLimit = 10; //��ɽ������Ƕȵķ���
	float SwingRate = 0.05; // ��ɽ���Ķ����ٶ�
	float SwingDLimit = 0.1;
	std::stack<float> HorseStepHeight; // ��¼��������������½���ջ�����ڻ�ԭ
	float ResetRate = 5;  // �ָ����������Ƹ���������ԭ��ת���ٶ�
	// ������ֻ꣬��������Ǿ�������
	float x = 0;   // x����, Ϊ�����˵�����ײ�ƽ�������λ��
	float y = 0;   // y����
	float z = 0;   // z����

	bool mutex = 0; //��������ɽ����ʱ����·
	bool pause = 0; //��������ͣ��
	bool stopSwing= 0;  //������ɽ����ֹͣ

	std::mutex m;
	std::unique_lock<std::mutex> nothing; 	  // wait��Ҫһ��������Ϊ���ﲻ�Ǵ�ͳ�Ļ��⣬�ñ��������������

	void AddComponet(const string& label, Component* part);  // �������еĲ�����Ӳ���

	template<typename T> 
	void SetPartProperty(T* arr, T x, T y, T z);

	void SetPartRadius(float& r, float value);
	void TurnFrontBack(bool toward = 1);  // ʹ������������߱��泯����Ļ
	void WalkStand(); // ����·��̬�ع鵽������̬
	void HorseStep();  // ��������̬תΪ����̬
	void ResetSwing();   // ��ҡ���лظ�������̬
	void ResetHorseStep();  // ������̬�ظ���������̬
	void Reset();  // �ظ���������̬
	void Swing();
	void(*windowDisplay)();    // ָ��������Ļ��display����
	std::condition_variable cv;  //���ڴ�����ͣ
	

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
	void Generate();   // ���ɸ�����Ҫ�Ĳ���
	void Display();  // ��ʾ���еĲ���
	void ClockwiseRotate(); // ˳ʱ����ת
	void AntiClockwiseRotate(); //��ʱ����ת
	void WalkForward(); //��ǰ����
	void WalkBackward(); //�����
	void Dance(); // ģ�²���������ɽ��
	void WindowDisplay() { this->windowDisplay(); }
	void Greeting();   // �ʺ���
	// ��ɽ����ʱ��Ҫ�������ڼ䲻��ǰ���ƶ�
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





