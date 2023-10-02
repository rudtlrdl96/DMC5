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
	// ���� �⺻ ���ҽ� �ε�
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


	// GameEngineInputLayOutInfo�� SemanticName, Format ����
	GameEngineVertex::LayOut.AddInputLayOut("POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertex::LayOut.AddInputLayOut("TEXCOORD", DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertex::LayOut.AddInputLayOut("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertex::LayOut.AddInputLayOut("NORMAL", DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertex::LayOut.AddInputLayOut("BINORMAL", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertex::LayOut.AddInputLayOut("TANGENT", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertex::LayOut.AddInputLayOut("BLENDWEIGHT", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertex::LayOut.AddInputLayOut("BLENDINDICES", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_SINT);

	// ���÷�
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

	// �⺻ �޽� ����, Rect
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
		// �ո�
		Vertex[0] = { float4(-0.5f, 0.5f, 0.5f) , float4(0.0f, 0.0f), float4::RED, float4(0.0f, 0.0f, 1.0f) };
		Vertex[1] = { float4(0.5f, 0.5f, 0.5f)  , float4(1.0f, 0.0f), float4::RED, float4(0.0f, 0.0f, 1.0f) };
		Vertex[2] = { float4(0.5f, -0.5f, 0.5f) , float4(1.0f, 1.0f), float4::RED, float4(0.0f, 0.0f, 1.0f) };
		Vertex[3] = { float4(-0.5f, -0.5f, 0.5f), float4(0.0f, 1.0f), float4::RED, float4(0.0f, 0.0f, 1.0f) };

		// �޸�
		Vertex[4] = { float4(-0.5f, 0.5f, 0.5f).RotationXDegReturn(180) , float4(0.0f, 0.0f), float4::RED, float4(0.0f, 0.0f, -1.0f) };
		Vertex[5] = { float4(0.5f, 0.5f, 0.5f).RotationXDegReturn(180)  , float4(1.0f, 0.0f), float4::RED, float4(0.0f, 0.0f, -1.0f) };
		Vertex[6] = { float4(0.5f, -0.5f, 0.5f).RotationXDegReturn(180) , float4(1.0f, 1.0f), float4::RED, float4(0.0f, 0.0f, -1.0f) };
		Vertex[7] = { float4(-0.5f, -0.5f, 0.5f).RotationXDegReturn(180), float4(0.0f, 1.0f), float4::RED, float4(0.0f, 0.0f, -1.0f) };

		// ����
		Vertex[8] = { float4(-0.5f, 0.5f, 0.5f).RotationYDegReturn(-90) , float4(0.0f, 0.0f), float4::RED, float4(1.0f, 0.0f, 0.0f) };
		Vertex[9] = { float4(0.5f, 0.5f, 0.5f).RotationYDegReturn(-90)  , float4(1.0f, 0.0f), float4::RED, float4(1.0f, 0.0f, 0.0f) };
		Vertex[10] = { float4(0.5f, -0.5f, 0.5f).RotationYDegReturn(-90) , float4(1.0f, 1.0f), float4::RED, float4(1.0f, 0.0f, 0.0f) };
		Vertex[11] = { float4(-0.5f, -0.5f, 0.5f).RotationYDegReturn(-90), float4(0.0f, 1.0f), float4::RED, float4(1.0f, 0.0f, 0.0f) };

		// ������
		Vertex[12] = { float4(-0.5f, 0.5f, 0.5f).RotationYDegReturn(90) , float4(0.0f, 0.0f), float4::RED, float4(-1.0f, 0.0f, 0.0f) };
		Vertex[13] = { float4(0.5f, 0.5f, 0.5f).RotationYDegReturn(90)  , float4(1.0f, 0.0f), float4::RED, float4(-1.0f, 0.0f, 0.0f) };
		Vertex[14] = { float4(0.5f, -0.5f, 0.5f).RotationYDegReturn(90) , float4(1.0f, 1.0f), float4::RED, float4(-1.0f, 0.0f, 0.0f) };
		Vertex[15] = { float4(-0.5f, -0.5f, 0.5f).RotationYDegReturn(90), float4(0.0f, 1.0f), float4::RED, float4(-1.0f, 0.0f, 0.0f) };

		// ��
		Vertex[16] = { float4(-0.5f, 0.5f, 0.5f).RotationXDegReturn(-90) , float4(0.0f, 0.0f), float4::RED, float4(0.0f, 1.0f, 0.0f) };
		Vertex[17] = { float4(0.5f, 0.5f, 0.5f).RotationXDegReturn(-90)  , float4(1.0f, 0.0f), float4::RED, float4(0.0f, 1.0f, 0.0f) };
		Vertex[18] = { float4(0.5f, -0.5f, 0.5f).RotationXDegReturn(-90) , float4(1.0f, 1.0f), float4::RED, float4(0.0f, 1.0f, 0.0f) };
		Vertex[19] = { float4(-0.5f, -0.5f, 0.5f).RotationXDegReturn(-90), float4(0.0f, 1.0f), float4::RED, float4(0.0f, 1.0f, 0.0f) };

		// �Ʒ�
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
		
		// ������
		Vertex[0] = {{ -1.0f, 1.0f, 0.0f }, float4(-1.0f, 1.0f, 1.0f).RotationYDegReturn(90) };
		Vertex[1] = {{ -1.0f, 1.0f, 0.0f }, float4(1.0f, 1.0f, 1.0f).RotationYDegReturn(90)  };
		Vertex[2] = {{ -1.0f, 1.0f, 0.0f }, float4(1.0f, -1.0f, 1.0f).RotationYDegReturn(90) };
		Vertex[3] = {{ -1.0f, 1.0f, 0.0f }, float4(-1.0f, -1.0f, 1.0f).RotationYDegReturn(90)};

		// ����
		Vertex[4] = {{ -1.0f, 1.0f, 0.0f }, float4(-1.0f, 1.0f, 1.0f).RotationYDegReturn(-90) };
		Vertex[5] = {{ -1.0f, 1.0f, 0.0f }, float4(1.0f, 1.0f, 1.0f).RotationYDegReturn(-90)  };
		Vertex[6] = {{ -1.0f, 1.0f, 0.0f }, float4(1.0f, -1.0f, 1.0f).RotationYDegReturn(-90) };
		Vertex[7] = {{ -1.0f, 1.0f, 0.0f }, float4(-1.0f, -1.0f, 1.0f).RotationYDegReturn(-90)};

		// ��
		Vertex[8] =  {{ -1.0f, 1.0f, 0.0f }, float4(-1.0f, 1.0f, 1.0f).RotationXDegReturn(-90) };
		Vertex[9] =  {{ -1.0f, 1.0f, 0.0f }, float4(1.0f, 1.0f, 1.0f).RotationXDegReturn(-90)  };
		Vertex[10] = {{ -1.0f, 1.0f, 0.0f }, float4(1.0f, -1.0f, 1.0f).RotationXDegReturn(-90) };
		Vertex[11] = {{ -1.0f, 1.0f, 0.0f }, float4(-1.0f, -1.0f, 1.0f).RotationXDegReturn(-90)};

		// �Ʒ�
		Vertex[12] = {{ -1.0f, 1.0f, 0.0f }, float4(-1.0f, 1.0f, 1.0f).RotationXDegReturn(90) };
		Vertex[13] = {{ -1.0f, 1.0f, 0.0f }, float4(1.0f, 1.0f, 1.0f).RotationXDegReturn(90)  };
		Vertex[14] = {{ -1.0f, 1.0f, 0.0f }, float4(1.0f, -1.0f, 1.0f).RotationXDegReturn(90) };
		Vertex[15] = {{ -1.0f, 1.0f, 0.0f }, float4(-1.0f, -1.0f, 1.0f).RotationXDegReturn(90)};

		// �ո�
		Vertex[16] = {{ -1.0f, 1.0f, 0.0f }, float4(-1.0f, 1.0f, 1.0f) };
		Vertex[17] = {{ -1.0f, 1.0f, 0.0f }, float4(1.0f, 1.0f, 1.0f)  };
		Vertex[18] = {{ -1.0f, 1.0f, 0.0f }, float4(1.0f, -1.0f, 1.0f) };
		Vertex[19] = {{ -1.0f, 1.0f, 0.0f }, float4(-1.0f, -1.0f, 1.0f)};

		// �޸�
		Vertex[20] = {{ -1.0f, 1.0f, 0.0f }, float4(-1.0f, 1.0f, 1.0f).RotationXDegReturn(180) };
		Vertex[21] = {{ -1.0f, 1.0f, 0.0f }, float4(1.0f, 1.0f, 1.0f).RotationXDegReturn(180)  };
		Vertex[22] = {{ -1.0f, 1.0f, 0.0f }, float4(1.0f, -1.0f, 1.0f).RotationXDegReturn(180) };
		Vertex[23] = {{ -1.0f, 1.0f, 0.0f }, float4(-1.0f, -1.0f, 1.0f).RotationXDegReturn(180)};

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
		
		V.POSITION = float4(0.0f, Radius, 0.0f, 1.0f); // �ϱ������� ����
		V.TEXCOORD = float4(0.5f, 0.0f);
		V.NORMAL = float4(0.0f, Radius, 0.0f, 1.0f); // �븻 ���� Ȥ�� ��������(�鿡 ������ ����)��� �Ҹ��� �� �ݻ翡�� �ʼ��� ����
		V.NORMAL.Normalize();
		V.NORMAL.w = 0.0f;
		V.TANGENT = float4(1.0f, 0.0f, 0.0f, 0.0f);
		V.BINORMAL = float4(0.0f, 0.0f, 1.0f, 0.0f);

		VBVector.push_back(V);

		UINT iStackCount = 16; // ���� ���� �����Դϴ�.
		UINT iSliceCount = 16; // ���κ��� ����

		float yRotAngle = GameEngineMath::PIE / (float)iStackCount;
		float zRotAngle = (GameEngineMath::PIE * 2) / (float)iSliceCount;

		float yUvRatio = 1.0f / (float)iStackCount; // UV�� ���μ��� ���ݰ��� ���Ѵ�.
		float zUvRatio = 1.0f / (float)iStackCount;

		for (UINT y = 1; y < iStackCount; ++y)
		{
			// �� ���ݿ� ���� ������
			float phi = y * yRotAngle;
			for (UINT z = 0; z < iSliceCount + 1; ++z)
			{
				float theta = z * zRotAngle;
				V.POSITION = float4{
					Radius * sinf(y * yRotAngle) * cosf(z * zRotAngle),
					Radius * cosf(y * yRotAngle),
					Radius * sinf(y * yRotAngle) * sinf(z * zRotAngle),
					1.0f // ��ġ ũ�� ���� ������ �ֱ� ���ؼ�
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

		// ������
		V.POSITION = float4(0.0f, -Radius, 0.0f, 1.0f);
		V.TEXCOORD = float4(0.5f, 1.0f);
		V.NORMAL = float4(0.0f, -Radius, 0.0f, 1.0f);
		V.NORMAL.Normalize();
		V.NORMAL.w = 0.0f;
		V.TANGENT = float4(-1.0f, 0.0f, 0.0f, 0.0f);
		V.BINORMAL = float4(0.0f, 0.0f, -1.0f, 0.0f);

		VBVector.push_back(V);

		// �ε��� ���۸� ����ϴ�.
		IBVector.clear();

		// �ϱ����� �̷�� ���� ����°�.
		for (UINT i = 0; i < iSliceCount; i++)
		{
			// ������ ������ �ϱ���
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

		// ���������� ������ �ε���
		UINT iBotIndex = (UINT)VBVector.size() - 1;
		for (UINT i = 0; i < iSliceCount; i++)
		{
			// ������ ������ �ϱ���
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

	// ����
	{
		D3D11_BLEND_DESC Desc = { 0, };

		// Desc.AlphaToCoverageEnable = false; == �ڵ����� ���ĺκ��� �����Ͽ� ������ش�. �ʹ� ������ ��� ����.

		Desc.AlphaToCoverageEnable = false; 
		Desc.IndependentBlendEnable = false; // ���带 ������ ���� ������ �Ǵ�, true�� ���� �� �ִ� ��, false�� ����Ÿ���� ��̰� 0���� ���õ� �ɷ� ���� �� ����

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
		// ����
		D3D11_BLEND_DESC Desc = { 0, };

		// �ڵ����� ���ĺκ��� �����ؼ� ������ִ� �ǵ�
		// ��������ϴ�.
		// Desc.AlphaToCoverageEnable = false;

		// 
		Desc.AlphaToCoverageEnable = false;
		// ���带 ������ �����ų�
		// TRUE�� ���带 ������ �ֽ��ϴ�.
		// false�� ��� ����Ÿ���� �ְ� 0���� ���õ� �ɷ� ���δ� ����.
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

	// ���� ����
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


	// ���̴� ������
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

	// �����Ͷ����� ����
	{
		D3D11_RASTERIZER_DESC Desc = {};

		// D3D11_FILL_MODE FillMode; == �������� �� ����� ä��� ��带 �����մϴ�( D3D11_FILL_MODE ���� ).
		// D3D11_CULL_MODE CullMode; == ������ ������ ���ϴ� �ﰢ���� �׷����� ������ ��Ÿ���ϴ�( D3D11_CULL_MODE ���� ).
		// BOOL FrontCounterClockwise; == �ﰢ���� �ո����� �޸����� �����մϴ�. 
		//								  �� �Ű������� TRUE �̸� ���� �� ���� ��󿡼� 
		//								  �ð� �ݴ� �����̸� �ﰢ���� ������ ���ϴ� ������ 
		//								  ���ֵǰ� �ð� �����̸� �ĸ��� ���ϴ� ������ ���ֵ˴ϴ�. 
		//								  �� �Ű������� FALSE �̸� �� �ݴ��Դϴ�.
		// INT DepthBias; == ������ �ȼ��� �߰��� ���� ���Դϴ�. ���� ���⿡ ���� ������ ���� ���� �� �����Ͻʽÿ� .
		// FLOAT DepthBiasClamp; == �ȼ��� �ִ� ���� ����. ���� ���⿡ ���� ������ ���� ���� �� �����Ͻʽÿ� .
		// FLOAT SlopeScaledDepthBias; == �־��� �ȼ��� ���⿡ ���� ��Į��. ���� ���⿡ ���� ������ ���� ���� �� �����Ͻʽÿ� .
		// BOOL DepthClipEnable; == �Ÿ��� ���� Ŭ������ Ȱ��ȭ�մϴ�.
		//                          �ϵ����� �׻� ������ȭ�� ��ǥ�� x �� y Ŭ������ �����մϴ�.
		//                          DepthClipEnable�� �⺻���� TRUE �� ���� �Ǹ� �ϵ��� z 
		//                          ���� �ڸ��ϴ�(��, �ϵ����� ���� �˰����� ������ �ܰ踦 �����մϴ�).
		// BOOL ScissorEnable; == ���� �簢�� �ø��� Ȱ��ȭ�մϴ�. Ȱ�� ���� �簢�� �ܺ��� ��� �ȼ��� �߷����ϴ�.
		// // BOOL MultisampleEnable; == ���� ���� ��Ƽ�ٸ����(MSAA) ���� ��󿡼� �纯�� �Ǵ� ���� ���� ��Ƽ�ٸ���� 
		//                            �˰����� ������� ���θ� �����մϴ�. �纯�� ���� ��Ƽ �ٸ���� �˰����� 
		//                            ����Ϸ��� TRUE �� ���� �ϰ� ���� ���� ��Ƽ �ٸ���� �˰����� ����Ϸ��� 
		//                            FALSE �� �����մϴ�. �� ����� ���� �ڼ��� ������ ��� �����ϼ���.
		// BOOL AntialiasedLineEnable; == �� ��Ƽ�ٸ������ Ȱ��ȭ���� ���θ� �����մϴ�. �� �׸��⸦ �����ϰ� MultisampleEnable�� 
		//                                FALSE �� ��쿡�� ����˴ϴ� . �� ����� ���� �ڼ��� ������ ��� �����ϼ���.

		// ���̾� �������� ������ ǥ���ϴ� �̴ϴ�. 
		// Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Desc.FrontCounterClockwise = FALSE;

		std::shared_ptr<GameEngineRasterizer> Res = GameEngineRasterizer::Create("Engine2DBase", Desc);
	}

	// �����Ͷ����� ����
	{
		D3D11_RASTERIZER_DESC Desc = {};

		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		Desc.FrontCounterClockwise = FALSE;

		std::shared_ptr<GameEngineRasterizer> Res = GameEngineRasterizer::Create("Engine3DBase", Desc);
	}

	// ����������(��Ƽ����) ����
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
	// �ؽ��� ǰ��
	GameEngineOption::CreateOption("Texture", GameEngineOptionValue::High);

	// ���̴� ����
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
