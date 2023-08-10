#include "PrecompileHeader.h"
#include "NetObjectBase.h"

NetObjectBase::NetObjectBase()
{

}

NetObjectBase::~NetObjectBase()
{

}


void NetObjectBase::Update(float _DeltaTime) 
{
	NetControllType Controll =  GetControllType();
	switch (Controll)
	{
	case NetControllType::None:
		MyUpdate(_DeltaTime);
		break;
	case NetControllType::UserControll:
		MyUpdate(_DeltaTime);
		break;
	case NetControllType::ServerControll:
		OtherUpdate(_DeltaTime);
		break;
	}
}

