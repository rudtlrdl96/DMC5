#include "PrecompileHeader.h"
#include "GameEngineShaderResHelper.h"
#include "GameEngineShader.h"
#include "GameEngineConstantBuffer.h"


void GameEngineShaderResHelper::Copy(const GameEngineShaderResHelper& _ResHelper) 
{
	for (const std::pair<std::string, GameEngineConstantBufferSetter>& Setter : _ResHelper.ConstantBufferSetters)
	{
		ConstantBufferSetters.insert(Setter);
	}

	for (const std::pair<std::string, GameEngineTextureSetter>& Setter : _ResHelper.TextureSetters)
	{
		TextureSetters.insert(Setter);
	}

	for (const std::pair<std::string, GameEngineSamplerSetter>& Setter : _ResHelper.SamplerSetters)
	{
		SamplerSetters.insert(Setter);
	}

}

void GameEngineConstantBufferSetter::Setting() 
{
	Res->ChangeData(CPUData, CPUDataSize);

	ShaderType Type = ParentShader->GetType();

	switch (Type)
	{
	case ShaderType::None:
	{
		MsgAssert("� ���̴��� ���õ��� �˼����� ������� �Դϴ�.");
		break;
	}
	case ShaderType::Vertex:
	{
		Res->VSSetting(BindPoint);
		break;
	}
	case ShaderType::Pixel:
	{
		Res->PSSetting(BindPoint);
		break;
	}
	default:
		break;
	}

}


void GameEngineTextureSetter::Setting()
{

	ShaderType Type = ParentShader->GetType();

	switch (Type)
	{
	case ShaderType::None:
	{
		MsgAssert("� ���̴��� ���õ��� �˼����� ������� �Դϴ�.");
		break;
	}
	case ShaderType::Vertex:
	{
		Res->VSSetting(BindPoint);
		break;
	}
	case ShaderType::Pixel:
	{
		Res->PSSetting(BindPoint);
		break;
	}
	default:
		break;
	}

}

void GameEngineTextureSetter::Reset()
{
	ShaderType Type = ParentShader->GetType();

	switch (Type)
	{
	case ShaderType::None:
	{
		MsgAssert("� ���̴��� ���õ��� �˼����� ������� �Դϴ�.");
		break;
	}
	case ShaderType::Vertex:
	{
		Res->VSReset(BindPoint);
		break;
	}
	case ShaderType::Pixel:
	{
		Res->PSReset(BindPoint);
		break;
	}
	default:
		break;
	}
}

void GameEngineSamplerSetter::Setting()
{
	ShaderType Type = ParentShader->GetType();

	switch (Type)
	{
	case ShaderType::None:
	{
		MsgAssert("� ���̴��� ���õ��� �˼����� ������� �Դϴ�.");
		break;
	}
	case ShaderType::Vertex:
	{
		Res->VSSetting(BindPoint);
		break;
	}
	case ShaderType::Pixel:
	{
		Res->PSSetting(BindPoint);
		break;
	}
	default:
		break;
	}
}


void GameEngineShaderResHelper::Setting() 
{
	{
		std::multimap<std::string, GameEngineConstantBufferSetter>::iterator StartIter = ConstantBufferSetters.begin();
		std::multimap<std::string, GameEngineConstantBufferSetter>::iterator EndIter = ConstantBufferSetters.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			GameEngineConstantBufferSetter& Setter = StartIter->second;
			Setter.Setting();
		}
	}

	{
		std::multimap<std::string, GameEngineTextureSetter>::iterator StartIter = TextureSetters.begin();
		std::multimap<std::string, GameEngineTextureSetter>::iterator EndIter = TextureSetters.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			GameEngineTextureSetter& Setter = StartIter->second;
			Setter.Setting();
		}
	}

	{
		std::multimap<std::string, GameEngineSamplerSetter>::iterator StartIter = SamplerSetters.begin();
		std::multimap<std::string, GameEngineSamplerSetter>::iterator EndIter = SamplerSetters.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			GameEngineSamplerSetter& Setter = StartIter->second;
			Setter.Setting();
		}
	}


}

void GameEngineShaderResHelper::SetConstantBufferLink(const std::string_view& _Name, const void* _Data, UINT _Size)
{
	std::string UpperName = GameEngineString::ToUpper(_Name);

	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator FindIter = ConstantBufferSetters.find(UpperName);
	
	if (ConstantBufferSetters.end() == FindIter)
	{
		MsgAssert("�������� �ʴ� ������۸� �����Ϸ��� �߽��ϴ�." + UpperName);
		return;
	}

	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator NameStartIter = ConstantBufferSetters.lower_bound(UpperName);
	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator NameEndIter = ConstantBufferSetters.upper_bound(UpperName);

	for (; NameStartIter != NameEndIter; ++NameStartIter)
	{
		GameEngineConstantBufferSetter& Setter = NameStartIter->second;

		if (Setter.Res->GetBufferSize() != _Size)
		{
			MsgAssert("������ۿ� �����Ϸ��� �������� ũ�Ⱑ �ٸ��ϴ�. ������� : " + std::to_string(Setter.Res->GetBufferSize()) + "������ ������ ������" + std::to_string(_Size) + UpperName);
			return;
		}
		
		Setter.CPUData = _Data;
		Setter.CPUDataSize = _Size;
	}

}

void GameEngineShaderResHelper::SetTexture(const std::string_view& _SettingName, std::shared_ptr<GameEngineTexture> _Texture)
{
	if (nullptr == _Texture)
	{
		MsgAssert("nullptr�� �ؽ�ó�� �����Ϸ��� �߽��ϴ�." + std::string(_SettingName));
		return;
	}

	std::string UpperName = GameEngineString::ToUpper(_SettingName);

	std::multimap<std::string, GameEngineTextureSetter>::iterator FindIter = TextureSetters.find(UpperName);

	if (TextureSetters.end() == FindIter)
	{
		MsgAssert("���̴����� �̷� �̸��� �ؽ�ó ���ø� ������� �ʾҽ��ϴ�." + UpperName);
		return;
	}

	std::multimap<std::string, GameEngineTextureSetter>::iterator NameStartIter = TextureSetters.lower_bound(UpperName);
	std::multimap<std::string, GameEngineTextureSetter>::iterator NameEndIter = TextureSetters.upper_bound(UpperName);

	for (; NameStartIter != NameEndIter; ++NameStartIter)
	{
		GameEngineTextureSetter& Setter = NameStartIter->second;
		Setter.Res = _Texture;
	}
}

void GameEngineShaderResHelper::SetTexture(const std::string_view& _SettingName, const std::string_view& _ImageName)
{
	std::string UpperName = GameEngineString::ToUpper(_SettingName);

	std::multimap<std::string, GameEngineTextureSetter>::iterator FindIter = TextureSetters.find(UpperName);

	if (TextureSetters.end() == FindIter)
	{
		MsgAssert("���̴����� �̷� �̸��� �ؽ�ó ���ø� ������� �ʾҽ��ϴ�." + UpperName);
		return;
	}

	std::multimap<std::string, GameEngineTextureSetter>::iterator NameStartIter = TextureSetters.lower_bound(UpperName);
	std::multimap<std::string, GameEngineTextureSetter>::iterator NameEndIter = TextureSetters.upper_bound(UpperName);

	std::shared_ptr<GameEngineTexture> FindTex = GameEngineTexture::Find(_ImageName);

	if (nullptr == FindTex)
	{
		MsgAssert("�̷� �̸��� �ؽ�ó�� �ε��� ���� �����ϴ�." + std::string(_ImageName));
		return;
	}

	for (; NameStartIter != NameEndIter; ++NameStartIter)
	{
		GameEngineTextureSetter& Setter = NameStartIter->second;
		Setter.Res = FindTex;
	}

}

GameEngineTextureSetter* GameEngineShaderResHelper::GetTextureSetter(const std::string_view& _View)
{
	std::string UpperName = GameEngineString::ToUpper(_View);

	std::multimap<std::string, GameEngineTextureSetter>::iterator FindIter = TextureSetters.find(UpperName);

	if (FindIter == TextureSetters.end())
	{
		return nullptr;
	}

	return &(FindIter->second);

}

std::vector<GameEngineTextureSetter*> GameEngineShaderResHelper::GetTextureSetters(const std::string_view& _View)
{
	std::string UpperName = GameEngineString::ToUpper(_View);

	std::multimap<std::string, GameEngineTextureSetter>::iterator FindIter = TextureSetters.find(UpperName);

	std::vector<GameEngineTextureSetter*> Result;

	if (FindIter == TextureSetters.end())
	{
		return Result;
	}

	std::multimap<std::string, GameEngineTextureSetter>::iterator NameStartIter = TextureSetters.lower_bound(UpperName);
	std::multimap<std::string, GameEngineTextureSetter>::iterator NameEndIter = TextureSetters.upper_bound(UpperName);

	for (; NameStartIter != NameEndIter; ++NameStartIter)
	{
		Result.push_back(&NameStartIter->second);
	}

	return Result;

}

void GameEngineShaderResHelper::AllResourcesReset()
{
	{
		std::multimap<std::string, GameEngineTextureSetter>::iterator StartIter = TextureSetters.begin();
		std::multimap<std::string, GameEngineTextureSetter>::iterator EndIter = TextureSetters.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			GameEngineTextureSetter& Setter = StartIter->second;
			Setter.Reset();
		}
	}
}