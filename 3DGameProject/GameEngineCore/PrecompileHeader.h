#pragma once

#include <GameEnginePlatform/PrecompileHeader.h>

#include "GameEngineDebug3D.h"
#include "GameEngineEnum.h"

#pragma warning(disable : 4996)
#include <GameEngineCore/ThirdParty/PhysX_4_1/inc/PhysX/PxConfig.h>
#include <GameEngineCore/ThirdParty/PhysX_4_1/inc/PhysX/PxPhysicsAPI.h>
#include <GameEngineCore/ThirdParty/PhysX_4_1/inc/PhysX/PxPhysXConfig.h>

#pragma comment (lib, "PhysX_static_64.lib")
#pragma comment (lib, "PhysXCharacterKinematic_static_64.lib")
#pragma comment (lib, "PhysXCommon_static_64.lib")
#pragma comment (lib, "PhysXExtensions_static_64.lib")
#pragma comment (lib, "PhysXFoundation_static_64.lib")
#pragma comment (lib, "PhysXPvdSDK_static_64.lib")
#pragma comment (lib, "PhysXCooking_static_64.lib")