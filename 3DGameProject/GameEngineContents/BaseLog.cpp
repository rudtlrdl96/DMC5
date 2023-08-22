#include "PrecompileHeader.h"
#include "BaseLog.h"

std::map<int, std::vector<std::string>> BaseLog::LogBuffers;
std::map<int, int> BaseLog::LogMaxCount;
const int BaseLog::InitMaxLogCount = 20;

void BaseLog::PushLog(int _Order, const std::string_view& _Log)
{
	std::vector<std::string>& LogDatas = LogBuffers[_Order];

	LogDatas.push_back(_Log.data());

	if (false == LogMaxCount.contains(_Order))
	{
		LogMaxCount[_Order] = InitMaxLogCount;
	}

	int MaxCount = LogMaxCount[_Order];

	if (0 >= MaxCount)
	{
		return;
	}

	if (LogDatas.size() > MaxCount)
	{
		size_t OverCount = LogDatas.size() - MaxCount - 1;
		LogDatas.erase(LogDatas.begin(), LogDatas.begin() + OverCount);
	}
}

const std::vector<std::string>& BaseLog::GetLog(int _Order)
{
	std::map<int, std::vector<std::string>>::iterator FindIter = LogBuffers.find(_Order);

	if (FindIter == LogBuffers.end())
	{
		return LogBuffers[_Order];
	}

	return FindIter->second;
}

void BaseLog::LogReset(int _Order)
{
	LogBuffers[_Order].clear();
}

BaseLog::BaseLog()
{
}

BaseLog::~BaseLog() 
{
}

