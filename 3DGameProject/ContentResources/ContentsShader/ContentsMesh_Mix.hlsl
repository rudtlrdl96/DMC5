#include "Transform.fx"
#include "Light.fx"
#include "RenderBaseValue.fx"
#include "Reflection.fx"

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

Texture2D DiffuseTexture : register(t0); // ALBM
Texture2D NormalTexture : register(t1);  // NRMR
Texture2D SpecularTexture : register(t2); // ATOS

SamplerState ENGINEBASE : register(s0);

float4 MeshAniTexture_PS(Output _Input) : SV_Target0
{
    float4 AlbmData = DiffuseTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    float4 NrmrData = NormalTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    float4 AtosData = SpecularTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy);
    
    if (AlbmData.x == ClipColor.x && AlbmData.y == ClipColor.y && AlbmData.z == ClipColor.z)
    {
        clip(-1);
    }
    
    float4 RGBA = { AlbmData.r, AlbmData.g, AlbmData.b, AtosData.r};
    
    RGBA.a *= 0.0f;
    RGBA.a += 1.0f;
    
    float4 ResultColor = RGBA;
    
    if (0 != IsLight)
    {
        float4 NormalDir = _Input.NORMAL;
        
        if (0 != IsNormal)
        {
            float3 BumpNormal = NrmrData.xyz;
            
            BumpNormal = (BumpNormal * 2.0f) - 1.0f;
            BumpNormal = (BumpNormal.z * _Input.NORMAL) + (BumpNormal.x * _Input.BINORMAL) + (BumpNormal.y * -_Input.TANGENT);
        
            NormalDir.xyz = BumpNormal.xyz;
        }
        
        float4 DiffuseRatio = CalDiffuseLight(_Input.VIEWPOSITION, NormalDir, AllLight[0]);
        float4 SpacularRatio = CalSpacularLight(_Input.VIEWPOSITION, NormalDir, AllLight[0]);
        float4 AmbientRatio = CalAmbientLight(AllLight[0]);
        
        float A = RGBA.w;
        ResultColor = RGBA * (DiffuseRatio + SpacularRatio + AmbientRatio);
        ResultColor.a = A;
        
        float2 RevX_UV = { -_Input.TEXCOORD.x, _Input.TEXCOORD.y };
        float4 ReflectTexture = ReflectionTexture.Sample(ENGINEBASE, NormalDir.xy);
                      
        //ResultColor.xyz -= ResultColor.xyz;
        ResultColor.xyz += ReflectTexture.xyz * AlbmData.a;
    }
            
    if (1.0f != BaseColor.a)
    {
        float Step = ((_Input.POSITION.x + (_Input.POSITION.y * 2)) % 5) + 1;
        ResultColor.a = 1.0f - ((Step / 5.0f) * (1.0f - BaseColor.a));
    }
    
    //ResultColor.xyz += ReflectionTexture.Sample(ENGINEBASE, _Input.TEXCOORD.xy).xyz ;
    
    return ResultColor;
}
