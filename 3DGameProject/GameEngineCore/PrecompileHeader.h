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

// ������ ����
#define SIZE_MAGNIFICATION_RATIO 1.0f, 1.0f, 1.0f
#define PLAYER_SIZE_MAGNIFICATION_RATIO 3.0f, 3.0f, 3.0f

// �ִϸ��̼� �����ӽð�
#define ANIMATION_FRAME_TIME 0.016666666666666666666666666666666666666666667f

#define PHYSX_GRAVITY -200.0f              // �߷� -98.1f * 0.8 * 0.8f
#define PLAYER_MAX_SPEED 40.0f             // �÷��̾� �ִ�ӷ�
#define PLAYER_JUMP_FORCE 20.0f            // �÷��̾� ����
#define PLAYER_COL_SCALE 3.0f, 5.0f, 3.0f  // �÷��̾� eventCol ������ 
#define FLOOR_STATICFRICTION 0.7f          // �������� �ٴ��� Staticfriction (�����������)
#define FLOOR_DYNAMICFRICTION 1.8f         // �������� �ٴ��� Dynammicfriction (��������)
#define FLOOR_RESISTUTION 0.0f             // �������� �ٴ��� Resistution (�ݹ߰��)

