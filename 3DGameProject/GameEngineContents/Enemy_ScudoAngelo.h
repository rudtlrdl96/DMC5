#pragma once

// Ό³Έν :
class Enemy_ScudoAngelo
{
public:
	// constrcuter destructer
	Enemy_ScudoAngelo();
	~Enemy_ScudoAngelo();

	// delete Function
	Enemy_ScudoAngelo(const Enemy_ScudoAngelo& _Other) = delete;
	Enemy_ScudoAngelo(Enemy_ScudoAngelo&& _Other) noexcept = delete;
	Enemy_ScudoAngelo& operator=(const Enemy_ScudoAngelo& _Other) = delete;
	Enemy_ScudoAngelo& operator=(Enemy_ScudoAngelo&& _Other) noexcept = delete;

protected:

private:

};

