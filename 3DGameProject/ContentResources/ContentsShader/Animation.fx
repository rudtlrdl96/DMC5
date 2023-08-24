struct AniMat
{
    float4x4 Mat;
};

// 상수버퍼로 안되냐?
// 상수버퍼는 동적 할당이 안된다.
// 용량제한이 700바이트 정도 밖에 안된다.
// 애니메이션을 하는데 필요한게 행렬 200~300
// 64

// 1프레임에 해당하는 모든본의 행렬입니다.
StructuredBuffer<AniMat> ArrAniMationMatrix : register(t11);

// inout c++로 치면 레퍼런스
// float4&
void Skinning(inout float4 _Pos, inout float4 _Weight, inout int4 _Index, StructuredBuffer<AniMat> _ArrMatrix)
{
    float4 CalPos = (float4) 0.0f;
    // 1로 맞추려는 행동.
    // _Weight[3] = 1.0f - _Weight[0] - _Weight[1] - _Weight[2];
    _Pos.w = 1.0f;
    
    for (int i = 0; i < 4; ++i)
    {
        AniMat Mat = _ArrMatrix[_Index[i]];
        CalPos += _Weight[i] * mul(_Pos, Mat.Mat);
    }
    
    _Pos = CalPos;
    _Pos.w = 1.0f;

}

void SkinningNormal(inout float4 _Normal, inout float4 _Weight, inout int4 _Index, StructuredBuffer<AniMat> _ArrMatrix)
{
    float4 CalPos = (float4) 0.0f;
    // 1로 맞추려는 행동.
    // _Weight[3] = 1.0f - _Weight[0] - _Weight[1] - _Weight[2];
    
    _Normal.w = 1.0f;
    
    for (int i = 0; i < 4; ++i)
    {
        AniMat Mat = _ArrMatrix[_Index[i]];
        CalPos += _Weight[i] * mul(_Normal, Mat.Mat);
    }
    
    _Normal = CalPos;
    _Normal.xyz = normalize(_Normal.xyz);
    _Normal.w = 0.0f;

}
