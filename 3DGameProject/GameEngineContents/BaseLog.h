#pragma once



// 설명 :
class BaseLog
{
public:

	template<typename Enum>
	static void PushLog(Enum _Enum, const std::string_view& _Log)
	{
		PushLog(static_cast<int>(_Enum), _Log);
	}

	static void PushLog(int _Order, const std::string_view& _Log);

	//template<typename Enum>
	//static void PushLog(Enum _Enum, const std::vector<std::string>& _Log)
	//{
	//	PushLog(static_cast<int>(_Enum), _Log);
	//}

	//static void PushLog(int _Order, const std::vector<std::string>& _Log);

	template<typename Enum>
	static const std::vector<std::string>& GetLog(Enum _Enum)
	{
		return GetLog(static_cast<int>(_Enum));
	}

	static const std::vector<std::string>& GetLog(int _Order);

	static const std::map<int, std::vector<std::string>>& GetAllLog()
	{
		return LogBuffers;
	}

	template<typename Enum>
	static void LogReset(Enum _Enum)
	{
		LogReset(static_cast<int>(_Enum));
	}

	static void LogReset(int _Order);

protected:

private:
	static std::map<int, std::vector<std::string>> LogBuffers;

	// 0 < Count == 제한 없음
	static std::map<int, int> LogMaxCount;

	static const int InitMaxLogCount;

	BaseLog();
	~BaseLog();

	// delete Function
	BaseLog(const BaseLog& _Other) = delete;
	BaseLog(BaseLog&& _Other) noexcept = delete;
	BaseLog& operator=(const BaseLog& _Other) = delete;
	BaseLog& operator=(BaseLog&& _Other) noexcept = delete;
};

