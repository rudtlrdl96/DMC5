#pragma once


class Poolable
{
public:
	Poolable();
	~Poolable();

	Poolable(const Poolable& _Other) = delete;
	Poolable(Poolable&& _Other) noexcept = delete;
	Poolable& operator=(const Poolable& _Other) = delete;
	Poolable& operator=(const Poolable&& _Other) noexcept = delete;

protected:

private:

};

