#include "PrecompileHeader.h"
#include "GameEngineCore.h"

#include <GameEngineBase\GameEngineDebug.h>
#include <GameEnginePlatform\GameEngineWindow.h>
#include <GameEnginePlatform\GameEngineSound.h>

#include "GameEngineResource.h"
#include "GameEngineShaderResHelper.h"
#include "GameEngineVertex.h"
#include "GameEngineMesh.h"
#include "GameEngineBlend.h"
#include "GameEngineFBXMesh.h"
#include "GameEngineTexture.h"
#include "GameEngineDepthState.h"
#include "GameEngineRasterizer.h"
#include "GameEngineIndexBuffer.h"
#include "GameEnginePixelShader.h"
#include "GameEngineVertexShader.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineConstantBuffer.h"
#include "GameEngineFBXAnimation.h"
#include "GameEngineMaterial.h"
#include "GameEngineOption.h"



void GameEngineCore::CoreResourcesInit()
{
	// 엔진 기본 리소스 로드
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("EngineResources");
		NewDir.Move("EngineResources");

		std::vector<GameEngineFile> File = NewDir.GetAllFile({ ".Png", ".Tga"});

		for (size_t i = 0; i < File.size(); i++)
		{
			GameEngineTexture::Load(File[i].GetFullPath());
		}
	}

	{
		std::shared_ptr<GameEngineTexture> BaseCubeTexture = GameEngineTexture::Find("EngineBaseCubemap.png");
		std::vector<std::shared_ptr<GameEngineTexture>> CubemapRes;

		CubemapRes.reserve(6);
		CubemapRes.push_back(BaseCubeTexture);
		CubemapRes.push_back(BaseCubeTexture);
		CubemapRes.push_back(BaseCubeTexture);
		CubemapRes.push_back(BaseCubeTexture);
		CubemapRes.push_back(BaseCubeTexture);
		CubemapRes.push_back(BaseCubeTexture);

		GameEngineTexture::Create("BaseCubemap", CubemapRes);
	}

	{
		std::shared_ptr<GameEngineTexture> BaseCubeTexture = GameEngineTexture::Find("EngineBaseCubeShadow.png");
		std::vector<std::shared_ptr<GameEngineTexture>> CubemapRes;

		CubemapRes.reserve(6);
		CubemapRes.push_back(BaseCubeTexture);
		CubemapRes.push_back(BaseCubeTexture);
		CubemapRes.push_back(BaseCubeTexture);
		CubemapRes.push_back(BaseCubeTexture);
		CubemapRes.push_back(BaseCubeTexture);
		CubemapRes.push_back(BaseCubeTexture);

		GameEngineTexture::Create("BaseShadowCubemap", CubemapRes);
	}

	//static std::shared_ptr<GameEngineTexture> Create(
	//	const std::string_view & _Name,
	//	std::vector<std::shared_ptr<GameEngineTexture>>&_Textures,
	//	const D3D11_TEXTURE2D_DESC & _Value,
	//	const D3D11_RENDER_TARGET_VIEW_DESC & _RTV,
	//	const D3D11_SHADER_RESOURCE_VIEW_DESC & _SRV
	//);


	// GameEngineInputLayOutInfo에 SemanticName, Format 결정
	GameEngineVertex::LayOut.AddInputLayOut("POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertex::LayOut.AddInputLayOut("TEXCOORD", DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertex::LayOut.AddInputLayOut("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertex::LayOut.AddInputLayOut("NORMAL", DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertex::LayOut.AddInputLayOut("BINORMAL", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertex::LayOut.AddInputLayOut("TANGENT", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertex::LayOut.AddInputLayOut("BLENDWEIGHT", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertex::LayOut.AddInputLayOut("BLENDINDICES", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_SINT);

	// 파티클 설정
	GameEngineVertexParticle::LayOut.AddInputLayOut("POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertexParticle::LayOut.AddInputLayOut("PINDEX", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_SINT); // 48

	// 샘플러
	{
		D3D11_SAMPLER_DESC SamperData = {};

		SamperData.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamperData.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamperData.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamperData.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamperData.MipLODBias = 0.0f;
		SamperData.MaxAnisotropy = 1;
		SamperData.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		SamperData.MinLOD = -FLT_MAX;
		SamperData.MaxLOD = FLT_MAX;

		GameEngineSampler::Create("ENGINEBASE", SamperData);
	}
	{
		D3D11_SAMPLER_DESC SamperData = {};

		SamperData.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		SamperData.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamperData.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamperData.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamperData.MipLODBias = 0.0f;
		SamperData.MaxAnisotropy = 1;
		SamperData.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		SamperData.MinLOD = -FLT_MAX;
		SamperData.MaxLOD = FLT_MAX;

		GameEngineSampler::Create("POINTSAMPLER", SamperData);
	}
	{
		D3D11_SAMPLER_DESC SamperData = {};

		SamperData.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamperData.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamperData.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamperData.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamperData.MipLODBias = 0.0f;
		SamperData.MaxAnisotropy = 1;
		SamperData.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		SamperData.MinLOD = -FLT_MAX;
		SamperData.MaxLOD = FLT_MAX;

		GameEngineSampler::Create("CLAMPSAMPLER", SamperData);
	}
	{
		D3D11_SAMPLER_DESC SamperData = {};

		SamperData.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamperData.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamperData.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamperData.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		SamperData.MipLODBias = 0.0f;
		SamperData.MaxAnisotropy = 1;
		SamperData.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		SamperData.MinLOD = -FLT_MAX;
		SamperData.MaxLOD = FLT_MAX;

		GameEngineSampler::Create("WRAPSAMPLER", SamperData);
	}	
	
	{
		D3D11_SAMPLER_DESC SamperData = {};

		SamperData.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamperData.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		SamperData.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		SamperData.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamperData.MipLODBias = 0.0f;
		SamperData.MaxAnisotropy = 1;
		SamperData.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		SamperData.MinLOD = 0.0f;
		SamperData.MaxLOD = 0.0f;

		GameEngineSampler::Create("CUBEMAPSAMPLER", SamperData);
	}

	// 기본 메쉬 생성, Rect
	{
		std::vector<GameEngineVertex> ArrVertex;
		ArrVertex.resize(4);

		ArrVertex[0] = { { -0.5f, 0.5f, 0.0f }, {0.0f, 0.0f} };
		ArrVertex[1] = { { 0.5f, 0.5f, 0.0f }, {1.0f, 0.0f} };
		ArrVertex[2] = { { 0.5f, -0.5f, 0.0f }, {1.0f, 1.0f} };
		ArrVertex[3] = { { -0.5f, -0.5f, 0.0f }, {0.0f, 1.0f} };

		std::vector<UINT> ArrIndex = { 0, 1, 2, 0, 2, 3 };

		GameEngineVertexBuffer::Create("Rect", ArrVertex);
		GameEngineIndexBuffer::Create("Rect", ArrIndex);
		GameEngineMesh::Create("Rect");
	}

	// FullRect
	{
		std::vector<GameEngineVertex> ArrVertex;
		ArrVertex.resize(4);

		ArrVertex[0] = { { -1.0f, 1.0f, 0.0f }, {0.0f, 0.0f} };
		ArrVertex[1] = { { 1.0f, 1.0f, 0.0f }, {1.0f, 0.0f} };
		ArrVertex[2] = { { 1.0f, -1.0f, 0.0f }, {1.0f, 1.0f} };
		ArrVertex[3] = { { -1.0f, -1.0f, 0.0f }, {0.0f, 1.0f} };

		std::vector<UINT> ArrIndex = { 0, 1, 2, 0, 2, 3 };

		GameEngineVertexBuffer::Create("FullRect", ArrVertex);
		GameEngineIndexBuffer::Create("FullRect", ArrIndex);
		GameEngineMesh::Create("FullRect");
	}

	// GridRect
	{
		std::vector<GameEngineVertex> ArrVertex;
		ArrVertex.resize(4);

		ArrVertex[0] = { { -0.5f, 0.0f, 0.5f }, {0.0f, 0.0f} };
		ArrVertex[1] = { { 0.5f, 0.0f, 0.5f }, {1.0f, 0.0f} };
		ArrVertex[2] = { { 0.5f, 0.0f, -0.5f }, {1.0f, 1.0f} };
		ArrVertex[3] = { { -0.5f, 0.0f, -0.5f }, {0.0f, 1.0f} };

		std::vector<UINT> ArrIndex = { 0, 1, 2, 0, 2, 3 };

		GameEngineVertexBuffer::Create("GridRect", ArrVertex);
		GameEngineIndexBuffer::Create("GridRect", ArrIndex);
		GameEngineMesh::Create("GridRect");
	}

	// Box
	{
		std::vector<GameEngineVertex> Vertex;
		Vertex.resize(24);
		// 앞면
		Vertex[0] = { float4(-0.5f, 0.5f, 0.5f) , float4(0.0f, 0.0f), float4::RED, float4(0.0f, 0.0f, 1.0f) };
		Vertex[1] = { float4(0.5f, 0.5f, 0.5f)  , float4(1.0f, 0.0f), float4::RED, float4(0.0f, 0.0f, 1.0f) };
		Vertex[2] = { float4(0.5f, -0.5f, 0.5f) , float4(1.0f, 1.0f), float4::RED, float4(0.0f, 0.0f, 1.0f) };
		Vertex[3] = { float4(-0.5f, -0.5f, 0.5f), float4(0.0f, 1.0f), float4::RED, float4(0.0f, 0.0f, 1.0f) };

		// 뒷면
		Vertex[4] = { float4(-0.5f, 0.5f, 0.5f).RotationXDegReturn(180) , float4(0.0f, 0.0f), float4::RED, float4(0.0f, 0.0f, -1.0f) };
		Vertex[5] = { float4(0.5f, 0.5f, 0.5f).RotationXDegReturn(180)  , float4(1.0f, 0.0f), float4::RED, float4(0.0f, 0.0f, -1.0f) };
		Vertex[6] = { float4(0.5f, -0.5f, 0.5f).RotationXDegReturn(180) , float4(1.0f, 1.0f), float4::RED, float4(0.0f, 0.0f, -1.0f) };
		Vertex[7] = { float4(-0.5f, -0.5f, 0.5f).RotationXDegReturn(180), float4(0.0f, 1.0f), float4::RED, float4(0.0f, 0.0f, -1.0f) };

		// 왼쪽
		Vertex[8] = { float4(-0.5f, 0.5f, 0.5f).RotationYDegReturn(-90) , float4(0.0f, 0.0f), float4::RED, float4(1.0f, 0.0f, 0.0f) };
		Vertex[9] = { float4(0.5f, 0.5f, 0.5f).RotationYDegReturn(-90)  , float4(1.0f, 0.0f), float4::RED, float4(1.0f, 0.0f, 0.0f) };
		Vertex[10] = { float4(0.5f, -0.5f, 0.5f).RotationYDegReturn(-90) , float4(1.0f, 1.0f), float4::RED, float4(1.0f, 0.0f, 0.0f) };
		Vertex[11] = { float4(-0.5f, -0.5f, 0.5f).RotationYDegReturn(-90), float4(0.0f, 1.0f), float4::RED, float4(1.0f, 0.0f, 0.0f) };

		// 오른쪽
		Vertex[12] = { float4(-0.5f, 0.5f, 0.5f).RotationYDegReturn(90) , float4(0.0f, 0.0f), float4::RED, float4(-1.0f, 0.0f, 0.0f) };
		Vertex[13] = { float4(0.5f, 0.5f, 0.5f).RotationYDegReturn(90)  , float4(1.0f, 0.0f), float4::RED, float4(-1.0f, 0.0f, 0.0f) };
		Vertex[14] = { float4(0.5f, -0.5f, 0.5f).RotationYDegReturn(90) , float4(1.0f, 1.0f), float4::RED, float4(-1.0f, 0.0f, 0.0f) };
		Vertex[15] = { float4(-0.5f, -0.5f, 0.5f).RotationYDegReturn(90), float4(0.0f, 1.0f), float4::RED, float4(-1.0f, 0.0f, 0.0f) };

		// 위
		Vertex[16] = { float4(-0.5f, 0.5f, 0.5f).RotationXDegReturn(-90) , float4(0.0f, 0.0f), float4::RED, float4(0.0f, 1.0f, 0.0f) };
		Vertex[17] = { float4(0.5f, 0.5f, 0.5f).RotationXDegReturn(-90)  , float4(1.0f, 0.0f), float4::RED, float4(0.0f, 1.0f, 0.0f) };
		Vertex[18] = { float4(0.5f, -0.5f, 0.5f).RotationXDegReturn(-90) , float4(1.0f, 1.0f), float4::RED, float4(0.0f, 1.0f, 0.0f) };
		Vertex[19] = { float4(-0.5f, -0.5f, 0.5f).RotationXDegReturn(-90), float4(0.0f, 1.0f), float4::RED, float4(0.0f, 1.0f, 0.0f) };

		// 아래
		Vertex[20] = { float4(-0.5f, 0.5f, 0.5f).RotationXDegReturn(90) , float4(0.0f, 0.0f), float4::RED, float4(1.0f, -1.0f, 0.0f) };
		Vertex[21] = { float4(0.5f, 0.5f, 0.5f).RotationXDegReturn(90)  , float4(1.0f, 0.0f), float4::RED, float4(1.0f, -1.0f, 0.0f) };
		Vertex[22] = { float4(0.5f, -0.5f, 0.5f).RotationXDegReturn(90) , float4(1.0f, 1.0f), float4::RED, float4(1.0f, -1.0f, 0.0f) };
		Vertex[23] = { float4(-0.5f, -0.5f, 0.5f).RotationXDegReturn(90), float4(0.0f, 1.0f), float4::RED, float4(1.0f, -1.0f, 0.0f) };

		GameEngineVertexBuffer::Create("Box", Vertex);
	}

	// Full Box
	{
		std::vector<GameEngineVertex> Vertex;
		Vertex.resize(24);

		// 오른쪽
		Vertex[0] = {{ -1.0f, 1.0f, 0.0f }, float4(-1.0f, 1.0f,  1.0f).RotationYDegReturn(90)};
		Vertex[1] = {{ 1.0f, 1.0f, 0.0f  }, float4(1.0f,  1.0f,  1.0f).RotationYDegReturn(90)};
		Vertex[2] = {{ 1.0f, -1.0f, 0.0f }, float4(1.0f,  -1.0f, 1.0f).RotationYDegReturn(90)};
		Vertex[3] = {{ -1.0f, -1.0f, 0.0f}, float4(-1.0f, -1.0f, 1.0f).RotationYDegReturn(90)};

		// 왼쪽
		Vertex[4] = {{ -1.0f, 1.0f, 0.0f }, float4(-1.0f, 1.0f,  1.0f).RotationYDegReturn(-90)};
		Vertex[5] = {{ 1.0f, 1.0f, 0.0f  }, float4(1.0f,  1.0f,  1.0f).RotationYDegReturn(-90)};
		Vertex[6] = {{ 1.0f, -1.0f, 0.0f }, float4(1.0f,  -1.0f, 1.0f).RotationYDegReturn(-90)};
		Vertex[7] = {{ -1.0f, -1.0f, 0.0f}, float4(-1.0f, -1.0f, 1.0f).RotationYDegReturn(-90)};

		// 위
		Vertex[8] =  {{ -1.0f, 1.0f, 0.0f }, float4(-1.0f, 1.0f,  1.0f).RotationXDegReturn(-90)};
		Vertex[9] =  {{ 1.0f, 1.0f, 0.0f  }, float4(1.0f,  1.0f,  1.0f).RotationXDegReturn(-90)};
		Vertex[10] = {{ 1.0f, -1.0f, 0.0f }, float4(1.0f,  -1.0f, 1.0f).RotationXDegReturn(-90)};
		Vertex[11] = {{ -1.0f, -1.0f, 0.0f}, float4(-1.0f, -1.0f, 1.0f).RotationXDegReturn(-90)};

		// 아래
		Vertex[12] = {{ -1.0f, 1.0f, 0.0f }, float4(-1.0f, 1.0f,  1.0f).RotationXDegReturn(90)};
		Vertex[13] = {{ 1.0f, 1.0f, 0.0f  }, float4(1.0f,  1.0f,  1.0f).RotationXDegReturn(90)};
		Vertex[14] = {{ 1.0f, -1.0f, 0.0f }, float4(1.0f,  -1.0f, 1.0f).RotationXDegReturn(90)};
		Vertex[15] = {{ -1.0f, -1.0f, 0.0f}, float4(-1.0f, -1.0f, 1.0f).RotationXDegReturn(90)};

		// 앞면
		Vertex[16] = {{ -1.0f, 1.0f, 0.0f }, float4(-1.0f, 1.0f,  1.0f)};
		Vertex[17] = {{ 1.0f, 1.0f, 0.0f  }, float4(1.0f,  1.0f,  1.0f)};
		Vertex[18] = {{ 1.0f, -1.0f, 0.0f }, float4(1.0f,  -1.0f, 1.0f)};
		Vertex[19] = {{ -1.0f, -1.0f, 0.0f}, float4(-1.0f, -1.0f, 1.0f)};

		// 뒷면
		Vertex[20] = {{ -1.0f, 1.0f, 0.0f }, float4(1.0f, -1.0f,  1.0f).RotationXDegReturn(180)};
		Vertex[21] = {{ 1.0f, 1.0f, 0.0f  }, float4(-1.0f,  -1.0f,  1.0f).RotationXDegReturn(180)};
		Vertex[22] = {{ 1.0f, -1.0f, 0.0f }, float4(-1.0f,  1.0f, 1.0f).RotationXDegReturn(180)};
		Vertex[23] = {{ -1.0f, -1.0f, 0.0f}, float4(1.0f, 1.0f, 1.0f).RotationXDegReturn(180)};

		GameEngineVertexBuffer::Create("FullBox", Vertex);
	}

	// DebugBox
	{
		std::vector<int> Index;

		Index.push_back(0);
		Index.push_back(1);
		Index.push_back(1);
		Index.push_back(2);
		Index.push_back(2);
		Index.push_back(3);
		Index.push_back(3);
		Index.push_back(0);

		Index.push_back(4 + 0);
		Index.push_back(4 + 1);
		Index.push_back(4 + 1);
		Index.push_back(4 + 2);
		Index.push_back(4 + 2);
		Index.push_back(4 + 3);
		Index.push_back(4 + 3);
		Index.push_back(4 + 0);

		Index.push_back(8 + 0);
		Index.push_back(8 + 1);
		Index.push_back(8 + 1);
		Index.push_back(8 + 2);
		Index.push_back(8 + 2);
		Index.push_back(8 + 3);
		Index.push_back(8 + 3);
		Index.push_back(8 + 0);

		Index.push_back(12 + 0);
		Index.push_back(12 + 1);
		Index.push_back(12 + 1);
		Index.push_back(12 + 2);
		Index.push_back(12 + 2);
		Index.push_back(12 + 3);
		Index.push_back(12 + 3);
		Index.push_back(12 + 0);

		GameEngineIndexBuffer::Create("DebugBox", Index);
	}

	// TriAngleBox
	{
		std::vector<int> Index;

		for (int i = 0; i < 6; i++)
		{
			Index.push_back(i * 4 + 2);
			Index.push_back(i * 4 + 1);
			Index.push_back(i * 4 + 0);

			Index.push_back(i * 4 + 3);
			Index.push_back(i * 4 + 2);
			Index.push_back(i * 4 + 0);
		}

		GameEngineIndexBuffer::Create("TriAngleBox", Index);
	}

	{
		std::shared_ptr<GameEngineMesh> Mesh = GameEngineMesh::Create("DebugBox", "Box", "DebugBox");
		Mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	}

	{
		std::shared_ptr<GameEngineMesh> Mesh = GameEngineMesh::Create("Box", "Box", "TriAngleBox");
		Mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		
	}	
	
	{
		std::shared_ptr<GameEngineMesh> Mesh = GameEngineMesh::Create("FullBox", "FullBox", "TriAngleBox");
		Mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		
	}

	// Sphere
	{
		GameEngineVertex V;
		std::vector<GameEngineVertex> VBVector;
		std::vector<UINT> IBVector;

		float Radius = 0.5f;
		
		V.POSITION = float4(0.0f, Radius, 0.0f, 1.0f); // 북극점부터 시작
		V.TEXCOORD = float4(0.5f, 0.0f);
		V.NORMAL = float4(0.0f, Radius, 0.0f, 1.0f); // 노말 백터 혹은 법선백터(면에 수직인 벡터)라고 불리며 빛 반사에서 필수로 사용됨
		V.NORMAL.Normalize();
		V.NORMAL.w = 0.0f;
		V.TANGENT = float4(1.0f, 0.0f, 0.0f, 0.0f);
		V.BINORMAL = float4(0.0f, 0.0f, 1.0f, 0.0f);

		VBVector.push_back(V);

		UINT iStackCount = 16; // 가로 분할 개수입니다.
		UINT iSliceCount = 16; // 세로분할 개수

		float yRotAngle = GameEngineMath::PIE / (float)iStackCount;
		float zRotAngle = (GameEngineMath::PIE * 2) / (float)iSliceCount;

		float yUvRatio = 1.0f / (float)iStackCount; // UV의 가로세로 간격값을 구한다.
		float zUvRatio = 1.0f / (float)iStackCount;

		for (UINT y = 1; y < iStackCount; ++y)
		{
			// 각 간격에 대한 각도값
			float phi = y * yRotAngle;
			for (UINT z = 0; z < iSliceCount + 1; ++z)
			{
				float theta = z * zRotAngle;
				V.POSITION = float4{
					Radius * sinf(y * yRotAngle) * cosf(z * zRotAngle),
					Radius * cosf(y * yRotAngle),
					Radius * sinf(y * yRotAngle) * sinf(z * zRotAngle),
					1.0f // 위치 크기 값에 영향을 주기 위해서
				};

				// V.Pos *= GameEngineRandom::RandomFloat(-0.9f, 0.1f);

				V.TEXCOORD = float4(yUvRatio * z, zUvRatio * y);
				V.NORMAL = V.POSITION.NormalizeReturn();
				V.NORMAL.w = 0.0f;

				V.TANGENT.x = -Radius * sinf(phi) * sinf(theta);
				V.TANGENT.y = 0.0f;
				V.TANGENT.z = Radius * sinf(phi) * cosf(theta);
				V.TANGENT = V.TANGENT.NormalizeReturn();
				V.TANGENT.w = 0.0f;

				V.BINORMAL = float4::Cross3DReturn(V.TANGENT, V.NORMAL);
				V.BINORMAL = V.BINORMAL.NormalizeReturn();
				V.BINORMAL.w = 0.0f;

				VBVector.push_back(V);
			}
		}

		// 남극점
		V.POSITION = float4(0.0f, -Radius, 0.0f, 1.0f);
		V.TEXCOORD = float4(0.5f, 1.0f);
		V.NORMAL = float4(0.0f, -Radius, 0.0f, 1.0f);
		V.NORMAL.Normalize();
		V.NORMAL.w = 0.0f;
		V.TANGENT = float4(-1.0f, 0.0f, 0.0f, 0.0f);
		V.BINORMAL = float4(0.0f, 0.0f, -1.0f, 0.0f);

		VBVector.push_back(V);

		// 인덱스 버퍼를 만듭니다.
		IBVector.clear();

		// 북극점을 이루는 점을 만드는건.
		for (UINT i = 0; i < iSliceCount; i++)
		{
			// 시작은 무조건 북극점
			IBVector.push_back(0);
			IBVector.push_back(i + 2);
			IBVector.push_back(i + 1);
		}

		for (UINT y = 0; y < iStackCount - 2; y++)
		{
			for (UINT z = 0; z < iSliceCount; z++)
			{
				IBVector.push_back((iSliceCount + 1) * y + z + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + z + 1);

				IBVector.push_back((iSliceCount + 1) * y + z + 1);
				IBVector.push_back((iSliceCount + 1) * y + (z + 1) + 1);
				IBVector.push_back((iSliceCount + 1) * (y + 1) + (z + 1) + 1);

			}
		}

		// 마지막으로 남극점 인덱스
		UINT iBotIndex = (UINT)VBVector.size() - 1;
		for (UINT i = 0; i < iSliceCount; i++)
		{
			// 시작은 무조건 북극점
			IBVector.push_back(iBotIndex);
			IBVector.push_back(iBotIndex - (i + 2));
			IBVector.push_back(iBotIndex - (i + 1));
		}

		GameEngineVertexBuffer::Create("Sphere", VBVector);
		GameEngineIndexBuffer::Create("Sphere", IBVector);

		GameEngineMesh::Create("Sphere");

		std::shared_ptr<GameEngineMesh> Mesh = GameEngineMesh::Create("DebugSphere", "Sphere", "Sphere");
		Mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	}

	// 블랜드
	{
		D3D11_BLEND_DESC Desc = { 0, };

		// Desc.AlphaToCoverageEnable = false; == 자동으로 알파부분을 제거하여 출력해준다. 너무 느려서 사용 안함.

		Desc.AlphaToCoverageEnable = false; 
		Desc.IndependentBlendEnable = false; // 블랜드를 여러개 넣을 것인지 판단, true면 여러 개 넣는 것, false면 랜더타겟이 몇개이건 0번에 세팅된 걸로 전부 다 블랜드

		Desc.RenderTarget[0].BlendEnable = true;
		Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		GameEngineBlend::Create("BaseBlend", Desc);
	}

	{
		D3D11_BLEND_DESC Desc = { 0, };

		Desc.AlphaToCoverageEnable = false;
		Desc.IndependentBlendEnable = false;

		Desc.RenderTarget[0].BlendEnable = true;
		Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		GameEngineBlend::Create("MergeBlend", Desc);
	}

	{
		D3D11_BLEND_DESC Desc = { 0, };

		Desc.AlphaToCoverageEnable = false;
		Desc.IndependentBlendEnable = false;

		Desc.RenderTarget[0].BlendEnable = true;
		Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX;
		Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

		Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		GameEngineBlend::Create("MaxMergeBlend", Desc);
	}

	{
		D3D11_BLEND_DESC Desc = { 0, };

		Desc.AlphaToCoverageEnable = false;
		Desc.IndependentBlendEnable = false;

		Desc.RenderTarget[0].BlendEnable = true;
		Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

		Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		GameEngineBlend::Create("LightBlend", Desc);
	}
	{
		D3D11_BLEND_DESC Desc = { 0, };

		Desc.AlphaToCoverageEnable = false;
		Desc.IndependentBlendEnable = false;

		Desc.RenderTarget[0].BlendEnable = true;
		Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

		Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		GameEngineBlend::Create("OneBlend", Desc);
	}

	{
		// 블랜드
		D3D11_BLEND_DESC Desc = { 0, };

		// 자동으로 알파부분을 제거해서 출력해주는 건데
		// 졸라느립니다.
		// Desc.AlphaToCoverageEnable = false;

		// 
		Desc.AlphaToCoverageEnable = false;
		// 블랜드를 여러개 넣을거냐
		// TRUE면 블랜드를 여러개 넣습니다.
		// false면 몇개의 랜더타겟이 있건 0번에 세팅된 걸로 전부다 블랜드.
		Desc.IndependentBlendEnable = false;

		Desc.RenderTarget[0].BlendEnable = true;
		Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MIN;
		Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

		Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		GameEngineBlend::Create("MinBlend", Desc);
	}

	// 뎁스 생성
	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0, };

		Desc.DepthEnable = true;
		Desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		Desc.StencilEnable = false;

		GameEngineDepthState::Create("EngineDepth", Desc);
	}
	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0, };

		Desc.DepthEnable = true;
		Desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		Desc.StencilEnable = false;

		GameEngineDepthState::Create("AlwayDepth", Desc);
	}
	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0, };

		Desc.DepthEnable = true;
		Desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		Desc.StencilEnable = false;

		GameEngineDepthState::Create("AlphaDepth", Desc);
	}


	// 쉐이더 컴파일
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("EngineResources");
		NewDir.Move("EngineResources");
		NewDir.Move("Shader");

		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".hlsl", ".fx" });

		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineShader::AutoCompile(Files[i]);
		}
	}

	// 레스터라이저 세팅
	{
		D3D11_RASTERIZER_DESC Desc = {};

		// D3D11_FILL_MODE FillMode; == 렌더링할 때 사용할 채우기 모드를 결정합니다( D3D11_FILL_MODE 참조 ).
		// D3D11_CULL_MODE CullMode; == 지정된 방향을 향하는 삼각형이 그려지지 않음을 나타냅니다( D3D11_CULL_MODE 참조 ).
		// BOOL FrontCounterClockwise; == 삼각형이 앞면인지 뒷면인지 결정합니다. 
		//								  이 매개변수가 TRUE 이면 정점 이 렌더 대상에서 
		//								  시계 반대 방향이면 삼각형이 전면을 향하는 것으로 
		//								  간주되고 시계 방향이면 후면을 향하는 것으로 간주됩니다. 
		//								  이 매개변수가 FALSE 이면 그 반대입니다.
		// INT DepthBias; == 지정된 픽셀에 추가된 깊이 값입니다. 깊이 편향에 대한 정보는 깊이 편향 을 참조하십시오 .
		// FLOAT DepthBiasClamp; == 픽셀의 최대 깊이 편향. 깊이 편향에 대한 정보는 깊이 편향 을 참조하십시오 .
		// FLOAT SlopeScaledDepthBias; == 주어진 픽셀의 기울기에 대한 스칼라. 깊이 편향에 대한 정보는 깊이 편향 을 참조하십시오 .
		// BOOL DepthClipEnable; == 거리에 따라 클리핑을 활성화합니다.
		//                          하드웨어는 항상 래스터화된 좌표의 x 및 y 클리핑을 수행합니다.
		//                          DepthClipEnable이 기본값인 TRUE 로 설정 되면 하드웨어도 z 
		//                          값을 자릅니다(즉, 하드웨어는 다음 알고리즘의 마지막 단계를 수행합니다).
		// BOOL ScissorEnable; == 가위 사각형 컬링을 활성화합니다. 활성 가위 사각형 외부의 모든 픽셀이 추려집니다.
		// // BOOL MultisampleEnable; == 다중 샘플 앤티앨리어싱(MSAA) 렌더 대상에서 사변형 또는 알파 라인 앤티앨리어싱 
		//                            알고리즘을 사용할지 여부를 지정합니다. 사변형 라인 앤티 앨리어싱 알고리즘을 
		//                            사용하려면 TRUE 로 설정 하고 알파 라인 앤티 앨리어싱 알고리즘을 사용하려면 
		//                            FALSE 로 설정합니다. 이 멤버에 대한 자세한 내용은 비고를 참조하세요.
		// BOOL AntialiasedLineEnable; == 선 앤티앨리어싱을 활성화할지 여부를 지정합니다. 선 그리기를 수행하고 MultisampleEnable이 
		//                                FALSE 인 경우에만 적용됩니다 . 이 멤버에 대한 자세한 내용은 비고를 참조하세요.

		// 와이어 프레임은 선으로 표현하는 겁니다. 
		// Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Desc.FrontCounterClockwise = FALSE;

		std::shared_ptr<GameEngineRasterizer> Res = GameEngineRasterizer::Create("Engine2DBase", Desc);
	}

	// 레스터라이저 세팅
	{
		D3D11_RASTERIZER_DESC Desc = {};

		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		Desc.FrontCounterClockwise = FALSE;

		std::shared_ptr<GameEngineRasterizer> Res = GameEngineRasterizer::Create("Engine3DBase", Desc);
	}

	// 파이프라인(머티리얼) 생성
	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("2DTexture");

		Pipe->SetVertexShader("TextureShader.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("TextureShader.hlsl");
		Pipe->SetBlendState("BaseBlend");
		Pipe->SetDepthState("EngineDepth");
	}
	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("TileMap");

		Pipe->SetVertexShader("TileMapShader.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("TileMapShader.hlsl");
		Pipe->SetBlendState("BaseBlend");
		Pipe->SetDepthState("EngineDepth");
	}

	{
		{
			std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("Merge");

			Pipe->SetVertexShader("MergeShader.hlsl");
			Pipe->SetRasterizer("Engine2DBase");
			Pipe->SetPixelShader("MergeShader.hlsl");
			Pipe->SetBlendState("MergeBlend");
			Pipe->SetDepthState("AlwayDepth");
		}		
		
		{
			std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("MaxMerge");

			Pipe->SetVertexShader("MergeShader.hlsl");
			Pipe->SetRasterizer("Engine2DBase");
			Pipe->SetPixelShader("MergeShader.hlsl");
			Pipe->SetBlendState("MaxMergeBlend");
			Pipe->SetDepthState("AlwayDepth");
		}

		{
			std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("CubemapMerge");

			Pipe->SetVertexShader("CubemapMergeShader.hlsl");
			Pipe->SetRasterizer("Engine2DBase");
			Pipe->SetPixelShader("CubemapMergeShader.hlsl");
			Pipe->SetBlendState("MergeBlend");
			Pipe->SetDepthState("AlwayDepth");
		}

		GameEngineRenderTarget::RenderTargetUnitInit();
	}

	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("DebugMeshRender");

		Pipe->SetVertexShader("DebugMeshRender.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("DebugMeshRender.hlsl");
		Pipe->SetBlendState("BaseBlend");
		Pipe->SetDepthState("AlwayDepth");
	}
	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("MeshTexture");

		Pipe->SetVertexShader("MeshTexture.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("MeshTexture.hlsl");
		Pipe->SetBlendState("BaseBlend");
		Pipe->SetDepthState("EngineDepth");
	}
	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("Blur");

		Pipe->SetVertexShader("BlurShader.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("BlurShader.hlsl");
		Pipe->SetBlendState("BaseBlend");
		Pipe->SetDepthState("AlwayDepth");
	}
	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("Bloom");

		Pipe->SetVertexShader("BloomShader.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("BloomShader.hlsl");
		Pipe->SetBlendState("OneBlend");
		Pipe->SetDepthState("AlwayDepth");
	}
	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("BloomBlur");

		Pipe->SetVertexShader("BloomBlur.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("BloomBlur.hlsl");
		Pipe->SetBlendState("OneBlend");
		Pipe->SetDepthState("AlwayDepth");
	}
	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("MeshAniTexture");
		
		Pipe->SetVertexShader("MeshAniTexture.hlsl");
		Pipe->SetRasterizer("Engine3DBase");
		Pipe->SetPixelShader("MeshAniTexture.hlsl");
		Pipe->SetBlendState("BaseBlend");
		Pipe->SetDepthState("EngineDepth");
	}
	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("MeshColor");

		Pipe->SetVertexShader("MeshColor.hlsl");
		Pipe->SetRasterizer("Engine3DBase");
		Pipe->SetPixelShader("MeshColor.hlsl");
		Pipe->SetBlendState("BaseBlend");
		Pipe->SetDepthState("EngineDepth");
	}
	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("GridShader");

		Pipe->SetVertexShader("GridShader.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("GridShader.hlsl");
		Pipe->SetBlendState("BaseBlend");
		Pipe->SetDepthState("EngineDepth");
	}
	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("MeshAniTextureDeferred");

		Pipe->SetVertexShader("MeshAniTextureDeferred.hlsl");
		Pipe->SetRasterizer("Engine3DBase");
		Pipe->SetPixelShader("MeshAniTextureDeferred.hlsl");
		Pipe->SetBlendState("BaseBlend");
		Pipe->SetDepthState("EngineDepth");
	}
	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("DeferredCalLight");

		Pipe->SetVertexShader("DeferredCalLight.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("DeferredCalLight.hlsl");
		Pipe->SetBlendState("LightBlend");
		Pipe->SetDepthState("AlwayDepth");
	}
	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("DeferredMerge");

		Pipe->SetVertexShader("DeferredMerge.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("DeferredMerge.hlsl");
		Pipe->SetBlendState("MergeBlend");
		Pipe->SetDepthState("AlwayDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("OShadow");
		Pipe->SetVertexShader("OrthogonalShadow.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("OrthogonalShadow.hlsl");
		Pipe->SetBlendState("MinBlend");
		Pipe->SetDepthState("AlwayDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("PShadow");
		Pipe->SetVertexShader("PerspectiveShadow.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("PerspectiveShadow.hlsl");
		Pipe->SetBlendState("MinBlend");
		Pipe->SetDepthState("AlwayDepth");
	}
}

void GameEngineCore::CoreOptionInit()
{
	// 텍스쳐 품질
	GameEngineOption::CreateOption("Texture", GameEngineOptionValue::High);

	// 쉐이더 적용
	GameEngineOption::CreateOption("Shader", GameEngineOptionValue::High);
}

void GameEngineCore::CoreResourcesEnd()
{
	GameEngineMesh::ResourcesClear();
	GameEngineBlend::ResourcesClear();
	GameEngineSound::ResourcesClear();
	GameEngineTexture::ResourcesClear();
	GameEngineDepthState::ResourcesClear();
	GameEngineRasterizer::ResourcesClear();
	GameEngineIndexBuffer::ResourcesClear();
	GameEnginePixelShader::ResourcesClear();
	GameEngineVertexShader::ResourcesClear();
	GameEngineVertexBuffer::ResourcesClear();
	GameEngineRenderTarget::ResourcesClear();
	GameEngineFBXMesh::ResourcesClear();
	GameEngineFBXAnimation::ResourcesClear();
	GameEngineConstantBuffer::ResourcesClear();
	GameEngineMaterial::ResourcesClear();
}
