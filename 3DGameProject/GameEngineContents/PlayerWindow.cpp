#include "PrecompileHeader.h"
#include <commdlg.h>
#include <fstream>
#include "PlayerWindow.h"
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineFBXMesh.h>
PlayerWindow::PlayerWindow() 
{
}

PlayerWindow::~PlayerWindow() 
{
}

void PlayerWindow::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	ImGui::Text(GameEngineString::AnsiToUTF8(Text).c_str());

    static std::string AnimationFileName = "";

    // 파일 열기
    if (ImGui::Button("Open FBX"))
    {
        OPENFILENAME OFN;
        TCHAR filePathName[100] = L"";
        TCHAR lpstrFile[100] = L"";
        static TCHAR filter[] = L".fbx 애니메이션 파일\0*.fbx";

        memset(&OFN, 0, sizeof(OPENFILENAME));
        OFN.lStructSize = sizeof(OPENFILENAME);
        OFN.hwndOwner = GameEngineWindow::GetHWnd();
        OFN.lpstrFilter = filter;
        OFN.lpstrFile = lpstrFile;
        OFN.nMaxFile = 100;
        OFN.lpstrInitialDir = L".";

        if (GetOpenFileName(&OFN) != 0) {
            std::string str = GameEngineString::UniCodeToAnsi(OFN.lpstrFile);
            GameEnginePath Path = GameEngineString::UniCodeToAnsi(OFN.lpstrFile);
            std::string FileName = Path.GetFileName();
            size_t Upper = FileName.find(".FBX");
            size_t Lower = FileName.find(".fbx");
            std::string AnimationName = "";
            if (Upper != -1)
            {
                AnimationName = FileName.substr(0, Upper);
            }
            else if (Lower != -1)
            {
                AnimationName = FileName.substr(0, Lower);
            }
            else
            {
                MsgAssert("FBX파일의 확장자 오류. 대소문자를 확인해주세요.");
            }
            AnimationName += ".animation";
            AnimationFileName = GameEnginePath::GetFolderPath(str) + AnimationName;
            //GameEngineFBXMesh::Load(Path.GetFullPath());
            int a = 0;
        }
    }
    ImGui::SameLine();
    // 파일 열기
    if (ImGui::Button("Open Animation"))
    {
        OPENFILENAME OFN;
        TCHAR filePathName[100] = L"";
        TCHAR lpstrFile[100] = L"";
        static TCHAR filter[] = L".animation 애니메이션 파일\0*.Animation";

        memset(&OFN, 0, sizeof(OPENFILENAME));
        OFN.lStructSize = sizeof(OPENFILENAME);
        OFN.hwndOwner = GameEngineWindow::GetHWnd();
        OFN.lpstrFilter = filter;
        OFN.lpstrFile = lpstrFile;
        OFN.nMaxFile = 100;
        OFN.lpstrInitialDir = L".";

        if (GetOpenFileName(&OFN) != 0) {
            std::string str = GameEngineString::UniCodeToAnsi(OFN.lpstrFile);
            GameEnginePath Path = GameEngineString::UniCodeToAnsi(OFN.lpstrFile);
        }
    }

	static int FrameValue;
	static const int StartFrame = 0;
	static const int EndFrame = 100;
	ImGui::DragInt("Frame", &FrameValue, 1, StartFrame, EndFrame);
	if (ImGui::Button("<") && StartFrame < FrameValue)
	{
		FrameValue--;
	}
	ImGui::SameLine();
	if (ImGui::Button(">") && FrameValue < EndFrame)
	{
		FrameValue++;
	}

    if (ImGui::Button("Save Animation"))
    {
        std::ofstream ofs;
        ofs.open(AnimationFileName);
        GameEngineSerializer Ser;
        ofs << "An";
        ofs.close();
    }
}

