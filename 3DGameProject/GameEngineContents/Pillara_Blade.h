#pragma once
#include "StaticFieldMapObject.h"
class Pillara_Blade : public StaticFieldMapObject
{
public:
	// construtor destructor
	Pillara_Blade();
	~Pillara_Blade();

	// delete Function
	Pillara_Blade(const Pillara_Blade& _Other) = delete;
	Pillara_Blade(Pillara_Blade&& _Other) noexcept = delete;
	Pillara_Blade& operator=(const Pillara_Blade& _Other) = delete;
	Pillara_Blade& operator=(Pillara_Blade&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:

};

