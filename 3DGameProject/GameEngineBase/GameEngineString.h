#pragma once
#include <string>
#include <string_view>

// 정수 == 특정글자 1개를 매칭하는 방식

// 멀티바이트 : 글자를 1바이트나 2바이트로 표현하는 인코딩 방식
// 멀티바이트 ansi표현방식이라고 불리기도 합니다.
// 유니코드 : 글자를 2바이트로 표현하는 인코딩 방식
// => 2바이트 이하 글자표현 전세계 모든 글자를 2바이트 안에 담을수 없었다.

// 그래스 UTF 방식등등이 나왔다.
// 그래스 UTF 1~3 4바이트로만 3바이트로만
// 비주얼스튜디오는 알수없는 표현식입니다.

// 설명 :
class GameEngineString
{
public:
	static std::string ToUpper(const std::string_view& _Str);
	static std::string ToString(int Value);

	static std::wstring AnsiToUniCode(const std::string_view& _Text);
	static std::string UniCodeToAnsi(const std::wstring_view& _Text);

	static std::string UniCodeToUTF8(const std::wstring_view& _Text);
	static std::string AnsiToUTF8(const std::string_view& _Text);

protected:

private:
	// constrcuter destructer
	GameEngineString();
	~GameEngineString();
	GameEngineString(const GameEngineString& _Other) = delete;
	GameEngineString(GameEngineString&& _Other) noexcept = delete;
	GameEngineString& operator=(const GameEngineString& _Other) = delete;
	GameEngineString& operator=(GameEngineString&& _Other) noexcept = delete;
};

