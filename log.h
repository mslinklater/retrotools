// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once

#include <vector>
#include <string>
#include <stdarg.h>                 // va_list, va_start, va_end

#define LOGINFO(x) Log::Instance()->Info(x)
#define LOGINFOF(x, ...) Log::Instance()->Infof(x, __VA_ARGS__)

#define LOGWARNING(x) Log::Instance()->Warning(x)
#define LOGWARNINGF(x, ...) Log::Instance()->Warningf(x, __VA_ARGS__)

#define LOGERROR(x) Log::Instance()->Error(x)
#define LOGERRORF(x, ...) Log::Instance()->Errorf(x, __VA_ARGS__)

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
	
	void Test();
	
	void Info(std::string line);
	void Infof(const char* fmt, ...);
	
	void Warning(std::string line);
	void Warningf(const char* fmt, ...);
	
	void Error(std::string line);
	void Errorf(const char* fmt, ...);
	
	int GetLineCount();
	const LogLine& GetLine(int number);
	
private:
	std::vector<LogLine> allLogLines;
};
