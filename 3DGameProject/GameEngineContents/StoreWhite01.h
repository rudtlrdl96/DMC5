#pragma once
#include "StaticFieldMapObject.h"
class StoreWhite01 : public StaticFieldMapObject
{
public:
	// construtor destructor
	StoreWhite01();
	~StoreWhite01();

	// delete Function
	StoreWhite01(const StoreWhite01& _Other) = delete;
	StoreWhite01(StoreWhite01&& _Other) noexcept = delete;
	StoreWhite01& operator=(const StoreWhite01& _Other) = delete;
	StoreWhite01& operator=(StoreWhite01&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};

