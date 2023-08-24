#pragma once

class ContentDefine
{
public:
	static const physx::PxVec3 Capsul_Small;
	static const physx::PxVec3 Capsul_Middle;
	static const physx::PxVec3 Capsul_Large;

	static const float Mess_Small;
	static const float Mess_Middle;
	static const float Mess_Large;


private:
	// constrcuter destructer
	ContentDefine();
	~ContentDefine();

	// delete Function
	ContentDefine(const ContentDefine& _Other) = delete;
	ContentDefine(ContentDefine&& _Other) noexcept = delete;
	ContentDefine& operator=(const ContentDefine& _Other) = delete;
	ContentDefine& operator=(ContentDefine&& _Other) noexcept = delete;
};

