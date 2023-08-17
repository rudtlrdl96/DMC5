#pragma once
#include <GameEngineCore/GameEngineGUI.h>
#include <GameEngineBase/GameEngineNetServer.h>
#include <GameEngineBase/GameEngineNetClient.h>

#include "NetLogFSM.h"

class GameEngineNet;

class NetLogViewer : public GameEngineGUIWindow
{
	friend class NetLogState_EntryReady;

public:
	static inline NetLogViewer* GetInst()
	{
		return ViewerInst;
	}

public:
	NetLogViewer();
	~NetLogViewer();

	NetLogViewer(const NetLogViewer& _Other) = delete;
	NetLogViewer(NetLogViewer&& _Other) noexcept = delete;
	NetLogViewer& operator=(const NetLogViewer& _Other) = delete;
	NetLogViewer& operator=(const NetLogViewer&& _Other) noexcept = delete;

	inline GameEngineNet* GetNetPtr()
	{
		return NetPtr;
	}

	inline bool IsHost() const
	{
		return IsHostValue;
	}
	

protected:
	void Start() override;
	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;

private:
	static NetLogViewer* ViewerInst;

	NetLogFSM Fsm;
	GameEngineNet* NetPtr = nullptr;
	GameEngineNetServer ServerNet;
	GameEngineNetClient ClientNet;
	bool IsHostValue = false;
};

