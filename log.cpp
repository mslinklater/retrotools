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

void Log::Info(std::string line)
{
	LogLine newLine;
	newLine.content = line;
	newLine.type = kInfo;
	
	allLogLines.push_back(newLine);
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
}

void Log::Warning(std::string line)
{
	LogLine newLine;
	newLine.content = line;
	newLine.type = kWarning;
	
	allLogLines.push_back(newLine);
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
}

void Log::Error(std::string line)
{
	LogLine newLine;
	newLine.content = line;
	newLine.type = kError;
	
	allLogLines.push_back(newLine);
	printf("%s", line.c_str());
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
}

int Log::GetLineCount()
{
	return allLogLines.size();
}

const Log::LogLine& Log::GetLine(int number)
{
	return allLogLines[number];
}
