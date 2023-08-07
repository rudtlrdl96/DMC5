#pragma once

#include <math.h>
#include <cmath>
#include <string>
#include <Windows.h>
#include <vector>

// dx함수들
// 다이렉트용 벡터
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>



// final 더이상 상속내릴지 못한다.
// 상속도 못하고 만들지도 못하게 만든 상태로

class GameEngineMath final
{
public:
	static std::vector<unsigned int> GetDigits(int _Value);
	static unsigned int GetLenth(int _Value);
	static const float PIE;
	static const float PIE2;
	static const float DegToRad;
	static const float RadToDeg;

private:
	virtual ~GameEngineMath() = 0;
};

class float4
{
public:
	static const float4 LEFT;
	static const float4 RIGHT;
	static const float4 UP;
	static const float4 DOWN;
	static const float4 FORWARD;
	static const float4 BACK;
	static const float4 ONE;
	static const float4 ZERO;
	static const float4 ZERONULL;

	static const float4 RED;
	static const float4 BLUE;
	static const float4 GREEN;
	static const float4 WHITE;
	static const float4 BLACK;

	static float4 MatrixToQuaternion(const class float4x4& M);


	static float InvSqrt(float f)
	{
		const __m128 fOneHalf = _mm_set_ss(0.5f);
		__m128 Y0, X0, X1, X2, FOver2;
		float temp;

		Y0 = _mm_set_ss(f);
		X0 = _mm_rsqrt_ss(Y0);	// 1/sqrt estimate (12 bits)
		FOver2 = _mm_mul_ss(Y0, fOneHalf);

		// 1st Newton-Raphson iteration
		X1 = _mm_mul_ss(X0, X0);
		X1 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X1));
		X1 = _mm_add_ss(X0, _mm_mul_ss(X0, X1));

		// 2nd Newton-Raphson iteration
		X2 = _mm_mul_ss(X1, X1);
		X2 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X2));
		X2 = _mm_add_ss(X1, _mm_mul_ss(X1, X2));

		_mm_store_ss(&temp, X2);
		return temp;
	}

	bool IsNearlyZero() const
	{
		return fabsf(x) <= 1.e-4f && fabsf(y) <= 1.e-4f && fabsf(z) <= 1.e-4f;
	}

	static float4 GetSafeScaleReciprocal(const float4& _InScale, float _Tolerance)
	{
		float4 SafeReciprocalScale;

		if (std::fabsf(_InScale.x) <= _Tolerance)
		{
			SafeReciprocalScale.x = 0.f;
		}
		else
		{
			SafeReciprocalScale.x = 1 / _InScale.x;
		}

		if (std::fabsf(_InScale.y) <= _Tolerance)
		{
			SafeReciprocalScale.y = 0.f;
		}
		else
		{
			SafeReciprocalScale.y = 1 / _InScale.y;
		}

		if (std::fabsf(_InScale.z) <= _Tolerance)
		{
			SafeReciprocalScale.z = 0.f;
		}
		else
		{
			SafeReciprocalScale.z = 1 / _InScale.z;
		}

		return SafeReciprocalScale;
	}

	static float4 AngleToDirection2DToDeg(float _Deg)
	{
		return AngleToDirection2DToRad(_Deg * GameEngineMath::DegToRad);
	}

	static float4 AngleToDirection2DToRad(float _Rad)
	{
		// 빗변의 길이가 1일수밖에 없다.
		return float4(cosf(_Rad), sinf(_Rad), 0.0f, 1.0f);
	}

	static float GetAngleVectorToVectorDeg(const float4& _Left, const float4& _Right)
	{
		return GetAngleVectorToVectorRad(_Left, _Right) * GameEngineMath::RadToDeg;
	}

	static float GetAngleVectorToVectorDeg360(const float4& _Pivot, const float4& _Other)
	{
		return GetAngleVectorToVectorRad360(_Pivot, _Other) * GameEngineMath::RadToDeg;
	}

	// 외적의 결과는 두개의 백터가 겹칠때 주의해서 처리해줘야 한다.
	static float GetAngleVectorToVectorRad(const float4& _Left, const float4& _Right)
	{
		float4 Left = _Left;
		float4 Right = _Right;

		Left.Normalize();
		Right.Normalize();

		float CosSeta = DotProduct3D(Left, Right);

		float Angle = acosf(CosSeta);

		return Angle;
	}

	static float GetAngleVectorToVectorRad360(const float4& _Pivot, const float4& _Other)
	{
		float4 Pivot = _Pivot;
		float4 Other = _Other;

		Pivot.Normalize();
		Other.Normalize();

		float CosSeta = DotProduct3D(Pivot, Other);

		if (CosSeta >= 1.f)
		{
			CosSeta = 0.999999f;
		}
		else if (CosSeta <= -1.f)
		{
			CosSeta = -0.999999f;
		}

		float Angle = 0.f;
		(Pivot.x * Other.y) - (Pivot.y * Other.x) > 0.0f ? Angle = acosf(CosSeta) : Angle = -acosf(CosSeta);


		return Angle;
	}

	// 외적의 결과는 두개의 백터가 겹칠때 주의해서 처리해줘야 한다.
	static float4 Cross3DReturnNormal(const float4& _Left, const float4& _Right)
	{
		return Cross3DReturn(_Left.NormalizeReturn(), _Right.NormalizeReturn()).NormalizeReturn();
	}


	static float4 Cross3DReturn(const float4& _Left, const float4& _Right)
	{
		//float4 ReturnValue;
		//ReturnValue.x = (_Left.y * _Right.z) - (_Left.z * _Right.y);
		//ReturnValue.y = (_Left.z * _Right.x) - (_Left.x * _Right.z);
		//ReturnValue.z = (_Left.x * _Right.y) - (_Left.y * _Right.x);
		float4 ReturnValue = DirectX::XMVector3Cross(_Left, _Right);
		return ReturnValue;
	}

	static float DotProduct3D(const float4& _Left, const float4& _Right)
	{
		//            1         1          1          0          0          0
		// float Value = _Left.x * _Right.x + _Left.y * _Right.y + _Left.z * _Right.z;

		float4 ReturnValue = DirectX::XMVector3Dot(_Left, _Right);
		return ReturnValue.x;
	}

public:
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};

		struct
		{
			float r;
			float g;
			float b;
			float a;
		};

		float Arr1D[4];


		// 윈도우 지원 수학연산 가속(SIMD연산)을 사용하기 위한 다이렉트가 제공해주는 벡터
		// 32비트에서는 simd연산을 사용할수 없다.
		DirectX::XMFLOAT3 DirectFloat3;
		DirectX::XMFLOAT4 DirectFloat4;
		DirectX::XMVECTOR DirectVector;


		struct
		{
			float PosX;
			float PosY;
			float SizeX;
			float SizeY;
		};

	};

	float4()
		: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
	{

	}

	float4(float _x, float _y)
		: x(_x), y(_y), z(0.0f), w(1.0f)
	{

	}

	float4(float _x, float _y, float _z)
		: x(_x), y(_y), z(_z), w(1.0f)
	{

	}

	float4(float _x, float _y, float _z, float _w)
		: x(_x), y(_y), z(_z), w(_w)
	{

	}

	float4(DirectX::FXMVECTOR _Vector)
		: DirectVector(_Vector)
	{

	}

	void RotaitonXRad(float _Rad);
	void RotaitonYRad(float _Rad);
	void RotaitonZRad(float _Rad);
	float4 EulerDegToQuaternion();
	class float4x4 QuaternionToRotationMatrix();

	float4 QuaternionToEulerDeg();
	float4 QuaternionToEulerRad();

	float XYDistance(float4 _Value);
	float XYZDistance(float4 _Value);

	int ix() const
	{
		return static_cast<int>(x);
	}

	int iy() const
	{
		return static_cast<int>(y);
	}

	int iz() const
	{
		return static_cast<int>(z);
	}

	int iw() const
	{
		return static_cast<int>(w);
	}

	int hix() const
	{
		return static_cast<int>(x * 0.5f);
	}

	int hiy() const
	{
		return static_cast<int>(y * 0.5f);
	}

	int hiz() const
	{
		return static_cast<int>(z * 0.5f);
	}

	int hiw() const
	{
		return static_cast<int>(w * 0.5f);
	}


	float hx() const
	{
		return x * 0.5f;
	}

	float hy() const
	{
		return y * 0.5f;
	}

	float hz() const
	{
		return z * 0.5f;
	}

	float hw() const
	{
		return w * 0.5f;
	}


	int uix() const
	{
		return static_cast<unsigned int>(x);
	}

	int uiy() const
	{
		return static_cast<unsigned int>(y);
	}

	int uiz() const
	{
		return static_cast<unsigned int>(z);
	}

	int uiw() const
	{
		return static_cast<unsigned int>(w);
	}

	int uhix() const
	{
		return static_cast<unsigned int>(x * 0.5f);
	}

	int uhiy() const
	{
		return static_cast<unsigned int>(y * 0.5f);
	}

	int uhiz() const
	{
		return static_cast<unsigned int>(z * 0.5f);
	}

	int uhiw() const
	{
		return static_cast<unsigned int>(w * 0.5f);
	}

	float GetAnagleDegZ()
	{
		return GetAnagleRadZ() * GameEngineMath::RadToDeg;
	}

	float MaxFloat() const
	{
		return (x < y) ? (y < z ? z : y) : (x < z ? z : x);
	}

	UINT ColorToUint() const
	{
		UINT Return;

		char* Ptr = reinterpret_cast<char*>(&Return);

		// 0~1
		Ptr[0] = static_cast<int>(r * 255.0f);
		Ptr[1] = static_cast<int>(g * 255.0f);
		Ptr[2] = static_cast<int>(b * 255.0f);
		Ptr[3] = static_cast<int>(a * 255.0f);

		return Return;
	}


	float4 RotaitonXDegReturn(float _Deg)
	{
		float4 ReturnValue = *this;
		ReturnValue.RotaitonXRad(_Deg * GameEngineMath::DegToRad);
		return ReturnValue;
	}

	float4 RotaitonYDegReturn(float _Deg)
	{
		float4 ReturnValue = *this;
		ReturnValue.RotaitonYRad(_Deg * GameEngineMath::DegToRad);
		return ReturnValue;
	}

	float4 RotaitonZDegReturn(float _Deg)
	{
		float4 ReturnValue = *this;
		ReturnValue.RotaitonZRad(_Deg * GameEngineMath::DegToRad);
		return ReturnValue;
	}

	void RotaitonXDeg(float _Deg)
	{
		RotaitonXRad(_Deg * GameEngineMath::DegToRad);
	}

	void RotaitonYDeg(float _Deg)
	{
		RotaitonYRad(_Deg * GameEngineMath::DegToRad);
	}

	void RotaitonZDeg(float _Deg)
	{
		RotaitonZRad(_Deg * GameEngineMath::DegToRad);
	}

	float GetAnagleRadZ()
	{
		float4 AngleCheck = (*this);
		AngleCheck.Normalize();
		// functon(1) == 50; 1을 50으로 바꾸는 함수
		// afuncton(50) == 1; 50이 1로 바꿔주는 함수라고도 할수 있지만 functon에 들어갔던 인자값을 알아내는 함수라고도 할수 있죠? <= 역함수

		// cosf(각도);

		float Result = acosf(AngleCheck.x);

		if (AngleCheck.y < 0)
		{
			Result = GameEngineMath::PIE2 - Result;
		}
		return Result;

	}

	POINT ToWindowPOINT()
	{
		return POINT(ix(), iy());
	}

	float4 half() const
	{
		return { x * 0.5f,y * 0.5f,z * 0.5f,w };
	}

	bool IsZero() const
	{
		return x == 0.0f && y == 0.0f && z == 0.0f;
	}

	float Size() const
	{
		// 완벽
		return sqrtf(x * x + y * y + z * z);
	}

	// 2, 0
	// 0, 2
	void Normalize()
	{
		DirectVector = DirectX::XMVector3Normalize(*this);

		//float SizeValue = Size();
		//x /= SizeValue;
		//y /= SizeValue;
		//z /= SizeValue;
	}

	// 자기가 길이 1로 줄어든 애를 리턴해주는것.
	float4 NormalizeReturn() const
	{
		float4 Result = *this;
		Result.Normalize();
		return Result;
	}

	static float4 Lerp(const float4& Start, const float4& End, float Ratio)
	{
		// 1.5 + 0.5 * 2.5;

		return DirectX::XMVectorLerp(Start, End, Ratio);
	}

	static float4 LerpClamp(const float4& Start, const float4& End, float Ratio)
	{
		if (0 >= Ratio)
		{
			Ratio = 0.0f;
		}

		if (1.0f <= Ratio)
		{
			Ratio = 1.0f;
		}

		return Lerp(Start, End, Ratio);
	}

	operator DirectX::FXMVECTOR() const
	{
		return DirectVector;
	}


	float4 operator *(const float _Value) const
	{
		return DirectX::XMVectorMultiply(*this, float4{ _Value , _Value , _Value , 1.0f });
		//float4 Return;
		//Return.x = x * _Value;
		//Return.y = y * _Value;
		//Return.z = z * _Value;
	}



	bool operator ==(const float4& _Value) const
	{
		return _Value.x == x && _Value.y == y && _Value.z == z;
	}


	float4 operator +(const float4& _Value) const
	{
		float PrevW = w;
		float4 Return = DirectX::XMVectorAdd(*this, _Value);
		Return.w = PrevW;
		return Return;
	}

	float4 operator -(const float4& _Value) const
	{
		float PrevW = w;
		float4 Return = DirectX::XMVectorAdd(*this, -_Value);
		Return.w = PrevW;
		//float4 Return;
		//Return.x = x - _Value.x;
		//Return.y = y - _Value.y;
		//Return.z = z - _Value.z;
		return Return;
	}

	float4 operator *(const float4& _Value) const
	{
		//float4 Return;
		//Return.x = x * _Value.x;
		//Return.y = y * _Value.y;
		//Return.z = z * _Value.z;
		//return Return;

		float PrevW = w;
		float4 Return = DirectX::XMVectorMultiply(*this, _Value);
		Return.w = PrevW;
		return Return;
	}

	float4 operator /(const float4& _Value) const
	{
		float PrevW = w;
		float4 Return = DirectX::XMVectorDivide(*this, _Value);
		Return.w = PrevW;
		return Return;
	}


	float4 operator /(const float _Value) const
	{
		float PrevW = w;
		float4 Return = DirectX::XMVectorDivide(*this, { _Value , _Value , _Value , _Value });
		Return.w = PrevW;
		return Return;
	}

	float4 operator -() const
	{
		return { -x, -y, -z, w };
	}

	float4& operator +=(const float4& _Other)
	{
		*this = *this + _Other;
		return *this;
	}

	float4& operator *=(const float _Value)
	{
		//x *= _Value;
		//y *= _Value;
		//z *= _Value;
		*this = *this * _Value;
		return *this;
	}

	float4& operator /=(const float _Value)
	{
		//x /= _Value;
		//y /= _Value;
		//z /= _Value;

		*this = *this / _Value;
		return *this;
	}

	float4& operator *=(const float4& _Other)
	{
		//x *= _Other.x;
		//y *= _Other.y;
		//z *= _Other.z;
		*this = *this * _Other;
		return *this;
	}

	float4& operator -=(const float4& _Other)
	{
		//x -= _Other.x;
		//y -= _Other.y;
		//z -= _Other.z;
		*this = *this - _Other;
		return *this;
	}

	float4& operator /=(const float4& _Other)
	{
		//x /= _Other.x;
		//y /= _Other.y;
		//z /= _Other.z;
		*this = *this / _Other;
		return *this;
	}

	float4 operator*(const class float4x4& _Other);
	float4& operator*=(const class float4x4& _Other);

	std::string ToString()
	{
		char ArrReturn[256];

		sprintf_s(ArrReturn, "x: %f, y: %f, z: %f, w: %f", x, y, z, w);

		return std::string(ArrReturn);
	}

};

class CollisionDataAPI
{
public:
	float4 Position;
	float4 Scale; // x만 원의 반지름으로 보겠습니다.

	float Left() const
	{
		return Position.x - Scale.hx();
	}
	float Right() const
	{
		return Position.x + Scale.hx();
	}
	float Top() const
	{
		return Position.y - Scale.hy();
	}
	float Bot() const
	{
		return Position.y + Scale.hy();
	}

	float4 LeftTop() const
	{
		return float4{ Left(), Top() };
	}
	float4 RightTop() const
	{
		return float4{ Right(), Top() };
	}
	float4 LeftBot() const
	{
		return float4{ Left(), Bot() };
	}
	float4 RightBot() const
	{
		return float4{ Right(), Bot() };
	}
};

typedef float4 Quaternion;

class float4x4
{
public:
	static const float4x4 Zero;

	static const int YCount = 4;
	static const int XCount = 4;

public:
	union
	{
		float Arr1D[16];
		float Arr2D[4][4];
		float4 ArrVector[4];
		DirectX::XMMATRIX DirectMatrix;

		struct
		{
			float _00;
			float _01;
			float _02;
			float _03;
			float _10;
			float _11;
			float _12;
			float _13;
			float _20;
			float _21;
			float _22;
			float _23;
			float _30;
			float _31;
			float _32;
			float _33;
		};
	};

	static float4x4 Transformation(float4 _Scale, float4 _RotQ, float4 _Pos)
	{
		return DirectX::XMMatrixTransformation(float4::ZERO.DirectVector, float4::ZERO.DirectVector, _Scale.DirectVector, float4::ZERO.DirectVector, _RotQ.DirectVector, _Pos.DirectVector);
	}

	float4 ExtractScaling()
	{
		float4 ret = float4::ZERO;

		float Tolerance = 1.e-8f;

		const float SquareSum0 = (Arr2D[0][0] * Arr2D[0][0]) + (Arr2D[0][1] * Arr2D[0][1]) + (Arr2D[0][2] * Arr2D[0][2]);
		const float SquareSum1 = (Arr2D[1][0] * Arr2D[1][0]) + (Arr2D[1][1] * Arr2D[1][1]) + (Arr2D[1][2] * Arr2D[1][2]);
		const float SquareSum2 = (Arr2D[2][0] * Arr2D[2][0]) + (Arr2D[2][1] * Arr2D[2][1]) + (Arr2D[2][2] * Arr2D[2][2]);

		if (SquareSum0 > Tolerance)
		{
			float Scale0 = sqrtf(SquareSum0);
			ret.x = Scale0;
			float InvScale0 = 1.f / Scale0;
			Arr2D[0][0] *= InvScale0;
			Arr2D[0][1] *= InvScale0;
			Arr2D[0][2] *= InvScale0;
		}
		else
		{
			ret.x = 0;
		}

		if (SquareSum1 > Tolerance)
		{
			float Scale1 = sqrtf(SquareSum1);
			ret.y = Scale1;
			float InvScale1 = 1.f / Scale1;
			Arr2D[1][0] *= InvScale1;
			Arr2D[1][1] *= InvScale1;
			Arr2D[1][2] *= InvScale1;
		}
		else
		{
			ret.y = 0;
		}

		if (SquareSum2 > Tolerance)
		{
			float Scale2 = sqrtf(SquareSum2);
			ret.z = Scale2;
			float InvScale2 = 1.f / Scale2;
			Arr2D[2][0] *= InvScale2;
			Arr2D[2][1] *= InvScale2;
			Arr2D[2][2] *= InvScale2;
		}
		else
		{
			ret.z = 0;
		}

		return ret;
	}



	inline float Determinant() const
	{
		return	Arr2D[0][0] * (
			Arr2D[1][1] * (Arr2D[2][2] * Arr2D[3][3] - Arr2D[2][3] * Arr2D[3][2]) -
			Arr2D[2][1] * (Arr2D[1][2] * Arr2D[3][3] - Arr2D[1][3] * Arr2D[3][2]) +
			Arr2D[3][1] * (Arr2D[1][2] * Arr2D[2][3] - Arr2D[1][3] * Arr2D[2][2])
			) -
			Arr2D[1][0] * (
				Arr2D[0][1] * (Arr2D[2][2] * Arr2D[3][3] - Arr2D[2][3] * Arr2D[3][2]) -
				Arr2D[2][1] * (Arr2D[0][2] * Arr2D[3][3] - Arr2D[0][3] * Arr2D[3][2]) +
				Arr2D[3][1] * (Arr2D[0][2] * Arr2D[2][3] - Arr2D[0][3] * Arr2D[2][2])
				) +
			Arr2D[2][0] * (
				Arr2D[0][1] * (Arr2D[1][2] * Arr2D[3][3] - Arr2D[1][3] * Arr2D[3][2]) -
				Arr2D[1][1] * (Arr2D[0][2] * Arr2D[3][3] - Arr2D[0][3] * Arr2D[3][2]) +
				Arr2D[3][1] * (Arr2D[0][2] * Arr2D[1][3] - Arr2D[0][3] * Arr2D[1][2])
				) -
			Arr2D[3][0] * (
				Arr2D[0][1] * (Arr2D[1][2] * Arr2D[2][3] - Arr2D[1][3] * Arr2D[2][2]) -
				Arr2D[1][1] * (Arr2D[0][2] * Arr2D[2][3] - Arr2D[0][3] * Arr2D[2][2]) +
				Arr2D[2][1] * (Arr2D[0][2] * Arr2D[1][3] - Arr2D[0][3] * Arr2D[1][2])
				);
	}

	static float4x4 Affine(float4 _Scale, float4 _Rot, float4 _Pos)
	{
		// _Rot.DirectVector 쿼터니온 입니다.
		return DirectX::XMMatrixAffineTransformation(_Scale.DirectVector, float4::ZERO.DirectVector, _Rot.DirectVector, _Pos.DirectVector);
	}

	void Identity()
	{

		DirectMatrix = DirectX::XMMatrixIdentity();

		/*memset(Arr1D, 0, sizeof(float) * 16);
		Arr2D[0][0] = 1.0f;
		Arr2D[1][1] = 1.0f;
		Arr2D[2][2] = 1.0f;
		Arr2D[3][3] = 1.0f;*/
	}

	// z -x _EyeDir

	//                                           1280 / 720
	//                         수직시야각        화면의 종횡비   근평면      원평면
	void PerspectiveFovLH(float _FovAngle, float _AspectRatio, float _NearZ = 0.1f, float _FarZ = 10000.0f)
	{
		Identity();

		DirectMatrix = DirectX::XMMatrixPerspectiveFovLH(_FovAngle * GameEngineMath::DegToRad, _AspectRatio, _NearZ, _FarZ);

		//// 수직시야각이라면
		//float FOV = _FovAngle * GameEngineMath::DegToRad;
		////  _FovAngle * _AspectRatio;// 수평시야각 구하는법

		//// [0] [] [] []
		//// [] [0] [] []
		//// [] [] [0][1]
		//// [] [] [] [0]
		//Arr2D[2][3] = 1.0f;
		//Arr2D[3][3] = 0.0f;

		//Arr2D[0][0] = 1 / (tanf(FOV / 2.0f) * _AspectRatio);

		//// y 300
		//// z 5000
		//Arr2D[1][1] = 1 / tanf(FOV / 2.0f); // y / z

		//Arr2D[2][2] = _FarZ / (_FarZ - _NearZ);

		//Arr2D[3][2] = -( _NearZ * _FarZ) / (_FarZ - _NearZ);
	}

	void OrthographicLH(float _ScreenWidth, float _ScreenHeight, float _NearZ = 0.1f, float _FarZ = 10000.0f)
	{
		Identity();

		DirectMatrix = DirectX::XMMatrixOrthographicLH(_ScreenWidth, _ScreenHeight, _NearZ, _FarZ);
	}

	//            화면의 너비
	void ViewPort(float _Width, float _Height, float _Left, float _Right, float _ZMin = 0.0f, float _ZMax = 1.0f)
	{
		Identity();

		// 모니터의 공간으로 변환시키는 행렬
		Arr2D[0][0] = _Width * 0.5f;
		Arr2D[1][1] = -_Height * 0.5f;
		Arr2D[2][2] = _ZMax != 0.0f ? 1.0f : _ZMin / _ZMax;

		Arr2D[3][0] = Arr2D[0][0] + _Left;
		Arr2D[3][1] = _Height * 0.5f + _Right;
		Arr2D[3][2] = _ZMax != 0.0f ? 0.0f : _ZMin / _ZMax;
		Arr2D[3][3] = 1.0f;
	}

	void Compose(const float4& _Scale, const float4& _RotQuaternion, const float4& _Pos)
	{
		float4 _Rot = _RotQuaternion;
		_Rot.QuaternionToEulerDeg();
		*this = DirectX::XMMatrixAffineTransformation(_Scale.DirectVector, _Rot.DirectVector, _RotQuaternion.DirectVector, _Pos.DirectVector);
	}

	void Decompose(float4& _Scale, float4& _RotQuaternion, float4& _Pos) const
	{
		DirectX::XMMatrixDecompose(&_Scale.DirectVector, &_RotQuaternion.DirectVector, &_Pos.DirectVector, DirectMatrix);
	}

	void DecomposeRotQuaternion(float4& _RotQuaternion)
	{
		float4 Temp0;
		float4 Temp1;

		DirectX::XMMatrixDecompose(&Temp0.DirectVector, &_RotQuaternion.DirectVector, &Temp1.DirectVector, DirectMatrix);
	}

	void DecomposePos(float4& _Pos)
	{
		float4 Temp0;
		float4 Temp1;

		DirectX::XMMatrixDecompose(&Temp0.DirectVector, &Temp1.DirectVector, &_Pos.DirectVector, DirectMatrix);
	}

	void DecomposeScale(float4& _Scale)
	{
		float4 Temp0;
		float4 Temp1;

		DirectX::XMMatrixDecompose(&_Scale.DirectVector, &Temp0.DirectVector, &Temp1.DirectVector, DirectMatrix);
	}

	void LookToLH(const float4& _EyePos, const float4& _EyeDir, const float4& _EyeUp)
	{
		Identity();

		DirectMatrix = DirectX::XMMatrixLookToLH(_EyePos, _EyeDir, _EyeUp);


		//float4 EyePos = _EyePos;

		//// 합쳐져서 회전행렬이 된다.
		//float4 EyeDir = _EyeDir.NormalizeReturn();
		//float4 EyeUp = _EyeUp;
		//float4 Right = float4::Cross3DReturn(EyeUp, EyeDir);
		//Right.Normalize();

		//float4 UpVector = float4::Cross3DReturn(_EyeDir, Right);
		//Right.Normalize();

		//float4 NegEyePos = -_EyePos;

		//float D0Value = float4::DotProduct3D(Right, NegEyePos);
		//float D1Value = float4::DotProduct3D(UpVector, NegEyePos);
		//float D2Value = float4::DotProduct3D(EyeDir, NegEyePos);

		//// 여기서 내적을 사용합니다.
		//// x + 1;
		//// 역함수 혹은 역행렬이라고 부릅니다.
		//// x - 1;

		//ArrVector[0] = Right;
		//ArrVector[1] = UpVector;
		//ArrVector[2] = EyeDir;

		//Transpose();

		//ArrVector[3] = { D0Value, D1Value, D2Value, 0 };
	}

	// 전치 행렬이라고 부르는행려
	void Transpose()
	{
		// 0   , 0, -1
		// 100 , 1,  0
		// 1   , 0,  0

		// 0 , 100,  1
		// 0,    1,  0
		// -1 ,  0,  0

		DirectMatrix = DirectX::XMMatrixTranspose(*this);

		//float4x4 This = *this;
		//Identity();
		//for (size_t y = 0; y < YCount; y++)
		//{
		//	for (size_t x = 0; x < XCount; x++)
		//	{
		//		Arr2D[x][y] = This.Arr2D[y][x];
		//	}
		//}

	}

	void Inverse()
	{
		DirectMatrix = DirectX::XMMatrixInverse(nullptr, *this);
	}

	float4x4 InverseReturn() const
	{
		float4x4 Return = *this;
		Return.Inverse();
		return Return;
	}

	void Scale(const float4& _Value)
	{
		//100, 0 , 0 , 0
		// 0 ,100, 0 , 0
		// 0 , 0 ,100, 0
		// 0 , 0 , 0 , 1

		Identity();

		DirectMatrix = DirectX::XMMatrixScalingFromVector(_Value);


		//Arr2D[0][0] = _Value.x;
		//Arr2D[1][1] = _Value.y;
		//Arr2D[2][2] = _Value.z;
	}


	void Pos(const float4& _Value)
	{
		//  0   1   2   3
		//0 0,  0 , 0 , 0
		//1 0 , 0,  0 , 0
		//2 0 , 0 , 0 , 0
		//3 200, 200 , 200 , 1

		Identity();

		DirectMatrix = DirectX::XMMatrixTranslationFromVector(_Value);

		//Arr2D[3][0] = _Value.x;
		//Arr2D[3][1] = _Value.y;
		//Arr2D[3][2] = _Value.z;
	}

	void RotationDegToXYZ(const float4& _Deg)
	{
		float4 Rot = _Deg * GameEngineMath::DegToRad;

		float4x4 RotX = DirectX::XMMatrixRotationX(Rot.x);
		float4x4 RotY = DirectX::XMMatrixRotationY(Rot.y);
		float4x4 RotZ = DirectX::XMMatrixRotationZ(Rot.z);

		*this = RotX * RotY * RotZ;

		// DirectMatrix = DirectX::XMMatrixRotationRollPitchYaw(Rot.x, Rot.y, Rot.z);
	}

	void RotationDeg(const float4& _Deg)
	{
		// 짐벌락 현상이라는 것이 있습니다.
		// 축이 겹치는 이상한 현상이 있는데 그 현상을 해결하려면
		// 곱하는 순서를 바꿔야 해결이 된다.
		// Rot = RotX * RotY * RotZ;

		// 기본적으로 쿼터니온 회전이라는걸 사용하는데 
		// 짐벌락 해결함.
		//float4x4 RotX = float4x4::Zero;
		//float4x4 RotY = float4x4::Zero;
		//float4x4 RotZ = float4x4::Zero;
		//RotX.RotationXDeg(_Deg.x);
		//RotY.RotationYDeg(_Deg.y);
		//RotZ.RotationZDeg(_Deg.z);

		// *this = RotX * RotY * RotZ;

		float4 Rot = _Deg * GameEngineMath::DegToRad;

		// DirectX::XMQuaternionRotationMatrix()

		DirectMatrix = DirectX::XMMatrixRotationRollPitchYaw(Rot.x, Rot.y, Rot.z);
	}

	void RotationXDeg(const float _Deg)
	{
		RotationXRad(_Deg * GameEngineMath::DegToRad);
	}

	void RotationXRad(const float _Rad)
	{
		Identity();

		DirectMatrix = DirectX::XMMatrixRotationX(_Rad);


		//Arr2D[1][1] = cosf(_Rad);
		//Arr2D[1][2] = sinf(_Rad);
		//Arr2D[2][1] = -sinf(_Rad);
		//Arr2D[2][2] = cosf(_Rad);
	}


	void RotationYDeg(const float _Deg)
	{
		RotationYRad(_Deg * GameEngineMath::DegToRad);
	}

	void RotationYRad(const float _Rad)
	{
		Identity();

		DirectMatrix = DirectX::XMMatrixRotationY(_Rad);

		//Arr2D[0][0] = cosf(_Rad);
		//Arr2D[0][2] = -sinf(_Rad);
		//Arr2D[2][0] = sinf(_Rad);
		//Arr2D[2][2] = cosf(_Rad);
	}

	void RotationZDeg(const float _Deg)
	{
		RotationZRad(_Deg * GameEngineMath::DegToRad);
	}

	void RotationZRad(const float _Rad)
	{
		Identity();

		DirectMatrix = DirectX::XMMatrixRotationZ(_Rad);

		//Arr2D[0][0] = cosf(_Rad);
		//Arr2D[0][1] = sinf(_Rad);
		//Arr2D[1][0] = -sinf(_Rad);
		//Arr2D[1][1] = cosf(_Rad);
	}


	float4x4 operator*(const float4x4& _Other)
	{
		//  0   0   0   0			   		  0   0   0   0	    0   0   0   0
		//  0,  0 , 0 , 0			   		  0,  0 , 0 , 0	    0,  0 , 0 , 0
		//  0 , 0,  0 , 0          *   		  0 , 0,  0 , 0  =  0 , 0,  0 , 0
		//  0 , 0 , 0 , 0			   		  0 , 0 , 0 , 0	    0 , 0 , 0 , 0

		//this->Arr2D;
		//_Other.Arr2D;

		//float4x4 Return = Zero;
		//for (size_t y = 0; y < YCount; y++)
		//{
		//	for (size_t x = 0; x < XCount; x++)
		//	{
		//		for (size_t j = 0; j < 4; j++)
		//		{
		//			Return.Arr2D[y][x] += Arr2D[y][j] * _Other.Arr2D[j][x];
		//		}
		//	}
		//}

		float4x4 Return = DirectX::XMMatrixMultiply(*this, _Other);

		return Return;
	}

	float4x4& operator*=(const float4x4& _Other)
	{
		// *this = *this * _Other;

		DirectMatrix = DirectX::XMMatrixMultiply(*this, _Other);

		return *this;
	}

	// w 가 0인 곱하기
	float4 TransformNormal(const float4& _Value)
	{
		return DirectX::XMVector3TransformNormal(_Value, *this);
	}

	// w 가 1인 곱하기
	float4 TransformCoord(const float4& _Value)
	{
		return DirectX::XMVector3TransformCoord(_Value, *this);
	}

	operator DirectX::FXMMATRIX() const
	{
		return DirectMatrix;
	}


	float4x4()
	{
		Identity();
	}

	float4x4(DirectX::FXMMATRIX _DirectMatrix)
		: DirectMatrix(_DirectMatrix)
	{

	}

	float4x4(float4 _x, float4 _y, float4 _z, float4 _w)
	{
		ArrVector[0] = _x;
		ArrVector[1] = _y;
		ArrVector[2] = _z;
		ArrVector[3] = _w;
	}

};


