#pragma once
#include "BaseStageActor.h"

class FieldMapObject : public BaseStageActor
{
public:
	// construtor destructor
	FieldMapObject();
	~FieldMapObject();

	// delete Function
	FieldMapObject(const FieldMapObject& _Other) = delete;
	FieldMapObject(FieldMapObject&& _Other) noexcept = delete;
	FieldMapObject& operator=(const FieldMapObject& _Other) = delete;
	FieldMapObject& operator=(FieldMapObject&& _Other) noexcept = delete;
	
	static std::shared_ptr<FieldMapObject> CreateFieldMapObj(GameEngineLevel* _Level, FieldMapObjType _FieldMapObjType, const class ObjTransformData& _ObjTransform);

protected:

private:

};

