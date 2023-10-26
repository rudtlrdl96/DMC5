#include "PrecompileHeader.h"
#include "AttackCollision.h"

AttackCollision::AttackCollision() 
{
}

AttackCollision::~AttackCollision() 
{
	CallBack = nullptr;
	CallBack_HitStop = nullptr;
	CallBack_Parry = nullptr;
	CallBack_Damage = nullptr;
}

