#include "PrecompileHeader.h"
#include "NetLogViewer.h"

NetLogViewer* NetLogViewer::ViewerInst = nullptr;

NetLogViewer::NetLogViewer()
{
	ViewerInst = this;
}

NetLogViewer::~NetLogViewer()
{
	NetPtr = nullptr;
}

void NetLogViewer::Start()
{
	Fsm.Init();
}

void NetLogViewer::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	Fsm.Update(_DeltaTime);
}
