#include "PrecompileHeader.h"
#include "AnimationToolWindow.h"
#include <fstream>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
AnimationToolWindow::AnimationToolWindow() 
{
}

AnimationToolWindow::~AnimationToolWindow() 
{
}

void AnimationToolWindow::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
    FileLoad();
    AnimationTimeLine();
    FileSave();
}

void AnimationToolWindow::FileLoad()
{
    // FBX 파일, 또는 Animation 파일을 불러와 이벤트 편집을 시작합니다

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
            FBXFilePath.SetPath(GameEngineString::UniCodeToAnsi(OFN.lpstrFile));
            std::string FileName = FBXFilePath.GetFileName();
            size_t Upper = FileName.find(".FBX");
            size_t Lower = FileName.find(".fbx");
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
            GameEngineFBXAnimation::Load(FBXFilePath.GetFullPath());
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
            FBXFilePath.SetPath(GameEngineString::UniCodeToAnsi(OFN.lpstrFile));
        }
    }
}

void AnimationToolWindow::AnimationTimeLine()
{
    // 애니메이션의 프레임을 확인하기 위한 Drag와 Button을 지원합니다
    
    ImGui::DragInt("Frame", &CurrentFrame, 1, 0, FrameSize);
    if (ImGui::Button("<") && 0 < CurrentFrame)
    {
        CurrentFrame--;
    }
    ImGui::SameLine();
    if (ImGui::Button(">") && CurrentFrame < FrameSize)
    {
        CurrentFrame++;
    }
    ImGui::SameLine();
    ImGui::Text((" (" + std::to_string(CurrentFrame) + " / " + std::to_string(FrameSize) + ")").c_str());
}

void AnimationToolWindow::FileSave()
{
    // 완성된 애니메이션 이벤트를 저장합니다
    if (AnimationName != "" && ImGui::Button("Save Animation"))
    {
        std::ofstream ofs;
        ofs.open(AnimationName);
        ofs.close();
    }
}

