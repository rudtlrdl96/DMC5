#pragma once
class Location2_FightEvent1
{
public:
	// construtor destructor
	Location2_FightEvent1();
	~Location2_FightEvent1();

	// delete Function
	Location2_FightEvent1(const Location2_FightEvent1& _Other) = delete;
	Location2_FightEvent1(Location2_FightEvent1&& _Other) noexcept = delete;
	Location2_FightEvent1& operator=(const Location2_FightEvent1& _Other) = delete;
	Location2_FightEvent1& operator=(Location2_FightEvent1&& _Other) noexcept = delete;

protected:


private:

};
//충돌체
//길을 막는 벽들
//몬스터 카운트
//몬스터 생성위치

//1. 벽을 만들기