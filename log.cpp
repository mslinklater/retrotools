// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "log.h"

static Log* pInstance = nullptr;

#define BUFFER_SIZE 4096

static char buffer[BUFFER_SIZE];

Log::Log()
{
}

Log::~Log()
{
}

Log * Log::Instance()
{
	if(pInstance == nullptr)
	{
		pInstance = new Log();
	}
	return pInstance;
}

void Log::RecalculateDisplayedLines()
{
}

bool Log::SplitCategory(std::string line, std::string& categoryOut, std::string& lineOut)
{
	
	return true;
}


void Log::Info(std::string line)
{
	LogLine newLine;
	
	SplitCategory(line, newLine.category, newLine.content);
	
	newLine.content = line;
	newLine.type = kInfo;
	
	allLogLines.push_back(newLine);
	printf("Info:%s\n", line.c_str());
}

void Log::Test()
{
	LOGINFO("-- retrotool --\n");
	LOGINFO("Test Info");
	LOGWARNING("Test Warning");
	LOGERROR("Test Error");

	LOGINFOF("Test Info variadic %d", 999);
	LOGWARNINGF("Test Warning variadic %d", 999);
	LOGERRORF("Test Error variadic %d", 999);	
}

void Log::Infof(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
	vsnprintf(buffer, BUFFER_SIZE, fmt, args );
    va_end(args);
	
	LogLine newLine;
	newLine.content = std::string(buffer);
	newLine.type = kInfo;
	
	allLogLines.push_back(newLine);
	printf("Info:%s\n", newLine.content.c_str());
}

void Log::Warning(std::string line)
{
	LogLine newLine;
	newLine.content = line;
	newLine.type = kWarning;
	
	allLogLines.push_back(newLine);
	printf("Warning:%s\n", line.c_str());
}

void Log::Warningf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
	vsnprintf(buffer, BUFFER_SIZE, fmt, args );
    va_end(args);
	
	LogLine newLine;
	newLine.content = std::string(buffer);
	newLine.type = kWarning;
	
	allLogLines.push_back(newLine);
	printf("Warning:%s\n", newLine.content.c_str());
}

void Log::Error(std::string line)
{
	LogLine newLine;
	newLine.content = line;
	newLine.type = kError;
	
	allLogLines.push_back(newLine);
	printf("Error:%s\n", line.c_str());
}

void Log::Errorf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
	vsnprintf(buffer, BUFFER_SIZE, fmt, args );
    va_end(args);
	
	LogLine newLine;
	newLine.content = std::string(buffer);
	newLine.type = kError;
	
	allLogLines.push_back(newLine);
	printf("Error:%s\n", newLine.content.c_str());
}

int Log::GetLineCount()
{
	return allLogLines.size();
}

const Log::LogLine& Log::GetLine(int number)
{
	return allLogLines[number];
}
