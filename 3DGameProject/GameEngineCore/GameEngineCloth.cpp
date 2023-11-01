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
nv::cloth::Cloth* GameEngineCloth::m_pCloth = nullptr;
nv::cloth::PhaseConfig* GameEngineCloth::m_pPhases = nullptr;

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


}

void GameEngineCloth::CreateCloth(std::shared_ptr<GameEngineFBXRenderer> _Renderer)
{
	m_pFactory = NvClothCreateFactoryCPU();

	if (m_pFactory == nullptr)
	{
		MsgAssert("NvClothFactory 积己 角菩");
	}

	FbxRenderUnitInfo* unif = _Renderer->GetFBXMesh()->GetRenderUnit(0);
	std::map<FbxMesh*, std::vector<GameEngineVertex>*> Mapif = _Renderer->GetFBXMesh()->GetRenderUnit(0)->FbxVertexMap;

	nv::cloth::ClothMeshDesc meshDesc;

	//Fill meshDesc with data
	meshDesc.setToDefault();
	meshDesc.points.data = &unif->VertexBuffer;
	meshDesc.points.stride = unif->VertexBuffer->GetVertexSize();
	meshDesc.points.count = unif->VertexBuffer->GetVertexCount();

	//etc. for quads, triangles and invMasses
	physx::PxVec3 gravity(0.0f, -9.8f, 0.0f);
	nv::cloth::Vector<int32_t>::Type phaseTypeInfo;

	m_pFabric = NvClothCookFabricFromMesh(m_pFactory, meshDesc, gravity, &phaseTypeInfo);

	if (m_pFabric == nullptr)
	{
		MsgAssert("NvClothFabric 积己 角菩");
	}

	physx::PxVec4 particlePositions = { 1.0f, 1.0f, 1.0f, 1.0f };

	m_pCloth = m_pFactory->createCloth(nv::cloth::Range<physx::PxVec4>(&particlePositions, &particlePositions + 1), *m_pFabric);
	m_pPhases = new nv::cloth::PhaseConfig[m_pFabric->getNumPhases()];
}

void GameEngineCloth::Simulate(float _DeltaTime)
{
	for (unsigned int i = 0; i < m_pFabric->getNumPhases(); i++)
	{
		m_pPhases[i].mPhaseIndex = i; // Set index to the corresponding set (constraint group)

		//Give phases different configs depending on type
		//switch (phaseTypeInfo[i])
		//{
		//case nv::cloth::ClothFabricPhaseType::eINVALID:
		//	//ERROR
		//	break;
		//case nv::cloth::ClothFabricPhaseType::eVERTICAL:
		//	break;
		//case nv::cloth::ClothFabricPhaseType::eHORIZONTAL:
		//	break;
		//case nv::cloth::ClothFabricPhaseType::eBENDING:
		//	break;
		//case nv::cloth::ClothFabricPhaseType::eSHEARING:
		//	break;
		//}

		//For this example we give very phase the same config
		m_pPhases[i].mStiffness = 1.0f;
		m_pPhases[i].mStiffnessMultiplier = 1.0f;
		m_pPhases[i].mCompressionLimit = 1.0f;
		m_pPhases[i].mStretchLimit = 1.0f;
	}

	m_pCloth->setPhaseConfig(nv::cloth::Range<nv::cloth::PhaseConfig>(m_pPhases, m_pPhases + m_pFabric->getNumPhases()));
}

void GameEngineCloth::Release()
{
	if (nullptr != m_pFactory)
	{
		NvClothDestroyFactory(m_pFactory);
		m_pFactory = nullptr;
	}
	if (nullptr != m_pCloth)
	{
		NV_CLOTH_DELETE(m_pCloth);
		m_pCloth = nullptr;
	}
	if (nullptr != m_pPhases)
	{
		delete[] m_pPhases;
		m_pPhases = nullptr;
	}
}