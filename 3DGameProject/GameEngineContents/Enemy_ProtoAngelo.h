#pragma once

// Ό³Έν :
class Enemy_ProtoAngelo
{
public:
	// constrcuter destructer
	Enemy_ProtoAngelo();
	~Enemy_ProtoAngelo();

	// delete Function
	Enemy_ProtoAngelo(const Enemy_ProtoAngelo& _Other) = delete;
	Enemy_ProtoAngelo(Enemy_ProtoAngelo&& _Other) noexcept = delete;
	Enemy_ProtoAngelo& operator=(const Enemy_ProtoAngelo& _Other) = delete;
	Enemy_ProtoAngelo& operator=(Enemy_ProtoAngelo&& _Other) noexcept = delete;

protected:

private:

};

