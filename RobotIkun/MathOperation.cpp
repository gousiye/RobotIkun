#include "MathOperation.h"
#include <iostream>
#include "cmath"


const float  MathOperation::PI = 3.14159;

void MathOperation::RotateAnyAxis(const float  *const pos, const float  *const axis, const float  theta, float * newPos) {

	float  rotateMatrix[3][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
	float  cosTheta = cos(theta * PI / 180.0f);
	float  sinTheta = sin(theta * PI / 180.0f);

	float  x = axis[0], y = axis[1], z = axis[2];
	float  sum = sqrt(x * x + y * y + z * z);
	x = x / sum;
	y = y / sum;
	z = z / sum;
	rotateMatrix[0][0] = x * x * (1 - cosTheta) + cosTheta;
	rotateMatrix[0][1] = x * y * (1 - cosTheta) - z * sinTheta;
	rotateMatrix[0][2] = x * z * (1 - cosTheta) + y * sinTheta;

	rotateMatrix[1][0] = x * y * (1 - cosTheta) + z * sinTheta;
	rotateMatrix[1][1] = y * y * (1 - cosTheta) + cosTheta;
	rotateMatrix[1][2] = y * z * (1 - cosTheta) - x * sinTheta;

	rotateMatrix[2][0] = x * z * (1 - cosTheta) - y * sinTheta;
	rotateMatrix[2][1] = y * z * (1 - cosTheta) + x * sinTheta;
	rotateMatrix[2][2] = z * z * (1 - cosTheta) + cosTheta;

	newPos[0] = newPos[1] = newPos[2] = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			newPos[i] += rotateMatrix[i][j] * pos[j];
		}
	}
}

void MathOperation::CrossProduct(float  a[3], float  b[3], float  ans[3])
{
	ans[0] = a[1] * b[2] - a[2] * b[1];
	ans[1] = a[2] * b[0] - a[0] * b[2];
	ans[2] = a[0] * b[1] - a[1] * b[0];
}

float  MathOperation::DotProduct(float  a[3], float  b[3])
{
	float  ans = 0.0f;
	for (int i = 0; i < 3; i++) {
		ans += a[i] * b[i];
	}
	return ans;
}

float  MathOperation::GetLen(float  a[3]){
	float  len = 0.0f;
	for (int i = 0; i < 3; i++) {
		len += a[i] * a[i];
	}
	return sqrt(len);
}



