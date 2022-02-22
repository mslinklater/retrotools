// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include "binarywindow.h"
#include "resources/resource_binary.h"
#include "system/common.h"

BinaryWindow::BinaryWindow()
{

}

BinaryWindow::~BinaryWindow()
{

}

void BinaryWindow::Draw()
{
	const ResourceBinary* pRes = (const ResourceBinary*)pResource.get();

//	int byteOffset = 0;
	int dataSize = pRes->GetDataSize();
	for (int address = 0; address < dataSize ; address += 16)
	{
		int rowOffset = 0;
		// print address
		while ((address + rowOffset < dataSize) && (rowOffset < 16))
		{

			rowOffset++;
		}
	}
}
