#include "PrecompileHeader.h"
#include "GameEngineTimeEvent.h"

GameEngineTimeEvent::GameEngineTimeEvent() 
{
}

GameEngineTimeEvent::~GameEngineTimeEvent() 
{
}

void GameEngineTimeEvent::AddEvent(float _Time, std::function<void(TimeEvent&, GameEngineTimeEvent*)> _Event, bool _Loop /*= false*/)
{
	// 기본 생성자가 있으면
	// 그냥 자기가 내부에서 기본생성자 가지고 만들어주는 것.
	TimeEvent& NewEvent = Events.emplace_back();
	NewEvent.CurTime = _Time;
	NewEvent.Time = _Time;
	NewEvent.Loop = _Loop;
	NewEvent.Event = _Event;
}

void GameEngineTimeEvent::Update(float _DeltaTime)
{
	// [1][2][4][5][6][7][]
	std::list<TimeEvent>::iterator StartIter = Events.begin();
	std::list<TimeEvent>::iterator EndIter = Events.end();

	for (; StartIter != EndIter;)
	{
		TimeEvent& CurEvent = *StartIter;

		CurEvent.CurTime -= _DeltaTime;

		if (CurEvent.CurTime < 0.0f)
		{
			CurEvent.Event(CurEvent, this);
			CurEvent.CurTime = CurEvent.Time;

			if (false == CurEvent.Loop)
			{
				StartIter = Events.erase(StartIter);
				continue;
			}
		}

		++StartIter;
	}
}