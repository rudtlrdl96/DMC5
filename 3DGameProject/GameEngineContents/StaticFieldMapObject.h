#pragma once
#include "FieldMapObject.h"

class StaticFieldMapObject : public FieldMapObject
{
public:
	// construtor destructor
	StaticFieldMapObject();
	~StaticFieldMapObject();

	// delete Function
	StaticFieldMapObject(const StaticFieldMapObject& _Other) = delete;
	StaticFieldMapObject(StaticFieldMapObject&& _Other) noexcept = delete;
	StaticFieldMapObject& operator=(const StaticFieldMapObject& _Other) = delete;
	StaticFieldMapObject& operator=(StaticFieldMapObject&& _Other) noexcept = delete;


protected:
	
	void Update(float _DeltaTime) override;

	void MapObj_FBXMesh(const std::string_view& _Name, const std::string_view& _Mat);

	std::shared_ptr<class GameEngineFBXRenderer> FBXMesh = nullptr;
	std::shared_ptr<class PhysXBoxComponent> PhysXBox = nullptr;

	std::string FBXFileName = "";

private:

};

