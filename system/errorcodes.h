// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

// Could really use exceptions for this I suppose.

#pragma once

#include <map>
#include <string>

enum eErrorCode {
	kError_OK = 0,
	kError_CommandLineParsing,
	kError_MemoryAlreadyInitialised,
	kError_MemoryNotInitialised,
	kError_FileNotFound,
	kError_JSONParse,

	// this last one shoud be the first index in project-specific error conditions
	kError_StartOfProjectErrors
};

class Error
{
	public:
//		static void Error(uint32_t code, std::string extra = "");
	private:
		static std::map<uint32_t, std::string> errorMap;
};

