#pragma once

// Ό³Έν :
class Shop_MainButton
{
public:
	// constrcuter destructer
	Shop_MainButton();
	~Shop_MainButton();

	// delete Function
	Shop_MainButton(const Shop_MainButton& _Other) = delete;
	Shop_MainButton(Shop_MainButton&& _Other) noexcept = delete;
	Shop_MainButton& operator=(const Shop_MainButton& _Other) = delete;
	Shop_MainButton& operator=(Shop_MainButton&& _Other) noexcept = delete;

protected:

private:

};

