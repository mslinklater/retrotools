#pragma once

#include <string>
#include "3rdparty/imgui/imgui.h"

namespace TextFormat
{
	static const ImVec4 kColourNormal(ImVec4(0.6,0.6,0.6,1.0));
	static const ImVec4 kColourBold(ImVec4(1.0,1.0,1.0,1.0));
	static const ImVec4 kColourWarning(ImVec4(1.0,1.0,0.0,1.0));
	static const ImVec4 kColourError(ImVec4(1.0,0.0,0.0,1.0));

	static const ImVec4 kColourRed(ImVec4(1.0,0.0,0.0,1.0));
	static const ImVec4 kColourGreen(ImVec4(0.0,1.0,0.0,1.0));
	static const ImVec4 kColourBlue(ImVec4(0.0,0.0,1.0,1.0));

	static const std::string kBoldTag("[bold]");
	static const std::size_t kBoldTagSize(kBoldTag.size());
}
