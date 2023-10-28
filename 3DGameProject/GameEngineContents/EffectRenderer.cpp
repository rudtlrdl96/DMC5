#include "PrecompileHeader.h"
#include "EffectRenderer.h"

EffectRenderer::EffectRenderer() 
{
}

EffectRenderer::~EffectRenderer() 
{
}

void EffectRenderer::SetDistortionTexture(const std::string_view& _TextureName, const float4& _DistortionPower/* = { 1, 1, 1, 1 }*/)
{
	for (size_t i = 0; i < Unit.size(); i++)
	{
		for (size_t j = 0; j < Unit[i].size(); j++)
		{
			if ("" == _TextureName)
			{
				Unit[i][j]->ShaderResHelper.SetTexture("MaskTexture", Unit[i][j]->ShaderResHelper.GetTextureSetter("DiffuseTexture")->Res->GetName());
			}
			else
			{
				Unit[i][j]->ShaderResHelper.SetTexture("MaskTexture", _TextureName);
			}
		}
	}

	DistortionOption.IsDistortion = _DistortionPower;
}

void EffectRenderer::UnSetDistortionTexture()
{
	DistortionOption.IsDistortion = float4(0, 0, 0, 0);
}

void EffectRenderer::SetSprite(const std::string_view& _SpriteName, size_t _Frame)
{
	Sprite = GameEngineSprite::Find(_SpriteName);
	Frame = _Frame;

	const SpriteInfo& Info = Sprite->GetSpriteInfo(Frame);
	GetShaderResHelper().SetTexture("DiffuseTexture", Info.Texture);

	VertexOption.FramePos.x = Info.CutData.PosX;
	VertexOption.FramePos.y = Info.CutData.PosY;
	VertexOption.FrameScale.x = Info.CutData.SizeX;
	VertexOption.FrameScale.y = Info.CutData.SizeY;
		
	CurTexture = Info.Texture;
}

void EffectRenderer::SetFrame(size_t _Frame)
{
	Frame = _Frame;

	const SpriteInfo& Info = Sprite->GetSpriteInfo(Frame);
	GetShaderResHelper().SetTexture("DiffuseTexture", Info.Texture);

	VertexOption.FramePos.x = Info.CutData.PosX;
	VertexOption.FramePos.y = Info.CutData.PosY;
	VertexOption.FrameScale.x = Info.CutData.SizeX;
	VertexOption.FrameScale.y = Info.CutData.SizeY;

	CurTexture = Info.Texture;
}

void EffectRenderer::SetAnimationUpdateEvent(const std::string_view& _AnimationName, size_t _Frame, std::function<void()> _Event)
{
	std::shared_ptr<AnimationInfo>  Info = FindAnimation(_AnimationName);

	if (nullptr == Info)
	{
		MsgAssert("존재하지 않는 애니메이션에 이벤트 세팅을 하려고 했습니다.");
	}

	Info->UpdateEventFunction[_Frame] = _Event;
}

void EffectRenderer::SetAnimationStartEvent(const std::string_view& _AnimationName, size_t _Frame, std::function<void()> _Event)
{
	std::shared_ptr<AnimationInfo>  Info = FindAnimation(_AnimationName);

	if (nullptr == Info)
	{
		MsgAssert("존재하지 않는 애니메이션에 이벤트 세팅을 하려고 했습니다.");
	}

	Info->StartEventFunction[_Frame].IsEvent = false;
	Info->StartEventFunction[_Frame].Function = _Event;
}

void EffectRenderer::Reset()
{
	if (nullptr != CurAnimation)
	{
		CurAnimation->CurFrame = 0;
	}
	Off();
}

void EffectRenderer::Start()
{
	GameEngineFBXRenderer::Start();
}

void EffectRenderer::Update(float _DeltaTime)
{
	if (nullptr != CurAnimation)
	{
		CurAnimation->Update(_DeltaTime);

		const SpriteInfo& Info = CurAnimation->CurSpriteInfo();
		
		GetShaderResHelper().SetTexture("DiffuseTexture", Info.Texture);

		VertexOption.FramePos.x = Info.CutData.PosX;
		VertexOption.FramePos.y = Info.CutData.PosY;
		VertexOption.FrameScale.x = Info.CutData.SizeX;
		VertexOption.FrameScale.y = Info.CutData.SizeY;

		if (true == CurAnimation->ScaleToTexture)
		{
			std::shared_ptr<GameEngineTexture> Texture = Info.Texture;
			CurTexture = Texture;
			float4 Scale = Texture->GetScale();

			Scale.x *= VertexOption.FrameScale.SizeX;
			Scale.y *= VertexOption.FrameScale.SizeY;
			Scale.z = 1.0f;

			Scale *= ScaleRatio;

			GetTransform()->SetLocalScale(Scale);
		}
	}
}


void EffectRenderer::RectInit(const std::string_view& _MaterialName)
{
	GetTransform()->SetLocalScale({ 100, 100, 100 });
	Unit.resize(1);
	Unit[0].resize(1);

	Unit[0][0] = CreateRenderUnit();
	Unit[0][0]->SetMesh("Rect");
	Unit[0][0]->SetMaterial(_MaterialName);
	CustomOptionSetting();
}

void EffectRenderer::SphereInit(const std::string_view& _MaterialName)
{
	Unit.resize(1);
	Unit[0].resize(1);

	Unit[0][0] = CreateRenderUnit();
	Unit[0][0]->SetMesh("DebugSphere");
	Unit[0][0]->SetMaterial(_MaterialName);
	CustomOptionSetting();
}

void EffectRenderer::SetFBXMesh(const std::string_view& _Name, const std::string_view& _Material)
{
	GameEngineFBXRenderer::SetFBXMesh(_Name, _Material);
	CustomOptionSetting();
}

void EffectRenderer::SetFBXMesh(const std::string_view& _Name, const std::vector<std::vector<std::string>>& _Materials)
{
	GameEngineFBXRenderer::SetFBXMesh(_Name, _Materials);
	CustomOptionSetting();
}

void EffectRenderer::SetFBXMesh(const std::string_view& _Name, const std::string_view& _Material, size_t MeshIndex)
{
	GameEngineFBXRenderer::SetFBXMesh(_Name, _Material, MeshIndex);
	CustomOptionSetting();
}

void EffectRenderer::SetFBXMesh(const std::string_view& _Name, std::vector<std::string> _Material, size_t MeshIndex)
{
	GameEngineFBXRenderer::SetFBXMesh(_Name, _Material, MeshIndex);
	CustomOptionSetting();
}

void EffectRenderer::SetFlipX()
{
	VertexOption.Flip.x = (VertexOption.Flip.x != 0.0f ? 0.0f : 1.0f);
}

void EffectRenderer::SetFlipY()
{
	VertexOption.Flip.y = (VertexOption.Flip.y != 0.0f ? 0.0f : 1.0f);
}

std::shared_ptr<AnimationInfo> EffectRenderer::FindAnimation(const std::string_view& _Name)
{
	std::map<std::string, std::shared_ptr<AnimationInfo>>::iterator FindIter = Animations.find(_Name.data());

	if (FindIter == Animations.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

std::shared_ptr<AnimationInfo> EffectRenderer::CreateAnimation(const AnimationParameter& _Paramter)
{	
	
	if (nullptr != FindAnimation(_Paramter.AnimationName))
	{
		MsgAssert("이미 존재하는 이름의 애니메이션을 또 만들려고 했습니다." + std::string(_Paramter.AnimationName));
		return nullptr;
	}

	std::shared_ptr<GameEngineSprite> Sprite = GameEngineSprite::Find(_Paramter.SpriteName);

	if (nullptr == Sprite)
	{
		MsgAssert("존재하지 않는 스프라이트로 애니메이션을 만들려고 했습니다." + std::string(_Paramter.AnimationName));
		return nullptr;
	}

	std::shared_ptr<AnimationInfo> NewAnimation = std::make_shared<AnimationInfo>();
	Animations[_Paramter.AnimationName.data()] = NewAnimation;

	if (0 != _Paramter.FrameIndex.size())
	{
		NewAnimation->FrameIndex = _Paramter.FrameIndex;
	}
	else
	{
		if (-1 != _Paramter.Start)
		{
			if (_Paramter.Start < 0)
			{
				MsgAssert("스프라이트 범위를 초과하는 인덱스로 애니메이션을 마들려고 했습니다." + std::string(_Paramter.AnimationName));
				return nullptr;
			}

			NewAnimation->StartFrame = _Paramter.Start;
		}
		else
		{
			NewAnimation->StartFrame = 0;
		}

		if (-1 != _Paramter.End)
		{
			if (_Paramter.End >= Sprite->GetSpriteCount())
			{
				MsgAssert("스프라이트 범위를 초과하는 인덱스로 애니메이션을 마들려고 했습니다." + std::string(_Paramter.AnimationName));
				return nullptr;
			}

			NewAnimation->EndFrame = _Paramter.End;
		}
		else
		{
			NewAnimation->EndFrame = Sprite->GetSpriteCount() - 1;
		}

		if (NewAnimation->EndFrame < NewAnimation->StartFrame)
		{
			MsgAssert("애니메이션을 생성할때 End가 Start보다 클 수 없습니다");
			return nullptr;
		}

		NewAnimation->FrameIndex.reserve(NewAnimation->EndFrame - NewAnimation->StartFrame + 1);

		for (size_t i = NewAnimation->StartFrame; i <= NewAnimation->EndFrame; ++i)
		{
			NewAnimation->FrameIndex.push_back(i);
		}
	}

	if (0 != _Paramter.FrameTime.size())
	{
		NewAnimation->FrameTime = _Paramter.FrameTime;

	}
	else
	{
		for (size_t i = 0; i < NewAnimation->FrameIndex.size(); ++i)
		{
			NewAnimation->FrameTime.push_back(_Paramter.FrameInter);
		}
	}

	NewAnimation->Sprite = Sprite;
	NewAnimation->Parent = this;
	NewAnimation->Loop = _Paramter.Loop;
	NewAnimation->ScaleToTexture = _Paramter.ScaleToTexture;

	return NewAnimation;
}

void EffectRenderer::ChangeAnimation(const std::string_view& _Name, size_t _Frame, bool _Force)
{
	std::shared_ptr<AnimationInfo> Find = FindAnimation(_Name);

	if (nullptr == Find)
	{
		MsgAssert("이러한 이름의 애니메이션은 존재하지 않습니다" + std::string(_Name));
		return;
	}

	if (CurAnimation.get() == Find.get() && false == _Force)
	{
		return;
	}

	CurAnimation = FindAnimation(_Name);
	CurAnimation->Reset();

	if (_Frame != -1)
	{
		CurAnimation->CurFrame = _Frame;
	}
}

void EffectRenderer::CustomOptionSetting()
{
	for (size_t i = 0; i < Unit.size(); i++)
	{
		for (size_t j = 0; j < Unit[i].size(); j++)
		{
			Unit[i][j]->ShaderResHelper.SetConstantBufferLink("EffectData", EffectOption);
			Unit[i][j]->ShaderResHelper.SetConstantBufferLink("EffectVertextData", VertexOption);
			Unit[i][j]->ShaderResHelper.SetConstantBufferLink("DistortionData", DistortionOption);
			Unit[i][j]->ShaderResHelper.SetConstantBufferLink("HSVColorData", HSVColor);
		}
	}
}

#include <GameEngineCore/imgui.h>

void EffectRenderer::DrawEditor()
{
	ImGui::DragFloat("Clip Start X", &EffectOption.ClipStartX, 0.01f, 0, 1);
	ImGui::DragFloat("Clip End X", &EffectOption.ClipEndX, 0.01f, 0, 1);

	ImGui::DragFloat("Clip Start Y", &EffectOption.ClipStartY, 0.01f, 0, 1);
	ImGui::DragFloat("Clip End Y", &EffectOption.ClipEndY, 0.01f, 0, 1);

	ImGui::DragFloat("UV X", &EffectOption.UVX, 0.01f, -1, 1);
	ImGui::DragFloat("UV Y", &EffectOption.UVY, 0.01f, -1, 1);
	
	ImGui::ColorEdit4("MulColor", EffectOption.MulColor.Arr1D);
	ImGui::ColorEdit4("PlusColor", EffectOption.PlusColor.Arr1D);

	if (ImGui::Button("Set Mesh FBX"))
	{
		OPENFILENAME OFN;
		TCHAR lpstrFile[200] = L"";
		static TCHAR filter[] = L".fbx 메쉬 파일\0*.fbx";

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = GameEngineWindow::GetHWnd();
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 200;
		OFN.lpstrInitialDir = L".";

		if (GetOpenFileName(&OFN) != 0) {
			GameEnginePath Path = GameEngineString::UniCodeToAnsi(OFN.lpstrFile);
			std::string FileName = Path.GetFileName();
			if (nullptr == GameEngineFBXMesh::Find(FileName))
			{
				GameEngineFBXMesh::Load(Path.GetFullPath());
			}
			SetFBXMesh(FileName, "Effect_2D");
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Set Texture"))
	{
		OPENFILENAME OFN;
		TCHAR lpstrFile[200] = L"";
		static TCHAR filter[] = L"텍스쳐 파일\0*.tga;*.png";

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = GameEngineWindow::GetHWnd();
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 200;
		OFN.lpstrInitialDir = L".";

		if (GetOpenFileName(&OFN) != 0) {
			GameEnginePath Path = GameEngineString::UniCodeToAnsi(OFN.lpstrFile);
			std::string FileName = Path.GetFileName();
			if (nullptr == GameEngineTexture::Find(FileName))
			{
				GameEngineTexture::Load(Path.GetFullPath());
			}

			if (0 == Unit.size())
			{
				RectInit("Effect_3D");
				LockRotation();
			}
			SetTexture("DiffuseTexture", FileName);
		}
	}

	static int Cut[] = {0, 0};
	ImGui::InputInt2("Sprite Slice", Cut);
	if (ImGui::Button("Set Sprite"))
	{
		OPENFILENAME OFN;
		TCHAR lpstrFile[200] = L"";
		static TCHAR filter[] = L"텍스쳐 파일\0*.tga;*.png";

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = GameEngineWindow::GetHWnd();
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 200;
		OFN.lpstrInitialDir = L".";

		if (GetOpenFileName(&OFN) != 0) {
			RectInit("Effect_2D");
			LockRotation();

			GameEnginePath Path = GameEngineString::UniCodeToAnsi(OFN.lpstrFile);
			std::string FileName = Path.GetFileName();
			if (nullptr == GameEngineSprite::Find(FileName))
			{
				GameEngineSprite::LoadSheet(Path.GetFullPath(), Cut[0], Cut[1]);
			}
			if (nullptr == FindAnimation(FileName))
			{
				CreateAnimation({ .AnimationName = FileName, .SpriteName = FileName, .FrameInter = 0.0166f, .Loop = true });
			}
			ChangeAnimation(FileName);
		}
	}

	if (ImGui::Button("DistortionOn"))
	{
		SetDistortionTexture();
	}
	ImGui::SameLine();
	if (ImGui::Button("DistortionOff"))
	{
		UnSetDistortionTexture();
	}
	if (ImGui::Button("Death"))
	{
		Death();
	}

	ImGui::Spacing();

}
