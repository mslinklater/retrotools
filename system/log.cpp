// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

// Copyright (c) 2019, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "common.h"

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

void Log::ResetCategories()
{
	// clear categories set
	categories.clear();

	// go through log lines and add cetagories to categories set
	for(auto line : allLogLines)
	{
		if((line.category.length() > 0) && (categories.find(line.category) == categories.end()))
		{
			categories.insert(line.category);
			categoryFilter[line.category] = true;
		}
	}
}

void Log::RecalculateDisplayedLines()
{
	filteredLogLines.clear();
	for(LogLine line : allLogLines)
	{
		if(line.category.size() > 0)
		{
			if(categoryFilter[line.category])
			{
				filteredLogLines.push_back(line);
			}
		}
		else
		{
			filteredLogLines.push_back(line);
		}
	}
}

void Log::SplitCategory(std::string line, std::string& categoryOut, std::string& lineOut)
{
	size_t findPos = line.find("::");
	if(findPos == std::string::npos)
	{
		lineOut = line;
		categoryOut = "";
	}
	else
	{
		categoryOut = line.substr(0, findPos);
		lineOut = line.substr(findPos+2);
	}
}

void Log::Info(std::string line)
{
	LogLine newLine;
	
	SplitCategory(line, newLine.category, newLine.content);	
	newLine.type = kInfo;

	AddLine(newLine);
	printf("Info:%s\n", line.c_str());
}

void Log::Test()
{
	LOGINFO("-- retrotool --\n");
	LOGINFO("Test::Info");
	LOGWARNING("Test::Warning");
	LOGERROR("Test::Error");

	LOGINFOF("Test::Info variadic %d", 999);
	LOGWARNINGF("Test::Warning variadic %d", 999);
	LOGERRORF("Test::Error variadic %d", 999);	
}

void Log::Infof(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
	vsnprintf(buffer, BUFFER_SIZE, fmt, args );
    va_end(args);
	
	LogLine newLine;
	SplitCategory(buffer, newLine.category, newLine.content);	
	newLine.type = kInfo;
	
	AddLine(newLine);
	printf("Info:%s\n", newLine.content.c_str());
}

void Log::Warning(std::string line)
{
	LogLine newLine;
	SplitCategory(line, newLine.category, newLine.content);	
	newLine.type = kWarning;
	
	AddLine(newLine);
	printf("Warning:%s\n", line.c_str());
}

void Log::Warningf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
	vsnprintf(buffer, BUFFER_SIZE, fmt, args );
    va_end(args);
	
	LogLine newLine;
	SplitCategory(buffer, newLine.category, newLine.content);	
	newLine.type = kWarning;
	
	AddLine(newLine);
	printf("Warning:%s\n", newLine.content.c_str());
}

void Log::Error(std::string line)
{
	LogLine newLine;
	SplitCategory(line, newLine.category, newLine.content);	
	newLine.type = kError;
	
	AddLine(newLine);
	printf("Error:%s\n", line.c_str());
}

void Log::Errorf(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
	vsnprintf(buffer, BUFFER_SIZE, fmt, args );
    va_end(args);
	
	LogLine newLine;
	SplitCategory(buffer, newLine.category, newLine.content);	
	newLine.type = kError;
	
	AddLine(newLine);
	printf("Error:%s\n", newLine.content.c_str());
}

void Log::AddLine(Log::LogLine line)
{
	if(line.category.size() > 0)
	{
		if(categories.find(line.category) == categories.end())
		{
			categories.insert(line.category);
			categoryFilter[line.category] = true;
		}
	}
	allLogLines.push_back(line);	
	RecalculateDisplayedLines();
}

const std::set<std::string> & Log::GetCategories()
{
	return categories;
}

bool Log::GetCategoryEnabled(std::string category)
{
	return categoryFilter[category];
}

void Log::SetCategoryEnabled(std::string category, bool enabled)
{
	if(enabled != categoryFilter[category])
	{
		// changed
		categoryFilter[category] = enabled;
		RecalculateDisplayedLines();
	}
}

int Log::GetLineCount()
{
	return filteredLogLines.size();
}

const Log::LogLine& Log::GetLine(int number)
{
	return filteredLogLines[number];
}
