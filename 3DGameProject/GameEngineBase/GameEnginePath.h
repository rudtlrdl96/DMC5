#pragma once
// c++이 실행되는 곳에서 사용할수 있는 모든 문자열 관련 처리방식을 지원해준다.
#include <filesystem>

// 설명 : 경로에 관련된 기능을 대표하는 클래스
//class GameEngineFile;
//class GameEngineDirectory;
class GameEnginePath
{
	// 전방선언을 그냥 필요
	friend class GameEngineFile;
	friend class GameEngineDirectory;

public:
	static std::string GetFileFullPath(const std::string_view& _RootDirectory, const std::vector<std::string>& _Moves);
	static std::string GetFileFullPath(const std::string_view& _RootDirectory, const std::vector<std::string>& _Moves, const std::string_view& _FileName);

public:
	// constrcuter destructer
	GameEnginePath();
	GameEnginePath(std::filesystem::path _Path);
	GameEnginePath(const std::string& _Path);
	~GameEnginePath();

	// delete Function
	GameEnginePath(const GameEnginePath& _Other);
	GameEnginePath(GameEnginePath&& _Other);
	GameEnginePath& operator=(const GameEnginePath& _Other) = delete;
	GameEnginePath& operator=(GameEnginePath&& _Other) noexcept = delete;

	static std::string GetFileName(const std::string_view& _Path);
	static std::string GetFolderPath(const std::string_view& _Path);

	std::string GetFileName() const;
	std::string GetFullPath() const;
	std::string GetExtension() const;

	void MoveParent();

	// 내자식중 특정 경로나 특정 파일이 있는곳까지 자동 move
	void MoveParentToChildPath(const std::string_view& _String);

	bool IsExists();
	bool IsExistsToPlusString(const std::string_view& _String);

	// bool MoveParentToChildPath(const std::string_view& _IOName );

	bool IsDirectory() const;

	bool IsRoot();

	bool Move(const std::string_view& _Path);

	void SetPath(const std::string_view& _Path);


protected:

private:
	// 고생고생해서 만들지 않게 되었습니다.
	// std::string Path;
	std::filesystem::path Path;

};

