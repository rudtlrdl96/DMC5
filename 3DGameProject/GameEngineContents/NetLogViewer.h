#pragma once
#include <GameEngineCore/GameEngineGUI.h>
//#include <GameEngineBase/GameEngineNetServer.h>
//#include <GameEngineBase/GameEngineNetClient.h>



class NetLogViewer : public GameEngineGUIWindow
{
public:
	NetLogViewer();
	~NetLogViewer();

	NetLogViewer(const NetLogViewer& _Other) = delete;
	NetLogViewer(NetLogViewer&& _Other) noexcept = delete;
	NetLogViewer& operator=(const NetLogViewer& _Other) = delete;
	NetLogViewer& operator=(const NetLogViewer&& _Other) noexcept = delete;

protected:
	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;

private:


};

