#pragma once

#include <vector>
#include <string>

#define LOGINFO(x) Log::Instance()->Info(x)
#define LOGWARNING(x) Log::Instance()->Warning(x)
#define LOGERROR(x) Log::Instance()->Error(x)

class Log
{
public:
	enum eLogType
	{
		kInfo,
		kWarning,
		kError
	};
	
	struct LogLine
	{
		eLogType 	type;
		std::string content;
	};
	
	Log();
	virtual ~Log();
	static Log* Instance();
	
	void Info(std::string line);
	void Warning(std::string line);
	void Error(std::string line);
	
private:
	std::vector<LogLine> allLogLines;
};
