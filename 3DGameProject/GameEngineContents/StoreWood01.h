#pragma once
#include "StaticFieldMapObject.h"
class StoreWood01 : public StaticFieldMapObject
{
public:
	// construtor destructor
	StoreWood01();
	~StoreWood01();

	// delete Function
	StoreWood01(const StoreWood01& _Other) = delete;
	StoreWood01(StoreWood01&& _Other) noexcept = delete;
	StoreWood01& operator=(const StoreWood01& _Other) = delete;
	StoreWood01& operator=(StoreWood01&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

