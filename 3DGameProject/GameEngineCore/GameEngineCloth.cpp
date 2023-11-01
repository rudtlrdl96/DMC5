#include "PrecompileHeader.h"
#include "GameEngineCloth.h"

#include "GameEngineDevice.h"
#include "GameEngineFBXRenderer.h"

#include "../GameEngineCore/ThirdParty/NvCloth/inc/Factory.h"
#include "../GameEngineCore/ThirdParty/NvCloth/inc/Fabric.h"
#include "../GameEngineCore/ThirdParty/NvCloth/inc/cloth.h"
#include "../NvCloth/extensions/ClothFabricCooker.h"

nv::cloth::Factory* GameEngineCloth::m_pFactory = nullptr;
nv::cloth::Fabric* GameEngineCloth::m_pFabric = nullptr;

GameEngineCloth::GameEngineCloth() 
{
}

GameEngineCloth::~GameEngineCloth() 
{
}

void GameEngineCloth::CreateFactory()
{
	////Setup DX11 context
	//ID3D11Device* DXDevice;
	//ID3D11DeviceContext* DXDeviceContext;
	//nv::cloth::DxContextManagerCallback* GraphicsContextManager;
	//D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	//D3D_FEATURE_LEVEL featureLevelResult;
	//HRESULT result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, 1, D3D11_SDK_VERSION, &DXDevice, &featureLevelResult, &DXDeviceContext);
	//ASSERT(result == S_OK);
	//ASSERT(featureLevelResult == D3D_FEATURE_LEVEL_11_0);
	//GraphicsContextManager = new DxContextManagerCallbackImpl(DXDevice);
	//ASSERT(GraphicsContextManager != nullptr);

	//nv::cloth::Factory* factory = NvClothCreateFactoryDX11(GraphicsContextManager);
	////We need to release all DX objects after destroying the factory.

	m_pFactory = NvClothCreateFactoryCPU();

	if (m_pFactory == nullptr)
	{
		MsgAssert("NvClothFactory 생성 실패");
	}
}

void GameEngineCloth::CreateCloth(std::shared_ptr<GameEngineFBXRenderer> _Renderer)
{
	std::vector<std::vector<std::shared_ptr<GameEngineRenderUnit>>> Unit = _Renderer->GetAllRenderUnit();

	nv::cloth::ClothMeshDesc meshDesc;

	//Fill meshDesc with data
	meshDesc.setToDefault();
	//meshDesc.points.data = vertexArray;
	//meshDesc.points.stride = sizeof(vertexArray[0]);
	//meshDesc.points.count = vertexCount;

	//etc. for quads, triangles and invMasses
	physx::PxVec3 gravity(0.0f, -9.8f, 0.0f);
	nv::cloth::Vector<int32_t>::Type phaseTypeInfo;

	m_pFabric = NvClothCookFabricFromMesh(m_pFactory, meshDesc, gravity, &phaseTypeInfo);
}

void GameEngineCloth::Release()
{
	if (nullptr != m_pFactory)
	{
		NvClothDestroyFactory(m_pFactory);
	}
}