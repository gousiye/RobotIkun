#pragma once
static class MathOperation
{

private:
	static const float PI;

public:
	//向量绕任意轴旋转后的坐标
	static void RotateAnyAxis(const float const* pos, const float const* axis, const float theta, float* newPos); 

	//向量叉乘 (三维)
	static void CrossProduct(float a[3], float b[3], float ans[3]);

	//向量点乘 (三维)
	static float DotProduct(float a[3], float b[3]);

	//向量取模（三维)
	static float GetLen(float a[3]);
};

