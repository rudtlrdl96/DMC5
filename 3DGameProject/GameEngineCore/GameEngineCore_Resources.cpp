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
#include "GameEngineTexture.h"
#include "GameEngineDepthState.h"
#include "GameEngineRasterizer.h"
#include "GameEngineIndexBuffer.h"
#include "GameEnginePixelShader.h"
#include "GameEngineVertexShader.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineConstantBuffer.h"
#include "GameEngineRenderingPipeLine.h"



void GameEngineCore::CoreResourcesInit()
{
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("EngineResources");
		NewDir.Move("EngineResources");

		std::vector<GameEngineFile> File = NewDir.GetAllFile({ ".Png", });


		for (size_t i = 0; i < File.size(); i++)
		{
			GameEngineTexture::Load(File[i].GetFullPath());
		}
	}


	// ���ؽ� ������ ����� ��ǲ ���̾ƿ��� ������ �� �߿��ϴ�.
	GameEngineVertex::LayOut.AddInputLayOut("POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertex::LayOut.AddInputLayOut("TEXCOORD", DXGI_FORMAT_R32G32B32A32_FLOAT);
	GameEngineVertex::LayOut.AddInputLayOut("NORMAL", DXGI_FORMAT_R32G32B32A32_FLOAT);

	//typedef struct D3D11_INPUT_ELEMENT_DESC
	//{
	//	LPCSTR SemanticName; = "POSITION"
	//	UINT SemanticIndex; = 0
	//	DXGI_FORMAT Format; 
	//	UINT InputSlot;
	//	UINT AlignedByteOffset;
	//	D3D11_INPUT_CLASSIFICATION InputSlotClass;
	//	UINT InstanceDataStepRate; 
	//} 	


	//const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
	//UINT NumElements, // 
	//const void* pShaderBytecodeWithInputSignature,  // ���̴��� ���̳ʸ� �ڵ带 ����
	//SIZE_T BytecodeLength,
	//ID3D11InputLayout** ppInputLayout // ������� ������ ������

	{
		D3D11_SAMPLER_DESC SamperData = {};

		// 
		SamperData.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamperData.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamperData.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamperData.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		// �ؽ�ó�� �ָ������� ��������
		// �ȹ�����.
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
		// �ؽ�ó�� �ָ������� ��������
		// �ȹ�����.
		SamperData.MipLODBias = 0.0f;
		SamperData.MaxAnisotropy = 1;
		SamperData.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		SamperData.MinLOD = -FLT_MAX;
		SamperData.MaxLOD = FLT_MAX;

		GameEngineSampler::Create("POINTSAMPLER", SamperData);
	}



	{
		D3D11_SAMPLER_DESC SamperData = {};

		// 
		SamperData.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SamperData.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamperData.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		SamperData.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		// �ؽ�ó�� �ָ������� ��������
		// �ȹ�����.
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
		// �ؽ�ó�� �ָ������� ��������
		// �ȹ�����.
		SamperData.MipLODBias = 0.0f;
		SamperData.MaxAnisotropy = 1;
		SamperData.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		SamperData.MinLOD = -FLT_MAX;
		SamperData.MaxLOD = FLT_MAX;

		GameEngineSampler::Create("WRAPSAMPLER", SamperData);
	}

	{
		std::vector<GameEngineVertex> ArrVertex;
		ArrVertex.resize(4);

		// 0   1
		// 3   2
		// �ո�
		ArrVertex[0] = { { -0.5f, 0.5f, 0.0f }, {0.0f, 0.0f} };
		ArrVertex[1] = { { 0.5f, 0.5f, 0.0f }, {1.0f, 0.0f} };
		ArrVertex[2] = { { 0.5f, -0.5f, 0.0f }, {1.0f, 1.0f} };
		ArrVertex[3] = { { -0.5f, -0.5f, 0.0f }, {0.0f, 1.0f} };

		std::vector<UINT> ArrIndex = { 0, 1, 2, 0, 2, 3 };

		GameEngineVertexBuffer::Create("Rect", ArrVertex);
		GameEngineIndexBuffer::Create("Rect", ArrIndex);
		GameEngineMesh::Create("Rect");
	}

	{
		std::vector<GameEngineVertex> ArrVertex;
		ArrVertex.resize(4);

		// 0   1
		// 3   2
		// �ո�
		ArrVertex[0] = { { -1.0f, 1.0f, 0.0f }, {0.0f, 0.0f} };
		ArrVertex[1] = { { 1.0f, 1.0f, 0.0f }, {1.0f, 0.0f} };
		ArrVertex[2] = { { 1.0f, -1.0f, 0.0f }, {1.0f, 1.0f} };
		ArrVertex[3] = { { -1.0f, -1.0f, 0.0f }, {0.0f, 1.0f} };

		std::vector<UINT> ArrIndex = { 0, 1, 2, 0, 2, 3 };

		GameEngineVertexBuffer::Create("FullRect", ArrVertex);
		GameEngineIndexBuffer::Create("FullRect", ArrIndex);
		GameEngineMesh::Create("FullRect");
	}


	{
		std::vector<GameEngineVertex> Vertex;
		Vertex.resize(24);
		// �ո�
		Vertex[0] = { float4(-0.5f, 0.5f, 0.5f) , float4(0.0f, 0.0f)};
		Vertex[1] = { float4(0.5f, 0.5f, 0.5f)  , float4(1.0f, 0.0f)};
		Vertex[2] = { float4(0.5f, -0.5f, 0.5f) , float4(1.0f, 1.0f)};
		Vertex[3] = { float4(-0.5f, -0.5f, 0.5f), float4(0.0f, 1.0f)};

		// �޸�
		Vertex[4] = { float4(-0.5f, 0.5f, 0.5f).RotaitonXDegReturn(180) , float4(0.0f, 0.0f)};
		Vertex[5] = { float4(0.5f, 0.5f, 0.5f).RotaitonXDegReturn(180)  , float4(1.0f, 0.0f) };
		Vertex[6] = { float4(0.5f, -0.5f, 0.5f).RotaitonXDegReturn(180) , float4(1.0f, 1.0f) };
		Vertex[7] = { float4(-0.5f, -0.5f, 0.5f).RotaitonXDegReturn(180), float4(0.0f, 1.0f) };

		// ����
		Vertex[8] = { float4(-0.5f, 0.5f, 0.5f).RotaitonYDegReturn(90) , float4(0.0f, 0.0f) };
		Vertex[9] = { float4(0.5f, 0.5f, 0.5f).RotaitonYDegReturn(90)  , float4(1.0f, 0.0f) };
		Vertex[10] = { float4(0.5f, -0.5f, 0.5f).RotaitonYDegReturn(90) , float4(1.0f, 1.0f) };
		Vertex[11] = { float4(-0.5f, -0.5f, 0.5f).RotaitonYDegReturn(90), float4(0.0f, 1.0f) };

		// ������
		Vertex[12] = { float4(-0.5f, 0.5f, 0.5f).RotaitonYDegReturn(-90) , float4(0.0f, 0.0f) };
		Vertex[13] = { float4(0.5f, 0.5f, 0.5f).RotaitonYDegReturn(-90)  , float4(1.0f, 0.0f) };
		Vertex[14] = { float4(0.5f, -0.5f, 0.5f).RotaitonYDegReturn(-90) , float4(1.0f, 1.0f) };
		Vertex[15] = { float4(-0.5f, -0.5f, 0.5f).RotaitonYDegReturn(-90), float4(0.0f, 1.0f) };


		// ��
		Vertex[16] = { float4(-0.5f, 0.5f, 0.5f).RotaitonXDegReturn(90) , float4(0.0f, 0.0f) };
		Vertex[17] = { float4(0.5f, 0.5f, 0.5f).RotaitonXDegReturn(90)  , float4(1.0f, 0.0f) };
		Vertex[18] = { float4(0.5f, -0.5f, 0.5f).RotaitonXDegReturn(90) , float4(1.0f, 1.0f) };
		Vertex[19] = { float4(-0.5f, -0.5f, 0.5f).RotaitonXDegReturn(90), float4(0.0f, 1.0f) };

		// �Ʒ�
		Vertex[20] = { float4(-0.5f, 0.5f, 0.5f).RotaitonXDegReturn(-90) , float4(0.0f, 0.0f) };
		Vertex[21] = { float4(0.5f, 0.5f, 0.5f).RotaitonXDegReturn(-90)  , float4(1.0f, 0.0f) };
		Vertex[22] = { float4(0.5f, -0.5f, 0.5f).RotaitonXDegReturn(-90) , float4(1.0f, 1.0f) };
		Vertex[23] = { float4(-0.5f, -0.5f, 0.5f).RotaitonXDegReturn(-90), float4(0.0f, 1.0f) };


		GameEngineVertexBuffer::Create("Box", Vertex);
	}

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
		std::shared_ptr<GameEngineMesh> Mesh = GameEngineMesh::Create("DebugBox", "Box", "DebugBox");
		Mesh->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	}

	
	// Sphere
		// ���Ǿ�
	{
		GameEngineVertex V;
		std::vector<GameEngineVertex> VBVector;
		std::vector<UINT> IBVector;

		float Radius = 0.5f;
		// �ϱ������� �����մϴ�.
		V.POSITION = float4(0.0f, Radius, 0.0f, 1.0f);
		V.UV = float4(0.5f, 0.0f);
		// �븻 ���� Ȥ�� �������Ͷ�� �Ҹ���
		// �鿡 ������ ���͸� �ǹ��ϰ� �ȴ�.
		// ���� �ݻ��Ҷ� �ʼ�.
		V.NORMAL = float4(0.0f, Radius, 0.0f, 1.0f);
		V.NORMAL.Normalize();
		V.NORMAL.w = 0.0f;

		VBVector.push_back(V);

		UINT iStackCount = 16; // ���� ���� �����Դϴ�.
		UINT iSliceCount = 16; // ���κ��� ����

		float yRotAngle = GameEngineMath::PIE / (float)iStackCount;
		float zRotAngle = (GameEngineMath::PIE * 2) / (float)iSliceCount;

		// UV�� ���μ��� ���ݰ��� ���Ѵ�.
		float yUvRatio = 1.0f / (float)iStackCount;
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

				V.UV = float4(yUvRatio * z, zUvRatio * y);
				V.NORMAL = V.POSITION.NormalizeReturn();
				V.NORMAL.w = 0.0f;

				VBVector.push_back(V);
			}
		}

		// ������
		V.POSITION = float4(0.0f, -Radius, 0.0f, 1.0f);
		V.UV = float4(0.5f, 1.0f);
		V.NORMAL = float4(0.0f, -Radius, 0.0f, 1.0f);
		V.NORMAL.Normalize();
		V.NORMAL.w = 0.0f;
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
		Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		GameEngineBlend::Create("AlphaBlend", Desc);
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
		Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		GameEngineBlend::Create("MergeBlend", Desc);
	}



	{
		D3D11_DEPTH_STENCIL_DESC Desc = { 0, };
		//BOOL DepthEnable;
		//D3D11_DEPTH_WRITE_MASK DepthWriteMask;
		//D3D11_COMPARISON_FUNC DepthFunc;
		//BOOL StencilEnable;
		//UINT8 StencilReadMask;
		//UINT8 StencilWriteMask;
		//D3D11_DEPTH_STENCILOP_DESC FrontFace;
		//D3D11_DEPTH_STENCILOP_DESC BackFace;

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
		// ������ ���ؽ��� ��ġ�� ���ð����̶�� �θ��ϴ�.
		std::vector<float4> ArrVertex;
		ArrVertex.resize(24);
		// �ո�
		ArrVertex[0] = { -0.5f, -0.5f, 0.5f };
		ArrVertex[1] = { 0.5f, -0.5f,0.5f };
		ArrVertex[2] = { 0.5f, 0.5f,0.5f };
		ArrVertex[3] = { -0.5f, 0.5f,0.5f };

		// �޸�
		ArrVertex[4] = ArrVertex[0].RotaitonXDegReturn(180.0f);
		ArrVertex[5] = ArrVertex[1].RotaitonXDegReturn(180.0f);
		ArrVertex[6] = ArrVertex[2].RotaitonXDegReturn(180.0f);
		ArrVertex[7] = ArrVertex[3].RotaitonXDegReturn(180.0f);

		// ���ʸ�
		ArrVertex[8] = ArrVertex[0].RotaitonYDegReturn(90.0f);
		ArrVertex[9] = ArrVertex[1].RotaitonYDegReturn(90.0f);
		ArrVertex[10] = ArrVertex[2].RotaitonYDegReturn(90.0f);
		ArrVertex[11] = ArrVertex[3].RotaitonYDegReturn(90.0f);

		// ������
		ArrVertex[12] = ArrVertex[0].RotaitonYDegReturn(-90.0f);
		ArrVertex[13] = ArrVertex[1].RotaitonYDegReturn(-90.0f);
		ArrVertex[14] = ArrVertex[2].RotaitonYDegReturn(-90.0f);
		ArrVertex[15] = ArrVertex[3].RotaitonYDegReturn(-90.0f);

		ArrVertex[16] = ArrVertex[0].RotaitonXDegReturn(90.0f);
		ArrVertex[17] = ArrVertex[1].RotaitonXDegReturn(90.0f);
		ArrVertex[18] = ArrVertex[2].RotaitonXDegReturn(90.0f);
		ArrVertex[19] = ArrVertex[3].RotaitonXDegReturn(90.0f);

		ArrVertex[20] = ArrVertex[0].RotaitonXDegReturn(-90.0f);
		ArrVertex[21] = ArrVertex[1].RotaitonXDegReturn(-90.0f);
		ArrVertex[22] = ArrVertex[2].RotaitonXDegReturn(-90.0f);
		ArrVertex[23] = ArrVertex[3].RotaitonXDegReturn(-90.0f);

	}

	// ���̴� ������
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("EngineResources");
		NewDir.Move("EngineResources");
		NewDir.Move("Shader");

		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".hlsl", ".fx" });

		// std::string FileString = Files[0].GetString();

		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineShader::AutoCompile(Files[i]);
		}

		//GameEngineVertexShader::Load(Files[0].GetFullPath(), "Merge_VS");
		//GameEnginePixelShader::Load(Files[0].GetFullPath(), "Merge_PS");

		//GameEngineVertexShader::Load(Files[1].GetFullPath(), "Texture_VS");
		//GameEnginePixelShader::Load(Files[1].GetFullPath(), "Texture_PS");
	}


	{
		D3D11_RASTERIZER_DESC Desc = {};

		//D3D11_FILL_MODE FillMode;
		// �������� �� ����� ä��� ��带 �����մϴ�( D3D11_FILL_MODE ���� ).
		//D3D11_CULL_MODE CullMode;
		// ������ ������ ���ϴ� �ﰢ���� �׷����� ������ ��Ÿ���ϴ�( D3D11_CULL_MODE ���� ).
		//BOOL FrontCounterClockwise;
		// �ﰢ���� �ո����� �޸����� �����մϴ�. 
		// �� �Ű������� TRUE �̸� ���� �� ���� ��󿡼� 
		// �ð� �ݴ� �����̸� �ﰢ���� ������ ���ϴ� ������ 
		// ���ֵǰ� �ð� �����̸� �ĸ��� ���ϴ� ������ ���ֵ˴ϴ�. 
		// �� �Ű������� FALSE �̸� �� �ݴ��Դϴ�.
		//INT DepthBias;
		// ������ �ȼ��� �߰��� ���� ���Դϴ�. ���� ���⿡ ���� ������ ���� ���� �� �����Ͻʽÿ� .
		//FLOAT DepthBiasClamp;
		// �ȼ��� �ִ� ���� ����. ���� ���⿡ ���� ������ ���� ���� �� �����Ͻʽÿ� .
		//FLOAT SlopeScaledDepthBias;
		// �־��� �ȼ��� ���⿡ ���� ��Į��. ���� ���⿡ ���� ������ ���� ���� �� �����Ͻʽÿ� .
		//BOOL DepthClipEnable;
		// �Ÿ��� ���� Ŭ������ Ȱ��ȭ�մϴ�.
		// �ϵ����� �׻� ������ȭ�� ��ǥ�� x �� y Ŭ������ �����մϴ�.
		// DepthClipEnable�� �⺻���� TRUE �� ���� �Ǹ� �ϵ��� z 
		// ���� �ڸ��ϴ�(��, �ϵ����� ���� �˰����� ������ �ܰ踦 �����մϴ�).
		//BOOL ScissorEnable;
		// ���� �簢�� �ø��� Ȱ��ȭ�մϴ�. Ȱ�� ���� �簢�� �ܺ��� ��� �ȼ��� �߷����ϴ�.
		//BOOL MultisampleEnable;
		// ���� ���� ��Ƽ�ٸ����(MSAA) ���� ��󿡼� �纯�� �Ǵ� ���� ���� ��Ƽ�ٸ���� 
		// �˰����� ������� ���θ� �����մϴ�. �纯�� ���� ��Ƽ �ٸ���� �˰����� 
		// ����Ϸ��� TRUE �� ���� �ϰ� ���� ���� ��Ƽ �ٸ���� �˰����� ����Ϸ��� 
		// FALSE �� �����մϴ�. �� ����� ���� �ڼ��� ������ ��� �����ϼ���.
		//BOOL AntialiasedLineEnable;
		// �� ��Ƽ�ٸ������ Ȱ��ȭ���� ���θ� �����մϴ�. �� �׸��⸦ �����ϰ� MultisampleEnable�� 
		// FALSE �� ��쿡�� ����˴ϴ� . �� ����� ���� �ڼ��� ������ ��� �����ϼ���.

		// ���̾� �������� ������ ǥ���ϴ� �̴ϴ�. 
		// Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		Desc.FrontCounterClockwise = FALSE;

		std::shared_ptr<GameEngineRasterizer> Res = GameEngineRasterizer::Create("Engine2DBase", Desc);
	}


	{
		// 2D�� ���� �Ž��� �߿������� �ʾƿ� Rect �Ƚ�.
		{
			std::shared_ptr<GameEngineRenderingPipeLine> Pipe = GameEngineRenderingPipeLine::Create("2DTexture");

			//Pipe->SetVertexBuffer("Rect");
			//Pipe->SetIndexBuffer("Rect");
			Pipe->SetVertexShader("TextureShader.hlsl");
			Pipe->SetRasterizer("Engine2DBase");
			Pipe->SetPixelShader("TextureShader.hlsl");
			Pipe->SetBlendState("AlphaBlend");
			Pipe->SetDepthState("EngineDepth");
		}

		{
			std::shared_ptr<GameEngineRenderingPipeLine> Pipe = GameEngineRenderingPipeLine::Create("TileMap");

			//Pipe->SetVertexBuffer("Rect");
			//Pipe->SetIndexBuffer("Rect");
			Pipe->SetVertexShader("TileMapShader.hlsl");
			Pipe->SetRasterizer("Engine2DBase");
			Pipe->SetPixelShader("TileMapShader.hlsl");
			Pipe->SetBlendState("AlphaBlend");
			Pipe->SetDepthState("EngineDepth");
		}


		{
			std::shared_ptr<GameEngineRenderingPipeLine> Pipe = GameEngineRenderingPipeLine::Create("Merge");
			//Pipe->SetVertexBuffer("FullRect");
			//Pipe->SetIndexBuffer("FullRect");
			Pipe->SetVertexShader("MergeShader.hlsl");
			Pipe->SetRasterizer("Engine2DBase");
			Pipe->SetPixelShader("MergeShader.hlsl");
			Pipe->SetBlendState("MergeBlend");
			Pipe->SetDepthState("AlwayDepth");

			GameEngineRenderTarget::RenderTargetUnitInit();
		}

		{
			std::shared_ptr<GameEngineRenderingPipeLine> Pipe = GameEngineRenderingPipeLine::Create("DebugMeshRender");
			//Pipe->SetVertexBuffer("FullRect");
			//Pipe->SetIndexBuffer("FullRect");
			Pipe->SetVertexShader("DebugMeshRender.hlsl");
			Pipe->SetRasterizer("Engine2DBase");
			Pipe->SetPixelShader("DebugMeshRender.hlsl");
			Pipe->SetBlendState("AlphaBlend");
			Pipe->SetDepthState("AlwayDepth");
		}

	}

	{
		std::shared_ptr<GameEngineRenderingPipeLine> Pipe = GameEngineRenderingPipeLine::Create("Blur");

		Pipe->SetVertexShader("BlurShader.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("BlurShader.hlsl");
		Pipe->SetBlendState("AlphaBlend");
		Pipe->SetDepthState("EngineDepth");
	}
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
	GameEngineConstantBuffer::ResourcesClear();
	GameEngineRenderingPipeLine::ResourcesClear();

}
