#pragma once
static class MathOperation
{

private:
	static const float PI;

public:
	//��������������ת�������
	static void RotateAnyAxis(const float const* pos, const float const* axis, const float theta, float* newPos); 

	//������� (��ά)
	static void CrossProduct(float a[3], float b[3], float ans[3]);

	//������� (��ά)
	static float DotProduct(float a[3], float b[3]);

	//����ȡģ����ά)
	static float GetLen(float a[3]);
};

