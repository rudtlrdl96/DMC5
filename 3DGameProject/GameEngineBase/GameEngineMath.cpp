#include "PrecompileHeader.h"
#include "GameEngineMath.h"
#include "GameEngineString.h"

// �������� ���ڰ� �տ��ü��� �����ϴ�.

const float GameEngineMath::PIE = 3.141592653589793238462643383279502884197169399375105820974944f;
const float GameEngineMath::PIE2 = PIE * 2.0f;
const float GameEngineMath::DegToRad = GameEngineMath::PIE / 180;
const float GameEngineMath::RadToDeg = 180 / GameEngineMath::PIE;

const float4 float4::LEFT = { -1.0f, 0.0f, 0.0f, 1.0f };
const float4 float4::RIGHT = { 1.0f, 0.0f, 0.0f, 1.0f };
const float4 float4::UP = { 0.0f, 1.0f, 0.0f, 1.0f };
const float4 float4::DOWN = { 0.0f, -1.0f, 0.0f, 1.0f };
const float4 float4::FORWARD = { 0.0f, 0.0f, 1.0f, 1.0f };
const float4 float4::BACK = { 0.0f, 0.0f, -1.0f, 1.0f };

const float4 float4::ONE = { 1.0f, 1.0f, 1.0f, 1.0f };
const float4 float4::ZERO = { 0.0f, 0.0f, 0.0f, 1.0f };
const float4 float4::ZERONULL = { 0.0f, 0.0f, 0.0f, 0.0f };

const float4 float4::RED = { 1.0f, 0.0f, 0.0f, 1.0f };
const float4 float4::BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };
const float4 float4::GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
const float4 float4::WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
const float4 float4::BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };

void float4::RotationXRad(float _Rad)
{
	float4x4 Rot;
	Rot.RotationXRad(_Rad);
	*this *= Rot;
}

void float4::RotationYRad(float _Rad)
{
	float4x4 Rot;
	Rot.RotationYRad(_Rad);
	*this *= Rot;
}

void float4::RotationZRad(float _Rad)
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

float4 float4::DegreeRotationToQuaternionReturn() const
{
	float4 Rot = *this;
	Rot *= GameEngineMath::DegToRad;
	Rot.DirectVector = DirectX::XMQuaternionRotationRollPitchYawFromVector(Rot.DirectVector);
	return Rot;
}

float4 float4::QuaternionToEulerDeg() const
{
	return QuaternionToEulerRad() * GameEngineMath::RadToDeg;
}

float4 float4::QuaternionToEulerRad() const
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
}

float float4::XYZDistance(float4 _Value)
{
	float4 Len = DirectX::XMVector3Length(*this - _Value);
	return Len.x;
}

float4 float4::DegreeToDirection2D(float _Degree)
{
	return RadianToDirection2D(_Degree * GameEngineMath::DegToRad);
}

float4 float4::RadianToDirection2D(float _Radian)
{
	return { cosf(_Radian) , sinf(_Radian) };
}

float4 float4::MatrixToQuaternion(const class float4x4& M)
{
	float4 Return;

	if (M.ArrVector[0].IsNearlyZero() || M.ArrVector[1].IsNearlyZero() || M.ArrVector[2].IsNearlyZero())
	{
		Return.x = 0.0f;
		Return.y = 0.0f;
		Return.z = 0.0f;
		Return.w = 1.0f;
		return Return;
	}

	float	s;

	// Check diagonal (trace)
	const float tr = M.Arr2D[0][0] + M.Arr2D[1][1] + M.Arr2D[2][2];

	if (tr > 0.0f)
	{
		float InvS = InvSqrt(tr + 1.f);
		Return.w = 0.5f * (1.f / InvS);
		s = 0.5f * InvS;

		Return.x = (M.Arr2D[1][2] - M.Arr2D[2][1]) * s;
		Return.y = (M.Arr2D[2][0] - M.Arr2D[0][2]) * s;
		Return.z = (M.Arr2D[0][1] - M.Arr2D[1][0]) * s;
	}
	else
	{
		// diagonal is negative
		int i = 0;

		if (M.Arr2D[1][1] > M.Arr2D[0][0])
			i = 1;

		if (M.Arr2D[2][2] > M.Arr2D[i][i])
			i = 2;

		static const int nxt[3] = { 1, 2, 0 };
		const int j = nxt[i];
		const int k = nxt[j];

		s = M.Arr2D[i][i] - M.Arr2D[j][j] - M.Arr2D[k][k] + 1.0f;

		float InvS = InvSqrt(s);

		float qt[4];
		qt[i] = 0.5f * (1.f / InvS);

		s = 0.5f * InvS;

		qt[3] = (M.Arr2D[j][k] - M.Arr2D[k][j]) * s;
		qt[j] = (M.Arr2D[i][j] + M.Arr2D[j][i]) * s;
		qt[k] = (M.Arr2D[i][k] + M.Arr2D[k][i]) * s;

		Return.x = qt[0];
		Return.y = qt[1];
		Return.z = qt[2];
		Return.w = qt[3];
	}

	return Return;
}

std::vector<unsigned int> GameEngineMath::GetDigits(int _Value)
{
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
	float4 ReturnValue = DirectX::XMVector4Transform(*this, _Other);

	return ReturnValue;
}

float4& float4::operator*=(const class float4x4& _Other)
{
	DirectVector = DirectX::XMVector4Transform(*this, _Other);;
	return *this;
}

const float4x4 float4x4::Zero = float4x4(float4::ZERONULL, float4::ZERONULL, float4::ZERONULL, float4::ZERONULL);