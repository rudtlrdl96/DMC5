#include "PrecompileHeader.h"
#include "AnimationToolWindow.h"
#include <fstream>
#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "AnimationEvent.h"
AnimationToolWindow::AnimationToolWindow() 
{
}

AnimationToolWindow::~AnimationToolWindow() 
{
}

void AnimationToolWindow::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
    FileLoad(Level);
    //RendererCreate(Level);
    AnimationTimeLine();
    FileSave();
}

void AnimationToolWindow::FileLoad(std::shared_ptr<GameEngineLevel> _Level)
{
    // FBX 파일, 또는 Animation 파일을 불러와 이벤트 편집을 시작합니다

    if (ImGui::Button("Open Mesh FBX"))
    {
        OPENFILENAME OFN;
        TCHAR filePathName[100] = L"";
        TCHAR lpstrFile[100] = L"";
        static TCHAR filter[] = L".fbx 메쉬 파일\0*.fbx";

        memset(&OFN, 0, sizeof(OPENFILENAME));
        OFN.lStructSize = sizeof(OPENFILENAME);
        OFN.hwndOwner = GameEngineWindow::GetHWnd();
        OFN.lpstrFilter = filter;
        OFN.lpstrFile = lpstrFile;
        OFN.nMaxFile = 100;
        OFN.lpstrInitialDir = L".";

        if (GetOpenFileName(&OFN) != 0) {
            MeshFilePath.SetPath(GameEngineString::UniCodeToAnsi(OFN.lpstrFile));
            std::string FileName = MeshFilePath.GetFileName();
            size_t Upper = FileName.find(".FBX");
            size_t Lower = FileName.find(".fbx");
            if (Upper != -1)
            {
                MeshName = FileName.substr(0, Upper);
            }
            else if (Lower != -1)
            {
                MeshName = FileName.substr(0, Lower);
            }
            else
            {
                MsgAssert("FBX파일의 확장자 오류. 대소문자를 확인해주세요.");
            }
            MeshName += ".FBX";
            if (nullptr == GameEngineFBXMesh::Find(MeshName))
            {
                GameEngineFBXMesh::Load(MeshFilePath.GetFullPath());
            }
            MeshCreate(_Level);
            int a = 0;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Open Animation FBX"))
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
            AnimFBXFilePath.SetPath(GameEngineString::UniCodeToAnsi(OFN.lpstrFile));
            AnimationFBXName = AnimFBXFilePath.GetFileName();
            size_t Upper = AnimationFBXName.find(".FBX");
            size_t Lower = AnimationFBXName.find(".fbx");
            if (Upper != -1)
            {
                AnimationName = AnimationFBXName.substr(0, Upper);
            }
            else if (Lower != -1)
            {
                AnimationName = AnimationFBXName.substr(0, Lower);
            }
            else
            {
                MsgAssert("FBX파일의 확장자 오류. 대소문자를 확인해주세요.");
            }
            AnimationName += ".animation";
            if (nullptr == GameEngineFBXAnimation::Find(AnimationFBXName))
            {
                GameEngineFBXAnimation::Load(AnimFBXFilePath.GetFullPath());
            }
            AnimationCreate(_Level);
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
            AnimFilePath.SetPath(GameEngineString::UniCodeToAnsi(OFN.lpstrFile));
        }
    }
}

void AnimationToolWindow::AnimationTimeLine()
{
    // 애니메이션의 프레임을 확인하기 위한 Drag와 Button을 지원합니다
    if (Renderer == nullptr) { return; }
    if (Renderer->CurAnimation == nullptr) { return; }
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


    ImGui::DragFloat("Speed", &AnimSpeed, 0.1f, 0.0f, 20.0f);
    if (IsStop)
    {
        Renderer->CurAnimation->CurFrame = CurrentFrame;
    }
    else
    {
        CurrentFrame = Renderer->CurAnimation->CurFrame;
        Renderer->CurAnimation->TimeScale = AnimSpeed;
    }

    if (ImGui::Button("Play"))
    {
        IsStop = false;
        Renderer->CurAnimation->TimeScale = AnimSpeed;
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop"))
    {
        IsStop = true;
        Renderer->CurAnimation->TimeScale = 0;
    }
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

void AnimationToolWindow::MeshCreate(std::shared_ptr<GameEngineLevel> _Level)
{
    // FBX 메쉬 로드시 렌더러를 생성한다
    if (nullptr != Renderer)
    {
        Renderer->Death();
        Renderer = nullptr;
    }
    if (Actor != nullptr)
    {
        if (Actor->GetLevel() != _Level.get())
        {
            Actor->Death();
            Actor = nullptr;
        }
    }
    if (Actor == nullptr)
    {
        Actor = _Level->CreateActor<GameEngineActor>();
    }

    Renderer = Actor->CreateComponent<GameEngineFBXRenderer>();

    if (MeshName != "")
    {
        Renderer->SetFBXMesh(MeshName, "MeshAniTexture");
    }
}

void AnimationToolWindow::AnimationCreate(std::shared_ptr<GameEngineLevel> Level)
{
    // FBX 애니 로드시 애니를 생성한다
    if (nullptr == Renderer)
    {
        return;
    }

    std::string UpperName = GameEngineString::ToUpper(AnimationFBXName);
    if (Renderer->Animations.end() == Renderer->Animations.find(UpperName))
    {
        Renderer->CreateFBXAnimation(UpperName, UpperName);
    }
   
    Renderer->ChangeAnimation(AnimationFBXName);
    FrameSize = Renderer->CurAnimation->End - 1;
    Renderer->CurAnimation->TimeScale = 0;
}

