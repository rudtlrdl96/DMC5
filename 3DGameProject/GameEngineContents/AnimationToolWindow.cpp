#include "PrecompileHeader.h"
#include "AnimationToolWindow.h"
#include <fstream>
#include <GameEnginePlatform/GameEngineWindow.h>
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
    // FBX ����, �Ǵ� Animation ������ �ҷ��� �̺�Ʈ ������ �����մϴ�

    if (ImGui::Button("Open FBX"))
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
                MsgAssert("FBX������ Ȯ���� ����. ��ҹ��ڸ� Ȯ�����ּ���.");
            }
            AnimationName += ".animation";
            //GameEngineFBXMesh::Load(Path.GetFullPath());
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
            FBXFilePath.SetPath(GameEngineString::UniCodeToAnsi(OFN.lpstrFile));
        }
    }
}

void AnimationToolWindow::AnimationTimeLine()
{
    // �ִϸ��̼��� �������� Ȯ���ϱ� ���� Drag�� Button�� �����մϴ�
    
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
    // �ϼ��� �ִϸ��̼� �̺�Ʈ�� �����մϴ�
    if (AnimationName != "" && ImGui::Button("Save Animation"))
    {
        std::ofstream ofs;
        ofs.open(AnimationName);
        ofs.close();
    }
}

