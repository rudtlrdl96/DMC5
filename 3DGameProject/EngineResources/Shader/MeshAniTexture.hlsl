
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

cbuffer RenderBaseValue : register(b10)
{
    float DeltaTime = 0.0f;
    float SumDeltaTime = 0.0f;
    int IsAnimation = 0;
    int IsNormal = 0;
    float4 ScreenScale;
    float4 Mouse;
};


cbuffer TransformData : register(b0)
{
    float4 Scale;
    float4 Rotation;
    float4 Quaternion;
    float4 Position;

    float4 LocalScale;
    float4 LocalRotation;
    float4 LocalQuaternion;
    float4 LocalPosition;

    float4 WorldScale;
    float4 WorldRotation;
    float4 WorldQuaternion;
    float4 WorldPosition;

    float4x4 ScaleMatrix;
    float4x4 RotationMatrix;
    float4x4 PositionMatrix;
    float4x4 LocalWorldMatrix;
    float4x4 WorldMatrix;
    float4x4 View;
    float4x4 Projection;
    float4x4 ViewPort;
    float4x4 WorldViewProjectionMatrix;
}


struct Input
{
    float4 POSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
    float4 NORMAL : NORMAL;
    float4 TANGENT : TANGENT;
    float4 BINORMAL : BINORMAL;
    float4 BLENDWEIGHT : BLENDWEIGHT;
    int4 BLENDINDICES : BLENDINDICES;
};

struct Output
{
    float4 POSITION : SV_POSITION;
    float4 TEXCOORD : TEXCOORD;
};

// 그래픽카드에서 이뤄지는것.
Output MeshAniTexture_VS(Input _Input)
{
    Output NewOutPut = (Output)0;
    
    float4 InputPos = _Input.POSITION;
    InputPos.w = 1.0f;
    
    if (IsAnimation != 0)
    {
        Skinning(InputPos, _Input.BLENDWEIGHT, _Input.BLENDINDICES, ArrAniMationMatrix);
        InputPos.w = 1.0f;
    }
    // 자신의 로컬공간에서 애니메이션을 시키고
    // NewOutPut.POSITION = mul(_Input.POSITION, ArrAniMationMatrix[_Input.BLENDINDICES[0]].Mat);
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    
    return NewOutPut;
}

Texture2D DiffuseTexture : register(t0);
SamplerState ENGINEBASE : register(s0);

float4 MeshAniTexture_PS(Output _Input) : SV_Target0
{
    float4 Color = DiffuseTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    if (Color.a <= 0.0f)
    {
        clip(-1);
    }
    
    return Color;
}
