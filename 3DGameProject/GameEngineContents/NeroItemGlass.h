#pragma once
#include "BaseUIActor.h"
// Ό³Έν :
class NeroItemGlass : public BaseUIActor, public GameEngineNetObject
{
public:

public:
	// constrcuter destructer
	NeroItemGlass();
	~NeroItemGlass();

	// delete Function
	NeroItemGlass(const NeroItemGlass& _Other) = delete;
	NeroItemGlass(NeroItemGlass&& _Other) noexcept = delete;
	NeroItemGlass& operator=(const NeroItemGlass& _Other) = delete;
	NeroItemGlass& operator=(NeroItemGlass&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class UIFBXRenderer> NeroUI_ItemGlass = nullptr;



};

