#pragma once

#include <GameEngineCore/GameEngineTexture.h>
#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEnginePlatform/GameEngineSound.h>

// 설명 :
class AutoResourceLoader
{
public:
	class LoadData
	{
	public:
		std::map<std::string, std::string> TextureNames;
		std::map<std::string, std::string> MeshNames;
		std::map<std::string, std::string> AnimationNames;
		std::map<std::string, std::string> SoundNames;
	};

	friend void LoadDataLoop(GameEngineThread* _Thread, AutoResourceLoader* _Loader, AutoResourceLoader::LoadData& _Datas);
	friend void UnLoadDataLoop(GameEngineThread* _Thread, AutoResourceLoader* _Loader, AutoResourceLoader::LoadData& _Datas);

	friend void TextureUnLoad(GameEngineThread* _Thread, AutoResourceLoader* _Loader, const std::string_view& _Name);
	friend void MeshUnLoad(GameEngineThread* _Thread, AutoResourceLoader* _Loader, const std::string_view& _Name);
	friend void AnimationUnLoad(GameEngineThread* _Thread, AutoResourceLoader* _Loader, const std::string_view& _Name);
	friend void SoundUnLoad(GameEngineThread* _Thread, AutoResourceLoader* _Loader, const std::string_view& _Name);

	friend void TextureLoad(GameEngineThread* _Thread, AutoResourceLoader* _Loader, const std::string_view& _Name, const std::string_view& _Path);
	friend void MeshLoad(GameEngineThread* _Thread, AutoResourceLoader* _Loader, const std::string_view& _Name, const std::string_view& _Path);
	friend void AnimationLoad(GameEngineThread* _Thread, AutoResourceLoader* _Loader, const std::string_view& _Name, const std::string_view& _Path);
	friend void SoundLoad(GameEngineThread* _Thread, AutoResourceLoader* _Loader, const std::string_view& _Name, const std::string_view& _Path);

public:
	// constrcuter destructer
	AutoResourceLoader();
	~AutoResourceLoader();

	// delete Function
	AutoResourceLoader(const AutoResourceLoader& _Other) = delete;
	AutoResourceLoader(AutoResourceLoader&& _Other) noexcept = delete;
	AutoResourceLoader& operator=(const AutoResourceLoader& _Other) = delete;
	AutoResourceLoader& operator=(AutoResourceLoader&& _Other) noexcept = delete;

	void AddGroupTexture(int _Group, const std::string_view& _TextureName, const std::string_view& _Path);
	void AddGroupFBXMesh(int _Group, const std::string_view& _FBXMeshName, const std::string_view& _Path);
	void AddGroupFBXAnimation(int _Group, const std::string_view& _FBXAnimName, const std::string_view& _Path);
	void AddGroupSound(int _Group, const std::string_view& _FBXSoundName, const std::string_view& _Path);

	// 실시간 스레드 로드/언로드를 시작합니다.
	void LoadStart(int _LoadGroup, int _UnloadGroup, const std::vector<int>& _UseGroup);

	// 현재 로드/언로드 중인지 반환합니다.
	inline bool IsEnd() const
	{
		return IsLoadEnd || IsUnLoadEnd;
	}


protected:

private:
	std::atomic_bool IsLoadEnd = true;
	std::atomic_bool IsUnLoadEnd = true;

	std::atomic<UINT> TextureLoadCount = 0;
	std::atomic<UINT> MeshLoadCount = 0;
	std::atomic<UINT> AnimationLoadCount = 0;
	std::atomic<UINT> SoundLoadCount = 0;

	std::atomic<UINT> TextureUnLoadCount = 0;
	std::atomic<UINT> MeshUnLoadCount = 0;
	std::atomic<UINT> AnimationUnLoadCount = 0;
	std::atomic<UINT> SoundUnLoadCount = 0;

	std::map<int, LoadData> GroupDatas;

	void LoadCountReset();
	void UnloadExceptionCheck(int _UnloadGroup, const std::vector<int>& _UseGroup);
	void LoadExceptionCheck(int _LoadGroup, const std::vector<int>& _UseGroup);
};