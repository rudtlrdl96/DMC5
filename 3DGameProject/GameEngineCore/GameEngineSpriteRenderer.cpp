#include "PrecompileHeader.h"
#include "GameEngineSpriteRenderer.h"
#include "GameEngineSprite.h"

const SpriteInfo& AnimationInfo::CurSpriteInfo()
{
	const SpriteInfo& Info = Sprite->GetSpriteInfo(FrameIndex[CurFrame]);
	return Info;
}

bool AnimationInfo::IsEnd()
{
	return IsEndValue;
}

void AnimationInfo::Reset()
{
	CurFrame = 0;
	CurTime = FrameTime[0];
	IsEndValue = false;
	IsPauseValue = false;
	for (std::pair<const size_t, AnimationStartEvent>& Pair : StartEventFunction)
	{
		Pair.second.IsEvent = false;
	}
}

void AnimationInfo::Update(float _DeltaTime)
{
	if (true == Loop)
	{
		IsEndValue = false;
	}

	// 1;
	// 

	if (true == IsPauseValue)
	{
		return;
	}

	size_t CurFrameIndex = FrameIndex[CurFrame];


	if (StartEventFunction.end() != StartEventFunction.find(CurFrameIndex))
	{
		if (false == StartEventFunction[CurFrameIndex].IsEvent
			&& nullptr != StartEventFunction[CurFrameIndex].Function)
		{
			StartEventFunction[CurFrameIndex].Function();
			StartEventFunction[CurFrameIndex].IsEvent = true;
		}
		
	}


	if (UpdateEventFunction.end() != UpdateEventFunction.find(CurFrameIndex))
	{
		UpdateEventFunction[CurFrameIndex]();
	}

	CurTime -= _DeltaTime;

	if (0.0f >= CurTime)
	{
		++CurFrame;

		if (FrameIndex.size() <= CurFrame)
		{
			IsEndValue = true;

			if (true == Loop)
			{
				CurFrame = 0;

				for (std::pair<const size_t, AnimationStartEvent>& Pair : StartEventFunction)
				{
					Pair.second.IsEvent = false;
				}
			}
			else
			{
				IsEndValue = true;
				--CurFrame;
			}
		}

		CurTime += FrameTime[CurFrame];
	}
}

// SpriteRenderer

GameEngineSpriteRenderer::GameEngineSpriteRenderer()
{
}

GameEngineSpriteRenderer::~GameEngineSpriteRenderer()
{
}


void GameEngineSpriteRenderer::Start()
{
	GameEngineRenderer::Start();

	SpriteRenderInit();
	// AtlasData
}

void GameEngineSpriteRenderer::SetTexture(const std::string_view& _Name)
{
	GetShaderResHelper().SetTexture("DiffuseTex", _Name);

	//Animation�� �����ϴ� SpriteRenderer�� �ٽ� �ؽ�ó�� ������ �� ���˴ϴ�.
	CurAnimation = nullptr;
	AtlasData = float4{ 0.0f, 0.0f, 1.0f, 1.0f };

	std::shared_ptr<GameEngineTexture> FindTex = GameEngineTexture::Find(_Name);
	if (nullptr == FindTex)
	{
		MsgAssert("�������� �ʴ� �̹��� �Դϴ�.");
		return;
	}
	CurTexture = FindTex;

}

void GameEngineSpriteRenderer::SetFlipX()
{
	Flip.x = Flip.x != 0.0f ? 0.0f : 1.0f;
	//float4 LocalScale = GetTransform()->GetLocalScale();
	//LocalScale.x = -LocalScale.x;
	//GetTransform()->SetLocalScale(LocalScale);
}

void GameEngineSpriteRenderer::SetFlipY()
{
	Flip.y = Flip.y != 0.0f ? 0.0f : 1.0f;

	//float4 LocalScale = GetTransform()->GetLocalScale();
	//LocalScale.y = -LocalScale.y;
	//GetTransform()->SetLocalScale(LocalScale);
}

void GameEngineSpriteRenderer::SetScaleToTexture(const std::string_view& _Name)
{
	GetShaderResHelper().SetTexture("DiffuseTex", _Name);
	std::shared_ptr<GameEngineTexture> FindTex = GameEngineTexture::Find(_Name);

	if (nullptr == FindTex)
	{
		MsgAssert("�������� �ʴ� �̹��� �Դϴ�.");
		return;
	}

	float4 Scale = float4(static_cast<float>(FindTex->GetWidth()), static_cast<float>(FindTex->GetHeight()), 1);
	GetTransform()->SetLocalScale(Scale);
	CurTexture = FindTex;


}

void GameEngineSpriteRenderer::SetSprite(const std::string_view& _SpriteName, size_t _Frame/* = 0*/)
{
	Sprite = GameEngineSprite::Find(_SpriteName);
	Frame = _Frame;

	const SpriteInfo& Info = Sprite->GetSpriteInfo(Frame);
	GetShaderResHelper().SetTexture("DiffuseTex", Info.Texture);
	AtlasData = Info.CutData;
	CurTexture = Info.Texture;
}

void GameEngineSpriteRenderer::SetFrame(size_t _Frame)
{
	Frame = _Frame;

	const SpriteInfo& Info = Sprite->GetSpriteInfo(Frame);
	GetShaderResHelper().SetTexture("DiffuseTex", Info.Texture);
	AtlasData = Info.CutData;
	CurTexture = Info.Texture;
}

std::shared_ptr<AnimationInfo> GameEngineSpriteRenderer::FindAnimation(const std::string_view& _Name)
{
	std::map<std::string, std::shared_ptr<AnimationInfo>>::iterator FindIter = Animations.find(_Name.data());

	if (FindIter == Animations.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

std::shared_ptr<AnimationInfo> GameEngineSpriteRenderer::CreateAnimation(const AnimationParameter& _Paramter)
{
	if (nullptr != FindAnimation(_Paramter.AnimationName))
	{
		MsgAssert("�̹� �����ϴ� �̸��� �ִϸ��̼��� �� ������� �߽��ϴ�." + std::string(_Paramter.AnimationName));
		return nullptr;
	}

	std::shared_ptr<GameEngineSprite> Sprite = GameEngineSprite::Find(_Paramter.SpriteName);

	if (nullptr == Sprite)
	{
		MsgAssert("�������� �ʴ� ��������Ʈ�� �ִϸ��̼��� ������� �߽��ϴ�." + std::string(_Paramter.AnimationName));
		return nullptr;
	}

	std::shared_ptr<AnimationInfo> NewAnimation = std::make_shared<AnimationInfo>();
	Animations[_Paramter.AnimationName.data()] = NewAnimation;

	if (0 != _Paramter.FrameIndex.size())
	{
		// ������ �ε��� �Է½�
		NewAnimation->FrameIndex = _Paramter.FrameIndex;

	}
	else
	{
		// ������ �ε��� �� �Է½�

		// ���� ������ ����
		if (-1 != _Paramter.Start)
		{
			if (_Paramter.Start < 0)
			{
				MsgAssert("��������Ʈ ������ �ʰ��ϴ� �ε����� �ִϸ��̼��� ������� �߽��ϴ�." + std::string(_Paramter.AnimationName));
				return nullptr;
			}

			NewAnimation->StartFrame = _Paramter.Start;
		}
		else
		{
			// -1 �Է½� ���������� 0
			NewAnimation->StartFrame = 0;
		}
		// �� ������ ����
		if (-1 != _Paramter.End)
		{
			if (_Paramter.End >= Sprite->GetSpriteCount())
			{
				MsgAssert("��������Ʈ ������ �ʰ��ϴ� �ε����� �ִϸ��̼��� ������� �߽��ϴ�." + std::string(_Paramter.AnimationName));
				return nullptr;
			}

			NewAnimation->EndFrame = _Paramter.End;
		}
		else
		{
			// -1 �Է½� ���������� ������
			NewAnimation->EndFrame = Sprite->GetSpriteCount() - 1;
		}

		if (NewAnimation->EndFrame < NewAnimation->StartFrame)
		{
			MsgAssert("�ִϸ��̼��� �����Ҷ� End�� Start���� Ŭ �� �����ϴ�");
			return nullptr;
		}
		NewAnimation->FrameIndex.reserve(NewAnimation->EndFrame - NewAnimation->StartFrame + 1);

		// StartFrame ���� EndFrame���� ������� FrameIndex�� Ǫ��
		for (size_t i = NewAnimation->StartFrame; i <= NewAnimation->EndFrame; ++i)
		{
			NewAnimation->FrameIndex.push_back(i);
		}
	}

	// Ÿ�� �����Ͱ� �ִٸ�
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


void GameEngineSpriteRenderer::ChangeAnimation(const std::string_view& _Name, size_t _Frame, bool _Force)
{
	std::shared_ptr<AnimationInfo> Find = FindAnimation(_Name);

	if (nullptr == Find)
	{
		MsgAssert("�̷��� �̸��� �ִϸ��̼��� �������� �ʽ��ϴ�" + std::string(_Name));
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

void GameEngineSpriteRenderer::Update(float _Delta)
{
	if (nullptr != CurAnimation)
	{
		CurAnimation->Update(_Delta);

		const SpriteInfo& Info = CurAnimation->CurSpriteInfo();

		GetShaderResHelper().SetTexture("DiffuseTex", Info.Texture);

		AtlasData = Info.CutData;

		if (true == CurAnimation->ScaleToTexture)
		{
			std::shared_ptr<GameEngineTexture> Texture = Info.Texture;
			CurTexture = Texture;
			float4 Scale = Texture->GetScale();

			Scale.x *= AtlasData.SizeX;
			Scale.y *= AtlasData.SizeY;
			Scale.z = 1.0f;

			Scale *= ScaleRatio;

			GetTransform()->SetLocalScale(Scale);
		}
	}
}

void GameEngineSpriteRenderer::Render(float _Delta)
{

	GameEngineRenderer::Render(_Delta);
	//AtlasData = float4(0, 0, 1, 1);

	if (nullptr != RenderEndCallBack)
	{
		RenderEndCallBack(this);
	}

}

void GameEngineSpriteRenderer::SetAnimationUpdateEvent(const std::string_view& _AnimationName, size_t _Frame, std::function<void()> _Event)
{
	std::shared_ptr<AnimationInfo>  Info = FindAnimation(_AnimationName);

	if (nullptr == Info)
	{
		MsgAssert("�������� �ʴ� �ִϸ��̼ǿ� �̺�Ʈ ������ �Ϸ��� �߽��ϴ�.");
	}

	Info->UpdateEventFunction[_Frame] = _Event;
}


void GameEngineSpriteRenderer::SetAnimationStartEvent(const std::string_view& _AnimationName, size_t _Frame, std::function<void()> _Event)
{
	std::shared_ptr<AnimationInfo>  Info = FindAnimation(_AnimationName);

	if (nullptr == Info)
	{
		MsgAssert("�������� �ʴ� �ִϸ��̼ǿ� �̺�Ʈ ������ �Ϸ��� �߽��ϴ�.");
	}

	Info->StartEventFunction[_Frame].IsEvent = false;
	Info->StartEventFunction[_Frame].Function = _Event;
}

std::string GameEngineSpriteRenderer::GetTexName()
{
	GameEngineTextureSetter* Tex = GetShaderResHelper().GetTextureSetter("DiffuseTex");
	std::string Name = Tex->Res->GetNameToString();
	return Name;
}

void GameEngineSpriteRenderer::SpriteRenderInit()
{

	SetMesh("Rect");
	SetPipeLine("2DTexture");

	AtlasData.x = 0.0f;
	AtlasData.y = 0.0f;
	AtlasData.z = 1.0f;
	AtlasData.w = 1.0f;

	ColorOptionValue.MulColor = float4::One;
	ColorOptionValue.PlusColor = float4::Null;

	GetShaderResHelper().SetConstantBufferLink("AtlasData", AtlasData);
	GetShaderResHelper().SetConstantBufferLink("ColorOption", ColorOptionValue);
	GetShaderResHelper().SetConstantBufferLink("ClipData", Clip);
	GetShaderResHelper().SetConstantBufferLink("FlipData", Flip);
}


// �� ���� ���̴� �̹������� 0.1;
void GameEngineSpriteRenderer::ImageClippingX(float _Ratio, ClipXDir _Dir)
{
	Clip.x = _Ratio;

	if (_Dir == ClipXDir::Left)
	{
		Clip.z = 0;
	}
	else {
		Clip.z = 1;
	}

	if (0.0f >= Clip.x)
	{
		Clip.x = 0.0f;
	}
}


void GameEngineSpriteRenderer::ImageClippingY(float _Ratio, ClipYDir _Dir)
{
	Clip.y = _Ratio;

	if (_Dir == ClipYDir::Top)
	{
		Clip.w = 0;
	}
	else {
		Clip.w = 1;
	}


	if (0.0f >= Clip.y)
	{
		Clip.y = 0.0f;
	}
}
