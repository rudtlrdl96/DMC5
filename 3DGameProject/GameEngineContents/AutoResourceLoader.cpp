#include "PrecompileHeader.h"
#include "AutoResourceLoader.h"

#include <GameEngineBase/GameEngineThread.h>

AutoResourceLoader::AutoResourceLoader() 
{
}

AutoResourceLoader::~AutoResourceLoader() 
{
}

void TextureUnLoad(GameEngineThread* _Thread, AutoResourceLoader* _Loader, const std::string_view& _Name)
{
	GameEngineTexture::UnLoad(_Name);
	++_Loader->TextureUnLoadCount;
}

void MeshUnLoad(GameEngineThread* _Thread, AutoResourceLoader* _Loader, const std::string_view& _Name)
{
	GameEngineFBXMesh::UnLoad(_Name);
	++_Loader->TextureUnLoadCount;
}

void AnimationUnLoad(GameEngineThread* _Thread, AutoResourceLoader* _Loader, const std::string_view& _Name)
{
	GameEngineFBXAnimation::UnLoad(_Name);
	++_Loader->TextureUnLoadCount;
}

void SoundUnLoad(GameEngineThread* _Thread, AutoResourceLoader* _Loader, const std::string_view& _Name)
{
	GameEngineSound::UnLoad(_Name);
	++_Loader->SoundUnLoadCount;
}

void TextureLoad(GameEngineThread* _Thread, AutoResourceLoader* _Loader, const std::string_view& _Name, const std::string_view& _Path)
{
	GameEngineTexture::Load(_Path, _Name);
	++_Loader->TextureLoadCount;
}

void MeshLoad(GameEngineThread* _Thread, AutoResourceLoader* _Loader, const std::string_view& _Name, const std::string_view& _Path)
{
	GameEngineFBXMesh::Load(_Path, _Name);
	++_Loader->TextureLoadCount;
}

void AnimationLoad(GameEngineThread* _Thread, AutoResourceLoader* _Loader, const std::string_view& _Name, const std::string_view& _Path)
{
	GameEngineFBXAnimation::Load(_Path, _Name);
	++_Loader->TextureLoadCount;
}

void SoundLoad(GameEngineThread* _Thread, AutoResourceLoader* _Loader, const std::string_view& _Name, const std::string_view& _Path)
{
	GameEngineSound::Load(_Name, _Path);
	++_Loader->SoundLoadCount;
}


void UnLoadDataLoop(GameEngineThread* _Thread, AutoResourceLoader* _Loader, AutoResourceLoader::LoadData& _Datas)
{
	{
		std::map<std::string, std::string>::iterator TextureLoop = _Datas.TextureNames.begin();
		std::map<std::string, std::string>::iterator TextureEnd = _Datas.TextureNames.end();
	
		while (TextureLoop != TextureEnd)
		{
			GameEngineCore::JobQueue.Work(std::bind(TextureUnLoad, std::placeholders::_1, _Loader, TextureLoop->first));
		}
	}

	{
		std::map<std::string, std::string>::iterator MeshLoop = _Datas.MeshNames.begin();
		std::map<std::string, std::string>::iterator MeshEnd = _Datas.MeshNames.end();

		while (MeshLoop != MeshEnd)
		{
			GameEngineCore::JobQueue.Work(std::bind(MeshUnLoad, std::placeholders::_1, _Loader, MeshLoop->first));
		}
	}

	{
		std::map<std::string, std::string>::iterator AnimationLoop = _Datas.AnimationNames.begin();
		std::map<std::string, std::string>::iterator AnimationEnd = _Datas.AnimationNames.end();

		while (AnimationLoop != AnimationEnd)
		{
			GameEngineCore::JobQueue.Work(std::bind(AnimationUnLoad, std::placeholders::_1, _Loader, AnimationLoop->first));
		}
	}

	{
		std::map<std::string, std::string>::iterator SoundLoop = _Datas.SoundNames.begin();
		std::map<std::string, std::string>::iterator SoundEnd = _Datas.SoundNames.end();

		while (SoundLoop != SoundEnd)
		{
			GameEngineCore::JobQueue.Work(std::bind(SoundUnLoad, std::placeholders::_1, _Loader, SoundLoop->first));
		}
	}

	while (   
		_Loader->TextureUnLoadCount < _Datas.TextureNames.size() &&
		_Loader->MeshUnLoadCount < _Datas.MeshNames.size() &&
		_Loader->AnimationUnLoadCount < _Datas.AnimationNames.size() &&
		_Loader->SoundUnLoadCount < _Datas.SoundNames.size()		
		)		
	{
		// Loop....
	}

	_Loader->IsUnLoadEnd = true;
}

void LoadDataLoop(GameEngineThread* _Thread, AutoResourceLoader* _Loader, AutoResourceLoader::LoadData& _Datas)
{
	{
		std::map<std::string, std::string>::iterator TextureLoop = _Datas.TextureNames.begin();
		std::map<std::string, std::string>::iterator TextureEnd = _Datas.TextureNames.end();

		while (TextureLoop != TextureEnd)
		{
			GameEngineCore::JobQueue.Work(std::bind(TextureLoad, std::placeholders::_1, _Loader, TextureLoop->first, TextureLoop->second));
		}
	}

	{
		std::map<std::string, std::string>::iterator MeshLoop = _Datas.MeshNames.begin();
		std::map<std::string, std::string>::iterator MeshEnd = _Datas.MeshNames.end();

		while (MeshLoop != MeshEnd)
		{
			GameEngineCore::JobQueue.Work(std::bind(MeshLoad, std::placeholders::_1, _Loader, MeshLoop->first, MeshLoop->second));
		}
	}

	{
		std::map<std::string, std::string>::iterator AnimationLoop = _Datas.AnimationNames.begin();
		std::map<std::string, std::string>::iterator AnimationEnd = _Datas.AnimationNames.end();

		while (AnimationLoop != AnimationEnd)
		{
			GameEngineCore::JobQueue.Work(std::bind(AnimationLoad, std::placeholders::_1, _Loader, AnimationLoop->first, AnimationLoop->second));
		}
	}

	{
		std::map<std::string, std::string>::iterator SoundLoop = _Datas.SoundNames.begin();
		std::map<std::string, std::string>::iterator SoundEnd = _Datas.SoundNames.end();

		while (SoundLoop != SoundEnd)
		{
			GameEngineCore::JobQueue.Work(std::bind(SoundLoad, std::placeholders::_1, _Loader, SoundLoop->first, SoundLoop->second));
		}
	}

	while (
		_Loader->TextureLoadCount < _Datas.TextureNames.size() &&
		_Loader->MeshLoadCount < _Datas.MeshNames.size() &&
		_Loader->AnimationLoadCount < _Datas.AnimationNames.size() &&
		_Loader->SoundLoadCount < _Datas.SoundNames.size()
		)
	{
		// Loop....
	}

	_Loader->IsLoadEnd = true;
}

void AutoResourceLoader::LoadStart(int _LoadGroup, int _UnloadGroup, const std::vector<int>& _UseGroup)
{
	if (false == IsEnd())
	{
		MsgAssert("이미 로드/언로드가 실행중입니다.");
		return;
	}

	if (_LoadGroup == _UnloadGroup)
	{
		MsgAssert("같은 그룹을 언로드, 로드하려 했습니다.");
		return;
	}

	LoadCountReset();

	if (true == GroupDatas.contains(_UnloadGroup))
	{
		UnloadExceptionCheck(_UnloadGroup, _UseGroup);
		GameEngineCore::JobQueue.Work(std::bind(UnLoadDataLoop, std::placeholders::_1, this, GroupDatas[_UnloadGroup]));
		IsUnLoadEnd = false;
	}

	if (true == GroupDatas.contains(_UnloadGroup))
	{
		GameEngineCore::JobQueue.Work(std::bind(LoadDataLoop, std::placeholders::_1, this, GroupDatas[_UnloadGroup]));
		IsLoadEnd = false;
	}

}

void AutoResourceLoader::AddGroupTexture(int _Group, const std::string_view& _TextureName, const std::string_view& _Path)
{
	if (false == IsEnd())
	{
		MsgAssert("로드/언로드가 실행중에는 Texture를 추가할 수 없습니다.");
		return;
	}

	std::string UpperName = GameEngineString::ToUpper(_TextureName);
	GroupDatas[_Group].TextureNames[UpperName] = _Path;
}

void AutoResourceLoader::AddGroupFBXMesh(int _Group, const std::string_view& _FBXMeshName, const std::string_view& _Path)
{
	if (false == IsEnd())
	{
		MsgAssert("로드/언로드가 실행중에는 Mesh를 추가할 수 없습니다.");
		return;
	}

	std::string UpperName = GameEngineString::ToUpper(_FBXMeshName);
	GroupDatas[_Group].MeshNames[UpperName] = _Path;
}

void AutoResourceLoader::AddGroupFBXAnimation(int _Group, const std::string_view& _FBXAnimName, const std::string_view& _Path)
{
	if (false == IsEnd())
	{
		MsgAssert("로드/언로드가 실행중에는 Animation를 추가할 수 없습니다.");
		return;
	}

	std::string UpperName = GameEngineString::ToUpper(_FBXAnimName);
	GroupDatas[_Group].AnimationNames[UpperName] = _Path;
}

void AutoResourceLoader::AddGroupSound(int _Group, const std::string_view& _FBXSoundName, const std::string_view& _Path)
{
	if (false == IsEnd())
	{
		MsgAssert("로드/언로드가 실행중에는 Sound를 추가할 수 없습니다.");
		return;
	}

	std::string UpperName = GameEngineString::ToUpper(_FBXSoundName);
	GroupDatas[_Group].SoundNames[UpperName] = _Path;
}

void AutoResourceLoader::LoadCountReset()
{
	TextureLoadCount = 0;
	MeshLoadCount = 0;
	AnimationLoadCount = 0;
	SoundLoadCount = 0;

	TextureUnLoadCount = 0;
	MeshUnLoadCount = 0;
	AnimationUnLoadCount = 0;
	SoundUnLoadCount = 0;
}

void AutoResourceLoader::UnloadExceptionCheck(int _UnloadGroup, const std::vector<int>& _UseGroup)
{
	{
		std::map<std::string, std::string>::iterator TextureLoop = GroupDatas[_UnloadGroup].TextureNames.begin();
		std::map<std::string, std::string>::iterator TextureEnd = GroupDatas[_UnloadGroup].TextureNames.end();
		
		while (TextureLoop != TextureEnd)
		{
			for (size_t i = 0; i < _UseGroup.size(); i++)
			{
				if (_UseGroup[i]== _UnloadGroup)
				{
					continue;
				}

				if (true == GroupDatas[_UseGroup[i]].TextureNames.contains(TextureLoop->first))
				{
					TextureLoop = GroupDatas[_UnloadGroup].TextureNames.erase(TextureLoop);
				}
				else
				{
					++TextureLoop;
				}
			}
		}
	}

	{
		std::map<std::string, std::string>::iterator MeshLoop = GroupDatas[_UnloadGroup].MeshNames.begin();
		std::map<std::string, std::string>::iterator MeshEnd = GroupDatas[_UnloadGroup].MeshNames.end();

		while (MeshLoop != MeshEnd)
		{
			for (size_t i = 0; i < _UseGroup.size(); i++)
			{
				if (_UseGroup[i] == _UnloadGroup)
				{
					continue;
				}

				if (true == GroupDatas[_UseGroup[i]].MeshNames.contains(MeshLoop->first))
				{
					MeshLoop = GroupDatas[_UnloadGroup].MeshNames.erase(MeshLoop);
				}
				else
				{
					++MeshLoop;
				}
			}
		}
	}

	{
		std::map<std::string, std::string>::iterator AnimationLoop = GroupDatas[_UnloadGroup].AnimationNames.begin();
		std::map<std::string, std::string>::iterator AnimationEnd = GroupDatas[_UnloadGroup].AnimationNames.end();

		while (AnimationLoop != AnimationEnd)
		{
			for (size_t i = 0; i < _UseGroup.size(); i++)
			{
				if (_UseGroup[i] == _UnloadGroup)
				{
					continue;
				}

				if (true == GroupDatas[_UseGroup[i]].AnimationNames.contains(AnimationLoop->first))
				{
					AnimationLoop = GroupDatas[_UnloadGroup].AnimationNames.erase(AnimationLoop);
				}
				else
				{
					++AnimationLoop;
				}
			}
		}
	}

	{
		std::map<std::string, std::string>::iterator SoundLoop = GroupDatas[_UnloadGroup].SoundNames.begin();
		std::map<std::string, std::string>::iterator SoundEnd = GroupDatas[_UnloadGroup].SoundNames.end();

		while (SoundLoop != SoundEnd)
		{
			for (size_t i = 0; i < _UseGroup.size(); i++)
			{
				if (_UseGroup[i] == _UnloadGroup)
				{
					continue;
				}

				if (true == GroupDatas[_UseGroup[i]].SoundNames.contains(SoundLoop->first))
				{
					SoundLoop = GroupDatas[_UnloadGroup].SoundNames.erase(SoundLoop);
				}
				else
				{
					++SoundLoop;
				}
			}
		}
	}
}