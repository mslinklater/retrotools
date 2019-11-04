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

	printf("Info:%s\n", line.c_str());
}

void Log::Warning(std::string line)
{
	LogLine newLine;
	newLine.content = line;
	newLine.type = kWarning;
	
	allLogLines.push_back(newLine);

	printf("%s\n", line.c_str());
}

void Log::Error(std::string line)
{
	LogLine newLine;
	newLine.content = line;
	newLine.type = kError;
	
	allLogLines.push_back(newLine);

	printf("%s\n", line.c_str());
}

int Log::GetLineCount()
{
	return allLogLines.size();
}

const std::string& Log::GetLine(int number)
{
	return allLogLines[number].content;
}
