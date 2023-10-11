#include "PrecompileHeader.h"
#include "GameEngineShader.h"
#include "GameEngineConstantBuffer.h"
#include "GameEngineVertexShader.h"
#include "GameEnginePixelShader.h"
#include "GameEngineGeometryShader.h"
#include "GameEngineStructuredBuffer.h"
#include "GameEngineComputeShader.h"

GameEngineShader::GameEngineShader() 
{
}

GameEngineShader::~GameEngineShader() 
{
	if (nullptr != BinaryCode)
	{
		BinaryCode->Release();
		BinaryCode = nullptr;
	}
}

void GameEngineShader::CreateVersion(const std::string_view& _ShaderType, UINT _VersionHigt /*= 5*/, UINT _VersionLow /*= 0*/)
{
	// vs_5_0
	Version += _ShaderType;
	Version += "_";
	Version += std::to_string(_VersionHigt);
	Version += "_";
	Version += std::to_string(_VersionLow);
}


void GameEngineShader::ShaderResCheck()
{
	if (nullptr == BinaryCode)
	{
		MsgAssert("쉐이더가 컴파일 되지 않아서 쉐이더의 리소스를 조사할 수 없습니다.");
		return;
	}

	// Reflection 
	// RTTI의 비슷한 개념으로 
	ID3D11ShaderReflection* CompileInfo = nullptr;

	if (S_OK != D3DReflect(BinaryCode->GetBufferPointer(), BinaryCode->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&CompileInfo)))
	{
		MsgAssert("쉐이더 리플렉션에 실패했습니다.");
		return;
	}

	D3D11_SHADER_DESC Info;

	CompileInfo->GetDesc(&Info);

	D3D11_SHADER_INPUT_BIND_DESC ResDesc;

	// 내가 사용한 상수버퍼 텍스처 샘플러등의 총합입니다.
	for (UINT i = 0; i < Info.BoundResources; i++)
	{
		// 리소스 정보를 얻어오게 되고
		CompileInfo->GetResourceBindingDesc(i, &ResDesc);

		std::string Name = ResDesc.Name;

		D3D_SHADER_INPUT_TYPE Type = ResDesc.Type;

		std::string UpperName = GameEngineString::ToUpper(ResDesc.Name);

		switch (Type)
		{
		case D3D_SIT_CBUFFER:
		{

			ID3D11ShaderReflectionConstantBuffer* CBufferPtr = CompileInfo->GetConstantBufferByName(ResDesc.Name);

			D3D11_SHADER_BUFFER_DESC BufferDesc;
			CBufferPtr->GetDesc(&BufferDesc);
			
			std::shared_ptr<GameEngineConstantBuffer> Res = GameEngineConstantBuffer::CreateAndFind(BufferDesc.Size, UpperName, BufferDesc);

			GameEngineConstantBufferSetter Setter;

			Setter.ParentShader = this;
			Setter.Name = UpperName;
			Setter.BindPoint = ResDesc.BindPoint;
			Setter.Res = Res;

			ResHelper.CreateConstantBufferSetter(Setter);

			int a = 0;

			break;
		}
		case D3D_SIT_TEXTURE:
		{
			if (9 == ResDesc.BindPoint)
			{
				continue;
			}

			if (D3D_SRV_DIMENSION_TEXTURECUBE == ResDesc.Dimension)
			{
				std::shared_ptr<GameEngineTexture> Res = GameEngineTexture::Find("BaseCubemap");

				GameEngineTextureSetter Setter;
				Setter.ParentShader = this;
				Setter.Name = UpperName;
				Setter.BindPoint = ResDesc.BindPoint;
				Setter.Res = Res;

				ResHelper.CreateTextureSetter(Setter);
			}
			else
			{
				std::shared_ptr<GameEngineTexture> Res = GameEngineTexture::Find("EngineBaseTex.png");

				GameEngineTextureSetter Setter;
				Setter.ParentShader = this;
				Setter.Name = UpperName;
				Setter.BindPoint = ResDesc.BindPoint;
				Setter.Res = Res;

				ResHelper.CreateTextureSetter(Setter);
			}

			break;
		}
		case D3D_SIT_SAMPLER:
		{
			std::shared_ptr<GameEngineSampler> Res = GameEngineSampler::Find(UpperName);

			if (nullptr == Res)
			{
				Res = GameEngineSampler::Find("ENGINEBASE");
				// MsgAssert("다음의 샘플러가 존재하지 않아서 쉐이더에 세팅해줄수가 없습니다. : " + UpperName);
			}

			GameEngineSamplerSetter Setter;
			Setter.ParentShader = this;
			Setter.Name = UpperName;
			Setter.BindPoint = ResDesc.BindPoint;
			Setter.Res = Res;

			ResHelper.CreateSamplerSetter(Setter);
			break;
		}
		case D3D_SIT_STRUCTURED:
		{
			// 스트럭처드 버퍼는 텍스처 슬롯을 사용합니다.
			// 기본적으로 텍스처로 판단합니다.

			ID3D11ShaderReflectionConstantBuffer* SBufferPtr = CompileInfo->GetConstantBufferByName(ResDesc.Name);
			D3D11_SHADER_BUFFER_DESC BufferDesc;
			SBufferPtr->GetDesc(&BufferDesc);

			std::shared_ptr<GameEngineStructuredBuffer> Res = GameEngineStructuredBuffer::CreateAndFind(Name, BufferDesc, 0);

			GameEngineStructuredBufferSetter Setter;
			Setter.ParentShader = this;
			Setter.Name = UpperName;
			Setter.BindPoint = ResDesc.BindPoint;
			Setter.Res = Res;

			ResHelper.CreateStructuredBufferSetter(Setter);
			break;
		}
		case D3D_SIT_UAV_RWSTRUCTURED:
		{
			// 스트럭처드 버퍼는 텍스처 슬롯을 사용합니다.
			// 기본적으로 텍스처로 판단합니다.

			ID3D11ShaderReflectionConstantBuffer* SBufferPtr = CompileInfo->GetConstantBufferByName(ResDesc.Name);
			D3D11_SHADER_BUFFER_DESC BufferDesc;
			SBufferPtr->GetDesc(&BufferDesc);

			// RW버퍼로 

			std::shared_ptr<GameEngineStructuredBuffer> Res = GameEngineStructuredBuffer::CreateAndFind(Name, BufferDesc, 0);

			GameEngineStructuredBufferSetter Setter;
			Setter.ParentShader = this;
			Setter.Name = UpperName;
			Setter.BindPoint = ResDesc.BindPoint;
			Setter.Res = Res;

			ResHelper.CreateStructuredBufferSetter(Setter);
			break;
		}
		default:
			MsgAssert(std::string(ResDesc.Name) + "처리할수 없는 리소스 입니다");
			break;
		}

	}
}

void GameEngineShader::AutoCompile(GameEngineFile& _File)
{
	std::string ShaderCode = _File.GetString();

	{
		size_t EntryIndex = ShaderCode.find("_VS(");
		// unsigned __int64 == max값이 std::string::npos
		if (EntryIndex != std::string::npos)
		{
			{
				size_t FirstIndex = ShaderCode.find_last_of(" ", EntryIndex);
				std::string EntryName = ShaderCode.substr(FirstIndex + 1, EntryIndex - FirstIndex - 1);
				EntryName += "_VS";
				GameEngineVertexShader::Load(_File.GetFullPath(), EntryName);
			}
		}
	}

	{
		size_t EntryIndex = ShaderCode.find("_GS(");
		// unsigned __int64 == max값이 std::string::npos
		if (EntryIndex != std::string::npos)
		{
			{
				size_t FirstIndex = ShaderCode.find_last_of(" ", EntryIndex);
				std::string EntryName = ShaderCode.substr(FirstIndex + 1, EntryIndex - FirstIndex - 1);
				EntryName += "_GS";
				GameEngineGeometryShader::Load(_File.GetFullPath(), EntryName);
			}
		}
	}

	{
		size_t EntryIndex = ShaderCode.find("_CS(");
		if (EntryIndex != std::string::npos)
		{
			{
				size_t FirstIndex = ShaderCode.find_last_of(" ", EntryIndex);
				std::string EntryName = ShaderCode.substr(FirstIndex + 1, EntryIndex - FirstIndex - 1);
				EntryName += "_CS";
				GameEngineComputeShader::Load(_File.GetFullPath(), EntryName);
			}
		}
	}

	{
		size_t EntryIndex = ShaderCode.find("_PS(");
		// unsigned __int64 == max값이 std::string::npos
		if (EntryIndex != std::string::npos)
		{
			{
				size_t FirstIndex = ShaderCode.find_last_of(" ", EntryIndex);
				std::string EntryName = ShaderCode.substr(FirstIndex + 1, EntryIndex - FirstIndex - 1);
				EntryName += "_PS";
				std::shared_ptr<GameEnginePixelShader> Shader = GameEnginePixelShader::Load(_File.GetFullPath(), EntryName);

				if (nullptr != Shader)
				{
					size_t PixelBody = ShaderCode.rfind("\n", EntryIndex);

					std::string PixelFirst = ShaderCode.substr(PixelBody + 1, EntryIndex - PixelBody);

					if (std::string::npos != PixelFirst.find("DeferredOut"))
					{
						Shader->Path = RenderPath::Deferred;
					}
					else if (std::string::npos != PixelFirst.find("AlphaOut"))
					{
						Shader->Path = RenderPath::Alpha;
					}
				}
			}
		}
	}
}