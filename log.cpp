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

//void Log::Infof(std::string line, va_list)
//{
//	
//}

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
