#include "PrecompileHeader.h"
#include "CustomCallback.h"

void CustomCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	while (nbPairs--)
	{
		physx::PxContactPair current = *pairs++;
		
		physx::PxShape* tmpContactActor = current.shapes[0];
		physx::PxShape* tmpOtherActor = current.shapes[1];

		physx::PxFilterData ContactFilterdata = tmpContactActor->getSimulationFilterData();
		physx::PxFilterData OtherFilterdata = tmpOtherActor->getSimulationFilterData();

		if (ContactFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::Player) &&
			OtherFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::Ground))
		{
			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
			{
				static_cast<physx::PxRigidDynamic*>(tmpContactActor->getActor());

				int a = 0;
				//Component->IsGround_true();
			}
			if (current.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				int a = 0;
				//Component->IsGround_false();
			}

		}
	}
}

void CustomCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
	while (count--)
	{
		physx::PxTriggerPair& current = *pairs++;

		// ���Ͱ� ������ �ִ� �������� ��� ������
		physx::PxRigidActor& tmpOtherActor = *current.otherActor;
		physx::PxFilterData OtherFilterdata = current.otherShape->getSimulationFilterData();
		physx::PxFilterData TriggerFilterdata = current.triggerShape->getSimulationFilterData();
		physx::PxU16 tmpnbShape = current.otherActor->getNbShapes();

		//C26813  : ��Ʈ�÷��׷� ���� enum������ �񱳴� == �� �ƴ� bitwise and(&)�� ���ϴ� ���� ����
		//WARNING : resultFd.word0 == static_cast<physx::PxU32>(PhysXFilterGroup::Ground
		//�浹ü�� filterData�� ground�̸鼭 ����� ���

		if (OtherFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::Ground) &&
			TriggerFilterdata.word0 & static_cast<physx::PxU32>(PhysXFilterGroup::Player) &&
			current.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			//if (Component != nullptr)
			//{
			//	return;
			//}
		}
	}
}