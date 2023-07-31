#include "PrecompileHeader.h"
#include "GameEngineMath.h"
#include "GameEngineString.h"

// 변수명은 숫자가 앞에올수가 없습니다.

const float GameEngineMath::PIE = 3.141592653589793238462643383279502884197169399375105820974944f;
const float GameEngineMath::PIE2 = PIE * 2.0f;
const float GameEngineMath::DegToRad = GameEngineMath::PIE / 180;
const float GameEngineMath::RadToDeg = 180 / GameEngineMath::PIE;

const float4 float4::Left	   = {-1.0f, 0.0f, 0.0f, 1.0f};
const float4 float4::Right	   = {1.0f, 0.0f, 0.0f, 1.0f };
const float4 float4::Up		   = { 0.0f, 1.0f, 0.0f, 1.0f };
const float4 float4::Down	   = { 0.0f, -1.0f, 0.0f, 1.0f };
const float4 float4::Forward   = { 0.0f, 0.0f, 1.0f, 1.0f };
const float4 float4::Back	   = { 0.0f, 0.0f, -1.0f, 1.0f };

const float4 float4::One = { 1.0f, 1.0f, 1.0f, 1.0f };
const float4 float4::Zero = { 0.0f, 0.0f, 0.0f, 1.0f };
const float4 float4::Null = { 0.0f, 0.0f, 0.0f, 0.0f };

const float4 float4::Red = {1.0f, 0.0f, 0.0f, 1.0f};
const float4 float4::Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
const float4 float4::Green = { 0.0f, 1.0f, 0.0f, 1.0f };
const float4 float4::White = { 1.0f, 1.0f, 1.0f, 1.0f };
const float4 float4::Black = { 0.0f, 0.0f, 0.0f, 1.0f };

void float4::RotaitonXRad(float _Rad)
{
	float4x4 Rot;
	Rot.RotationXRad(_Rad);
	*this *= Rot;
}

void float4::RotaitonYRad(float _Rad)
{
	float4x4 Rot;
	Rot.RotationYRad(_Rad);
	*this *= Rot;
}

void float4::RotaitonZRad(float _Rad)
{
	float4x4 Rot;
	Rot.RotationZRad(_Rad);
	*this *= Rot;
}

float4 float4::EulerDegToQuaternion()
{
	float4 Return = DirectVector;
	Return *= GameEngineMath::DegToRad;
	Return = DirectX::XMQuaternionRotationRollPitchYawFromVector(Return.DirectVector);
	return Return;
}

float4 float4::QuaternionToEulerDeg() 
{
	return QuaternionToEulerRad() * GameEngineMath::RadToDeg;
}

float4 float4::QuaternionToEulerRad()
{
	float4 result;

	float sinrCosp = 2.0f * (w * z + x * y);
	float cosrCosp = 1.0f - 2.0f * (z * z + x * x);
	result.z = atan2f(sinrCosp, cosrCosp);

	float pitchTest = w * x - y * z;
	float asinThreshold = 0.4999995f;
	float sinp = 2.0f * pitchTest;

	if (pitchTest < -asinThreshold)
	{
		result.x = -(0.5f * GameEngineMath::PIE);
	}
	else if (pitchTest > asinThreshold)
	{
		result.x = (0.5f * GameEngineMath::PIE);
	}
	else
	{
		result.x = asinf(sinp);
	}

	float sinyCosp = 2.0f * (w * y + x * z);
	float cosyCosp = 1.0f - 2.0f * (x * x + y * y);
	result.y = atan2f(sinyCosp, cosyCosp);

	return result;
}

float4x4 float4::QuaternionToRotationMatrix()
{
	return DirectX::XMMatrixRotationQuaternion(DirectVector);
}

float float4::XYDistance(float4 _Value)
{
	float4 Len = DirectX::XMVector2Length(*this - _Value);
	return Len.x;

	// return sqrtf((x - _Value.x) * (x - _Value.x) + (y - _Value.y) * (y - _Value.y));
}

float float4::XYZDistance(float4 _Value)
{
	float4 Len = DirectX::XMVector3Length(*this - _Value);
	return Len.x;

	// return sqrtf((x - _Value.x) * (x - _Value.x) + (y - _Value.y) * (y - _Value.y) + (z - _Value.z) * (z - _Value.z));
}

// 뭘하는 함수냐?
// 123121 [1][2][3][1][2][1]
std::vector<unsigned int> GameEngineMath::GetDigits(int _Value)
{
	// std::vector<unsigned int> ResultValue = std::vector<unsigned int>();

	//if (0 == _Value)
	//{
	//	ResultValue.push_back(0);
	//	return ResultValue;
	//}

	//int Lenth = GetLenth(_Value);
	//ResultValue.resize(Lenth);

	//int Value = _Value;
	//int HighestDigitValue = 0;
	//for (size_t i = 0; i < Lenth; i++)
	//{
	//	HighestDigitValue = Value / pow(10, Lenth - (1 + i));
	//	ResultValue[i] = HighestDigitValue;
	//	Value -= HighestDigitValue * pow(10, Lenth - (1 + i));
	//}

	std::vector<unsigned int> ResultValue = std::vector<unsigned int>();

	std::string StringValue = GameEngineString::ToString(_Value);

	int StartIndex = 0;

	if (0 != StringValue.size() && StringValue[0] == '-')
	{
		StartIndex = 1;
	}

	ResultValue.reserve(StringValue.size());

	for (size_t i = StartIndex; i < StringValue.size(); i++)
	{
		ResultValue.push_back(StringValue[i] - '0');
	}

	return ResultValue;
}

unsigned int GameEngineMath::GetLenth(int _Value)
{
	std::vector<unsigned int> ResultValue = std::vector<unsigned int>();

	std::string StringValue = GameEngineString::ToString(_Value);

	int StartIndex = 0;

	if (0 != StringValue.size() && StringValue[0] == '-')
	{
		StartIndex = 1;
	}

	return static_cast<unsigned int>(StringValue.size()) - StartIndex;
}

float4 float4::operator*(const class float4x4& _Other)
{
	// this

	// 크기
	// 0.5f, 0.5f, 0.5f, 1; *  100   0   0   0
	// 					         0 100   0   0
	// 					         0   0 100   0
	// 					         0   0   0   1
	// 
	//(100 * -0.5) + (0 * Arr1D[0]) + (0 * Arr1D[0]) + (0 * 1);
	//(0 * -0.5) + (100 * Arr1D[0]) + (0 * Arr1D[0]) + (0 * 1);
	//(0 * -0.5) + (0 * Arr1D[0]) + (100 * Arr1D[0]) + (0 * 1);
	//(0 * -0.5) + (0 * Arr1D[0]) + (0 * Arr1D[0]) + (0 * 1);

	// 위치
	// 100.0f, 100.0f, 100.0f, 1; *    1   0   0   0
	// 					               0   1   0   0
	// 					               0   0   1   0
	// 					             100  80  50   1
	
	// 100 + 100, 100 + 80, 100 + 50

	//(100 * 1) + (0 * 0) + (0 * 0) + (100 * 1);
	//(0 * 0) + (100 * 1) + (0 * 0) + (80 * 1);
	//(0 * 0) + (0 * 0) + (100 * 1) + (50 * 1);
	//(0 * 0) + (0 * 0) + (0 * 0) +   (1 * 1);

	// 크기
	// 1.0f, 0.0f, 0.0f, 1; *    1   0   0   0
	// 					         0   1   0   0
	// 					         0   0   1   0
	// 					         0   0   0   1

	// 0.5f, 0.5f, 0.0f  1  x 0   0  -1   0
	//						y 0   1   0   0
	//						z 1   0   0   0
	//						  0   0   0   1

	float4 ReturnValue = DirectX::XMVector4Transform(*this, _Other);
	//ReturnValue.x = (_Other.Arr2D[0][0] * Arr1D[0]) + (_Other.Arr2D[1][0] * Arr1D[1]) + (_Other.Arr2D[2][0] * Arr1D[2]) + (_Other.Arr2D[3][0] * Arr1D[3]);
	//ReturnValue.y = (_Other.Arr2D[0][1] * Arr1D[0]) + (_Other.Arr2D[1][1] * Arr1D[1]) + (_Other.Arr2D[2][1] * Arr1D[2]) + (_Other.Arr2D[3][1] * Arr1D[3]);
	//ReturnValue.z = (_Other.Arr2D[0][2] * Arr1D[0]) + (_Other.Arr2D[1][2] * Arr1D[1]) + (_Other.Arr2D[2][2] * Arr1D[2]) + (_Other.Arr2D[3][2] * Arr1D[3]);
	//ReturnValue.w = (_Other.Arr2D[0][3] * Arr1D[0]) + (_Other.Arr2D[1][3] * Arr1D[1]) + (_Other.Arr2D[2][3] * Arr1D[2]) + (_Other.Arr2D[3][3] * Arr1D[3]);

	return ReturnValue;
}


float4& float4::operator*=(const class float4x4& _Other)
{
	DirectVector = DirectX::XMVector4Transform(*this, _Other);;
	return *this;
}

const float4x4 float4x4::Zero = float4x4(float4::Null, float4::Null, float4::Null, float4::Null);