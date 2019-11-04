#include "log.h"

static Log* pInstance = nullptr;

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

void Log::Warning(std::string line)
{
	LogLine newLine;
	newLine.content = line;
	newLine.type = kWarning;
	
	allLogLines.push_back(newLine);
}

void Log::Error(std::string line)
{
	LogLine newLine;
	newLine.content = line;
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
