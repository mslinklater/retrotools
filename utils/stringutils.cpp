#include "stringutils.h"

void stringutils::splitByString( const std::string& source, const std::string& separator, std::vector<std::string> output)
{
	output.clear();

	size_t prevSearchPos = 0;
	size_t searchPos = source.find(separator, prevSearchPos);

	if( searchPos == source.npos )
	{
		output.push_back(source);
		return;
	}

	while (searchPos != source.npos) 
	{
		output.push_back(source.substr(prevSearchPos, searchPos - prevSearchPos));
		prevSearchPos = searchPos + separator.length();
		searchPos = source.find(separator, prevSearchPos);
	}
	
	output.push_back(source.substr(prevSearchPos, source.length() - prevSearchPos));

	return;
}


