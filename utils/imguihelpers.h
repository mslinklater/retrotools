// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#pragma once
#include <string>
#include "common.h"

namespace ImGuiHelpers
{
	void TextTooltip(std::string text, std::string tooltip, ImVec4 colour)
	{
		ImGui::TextColored(colour, "%s", text.c_str());	  
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(tooltip.c_str());
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}	
	}
};
