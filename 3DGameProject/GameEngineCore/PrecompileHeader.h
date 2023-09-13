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

#include "../GameEngineCore/ThirdParty/NvCloth/inc/Factory.h"
#include "../GameEngineCore/ThirdParty/NvCloth/inc/Fabric.h"
#include "../GameEngineCore/ThirdParty/NvCloth/inc/cloth.h"

#ifdef _DEBUG
#pragma comment (lib, "NvClothDEBUG_x64.lib")
#else
#pragma comment (lib, "NvCloth_x64.lib")
#endif

// 사이즈 배율
#define SIZE_MAGNIFICATION_RATIO 1.0f, 1.0f, 1.0f
#define PLAYER_SIZE_MAGNIFICATION_RATIO 3.0f, 3.0f, 3.0f

// 애니메이션 프레임시간
#define ANIMATION_FRAME_TIME 0.016666666666666666666666666666666666666666667f

#define PHYSX_GRAVITY -200.0f              // 중력 -98.1f * 0.8 * 0.8f
#define PLAYER_MAX_SPEED 40.0f             // 플레이어 최대속력
#define PLAYER_JUMP_FORCE 20.0f            // 플레이어 점프
#define PLAYER_COL_SCALE 3.0f, 5.0f, 3.0f  // 플레이어 eventCol 사이즈 
#define FLOOR_STATICFRICTION 0.7f          // 스테이지 바닥의 Staticfriction (정지마찰계수)
#define FLOOR_DYNAMICFRICTION 1.8f         // 스테이지 바닥의 Dynammicfriction (운동마찰계수)
#define FLOOR_RESISTUTION 0.0f             // 스테이지 바닥의 Resistution (반발계수)

