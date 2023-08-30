#include "Transform.fx"
#include "Animation.fx"
#include "Light.fx"
#include "RenderBaseValue.fx"

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
    float4 VIEWPOSITION : POSITION;
    float4 TEXCOORD : TEXCOORD;
    float4 NORMAL : NORMAL;
    float4 TANGENT : TANGENT;
    float4 BINORMAL : BINORMAL;
};


Output MeshAniTexture_VS(Input _Input)
{
    Output NewOutPut = (Output) 0;
    
    float4 InputPos = _Input.POSITION;
    InputPos.w = 1.0f;
    
    if (IsAnimation != 0)
    {
        Skinning(InputPos, _Input.BLENDWEIGHT, _Input.BLENDINDICES, ArrAniMationMatrix);
        InputPos.w = 1.0f;
    }
    
    NewOutPut.POSITION = mul(InputPos, WorldViewProjectionMatrix);
    NewOutPut.TEXCOORD = _Input.TEXCOORD;
    
    NewOutPut.VIEWPOSITION = mul(InputPos, WorldView);
    
    _Input.NORMAL.w = 0.0f;
    NewOutPut.NORMAL = mul(_Input.NORMAL, WorldView);
    
    _Input.TANGENT.w = 0.0f;
    NewOutPut.TANGENT = mul(_Input.TANGENT, WorldView);
    
    _Input.BINORMAL.w = 0.0f;
    NewOutPut.BINORMAL = mul(_Input.BINORMAL, WorldView);

    return NewOutPut;
}

Texture2D DiffuseTexture : register(t0);
Texture2D NormalTexture : register(t1);
SamplerState ENGINEBASE : register(s0);

float4 MeshAniTexture_PS(Output _Input) : SV_Target0
{
    float4 Color = DiffuseTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    Color.a = 1.0f;
    
    if (Color.x == ClipColor.x && Color.y == ClipColor.y && Color.z == ClipColor.z)
    {
        clip(-1);
    }
    
    float4 ResultColor = Color;
        
    if (0 != IsLight)
    {
        float4 NormalDir = _Input.NORMAL;
        
        if (0 != IsNormal)
        {
            float4 BumpNormal = NormalTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
            BumpNormal = (BumpNormal * 2.0f) - 1.0f;
            BumpNormal = (BumpNormal.z * _Input.NORMAL) + (BumpNormal.x * _Input.BINORMAL) + (BumpNormal.y * _Input.TANGENT);
        
            NormalDir = BumpNormal;
        }
        
        float4 DiffuseRatio = CalDiffuseLight(_Input.VIEWPOSITION, NormalDir, AllLight[0]);
        float4 SpacularRatio = CalSpacularLight(_Input.VIEWPOSITION, NormalDir, AllLight[0]);
        float4 AmbientRatio = CalAmbientLight(AllLight[0]);
        
        float A = Color.w;
        ResultColor = Color * (DiffuseRatio + SpacularRatio + AmbientRatio);
        ResultColor.a = A;
    }
    
    
    return ResultColor;
}
