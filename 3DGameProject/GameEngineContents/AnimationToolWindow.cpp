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
    FrameEvent();
    FileSave();
}

void AnimationToolWindow::FileLoad(std::shared_ptr<GameEngineLevel> _Level)
{
    // FBX ����, �Ǵ� Animation ������ �ҷ��� �̺�Ʈ ������ �����մϴ�

    if (ImGui::Button("Open Mesh FBX"))
    {
        OPENFILENAME OFN;
        TCHAR filePathName[100] = L"";
        TCHAR lpstrFile[100] = L"";
        static TCHAR filter[] = L".fbx �޽� ����\0*.fbx";

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
                MsgAssert("FBX������ Ȯ���� ����. ��ҹ��ڸ� Ȯ�����ּ���.");
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
        static TCHAR filter[] = L".fbx �ִϸ��̼� ����\0*.fbx";

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
                MsgAssert("FBX������ Ȯ���� ����. ��ҹ��ڸ� Ȯ�����ּ���.");
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
    // ���� ����
    if (ImGui::Button("Open Animation"))
    {
        OPENFILENAME OFN;
        TCHAR filePathName[100] = L"";
        TCHAR lpstrFile[100] = L"";
        static TCHAR filter[] = L".animation �ִϸ��̼� ����\0*.Animation";

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
    // �ִϸ��̼��� �������� Ȯ���ϱ� ���� Drag�� Button�� �����մϴ�
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

void AnimationToolWindow::FrameEvent()
{
 
    if (ImGui::Button("Add Object Event"))
    {
        AnimEvent.Events.push_back(new ObjectUpdateEvent(CurrentFrame));
    }
    if (ImGui::Button("Add CallBack Event"))
    {
        AnimEvent.Events.push_back(new CallBackEvent(CurrentFrame));
    }

    CurFrameEvents.clear();
    for (int i = 0; i < AnimEvent.Events.size(); i++)
    {
        EventData* CurData = AnimEvent.Events[i];
        if (CurData->Frame == CurrentFrame)
        {
            CurFrameEvents.push_back(CurData);
        }
    }

    std::list<EventData*>::iterator Iter = CurFrameEvents.begin();
    std::list<EventData*>::iterator End = CurFrameEvents.end();
    for (; Iter != End; Iter++)
    {
        EventData* CurData = (*Iter);
        std::string EventName;

        switch (CurData->Type)
        {
        case EventType::None:
            EventName = "None";
            break;
        case EventType::ObjectUpdate:
            EventName = "ObjectUpdate";
            break;
        case EventType::CallBackVoid:
            EventName = "CallBackVoid";
            break;
        case EventType::CallBackInt:
            EventName = "CallBackInt";
            break;
        case EventType::CallBackFloat:
            EventName = "CallBackFloat";
            break;
        default:
            break;
        }

        if (ImGui::TreeNode(EventName.c_str()))
        {
            if (ImGui::Button("Delete Event"))
            {
                std::find(AnimEvent.Events.begin(), AnimEvent.Events.end(), CurData);
                
            }
            ImGui::Text("Event");

            ImGui::TreePop();
        }
    }

    if (ImGui::TreeNode("Basic"))
    {
        if (ImGui::Button("Delete Event"))
        {

        }
        ImGui::Text("Event");

        ImGui::TreePop();
    }
}

void AnimationToolWindow::FileSave()
{
    // �ϼ��� �ִϸ��̼� �̺�Ʈ�� �����մϴ�
    if (AnimationName != "" && ImGui::Button("Save Animation"))
    {
        std::ofstream ofs;
        ofs.open(AnimationName);
        ofs.close();
    }
}

void AnimationToolWindow::MeshCreate(std::shared_ptr<GameEngineLevel> _Level)
{
    // FBX �޽� �ε�� �������� �����Ѵ�
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
    // FBX �ִ� �ε�� �ִϸ� �����Ѵ�
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

