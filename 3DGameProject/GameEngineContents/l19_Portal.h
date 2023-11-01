#pragma once
#include "FieldMapObject.h"

class l19_Portal : public FieldMapObject
{
public:
	// construtor destructor
	l19_Portal();
	~l19_Portal();

	// delete Function
	l19_Portal(const l19_Portal& _Other) = delete;
	l19_Portal(l19_Portal&& _Other) noexcept = delete;
	l19_Portal& operator=(const l19_Portal& _Other) = delete;
	l19_Portal& operator=(l19_Portal&& _Other) noexcept = delete;

protected:
	void Start() override;

private:
	std::shared_ptr<class LevelChangeZone> AcLevelChangeZone = nullptr;
	
};

