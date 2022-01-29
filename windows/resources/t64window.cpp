#include "t64window.h"
#include "resources/resource_t64.h"
#include "common.h"

T64Window::T64Window()
{

}

T64Window::~T64Window()
{

}

void T64Window::Draw()
{
	const ResourceT64* pRes = (const ResourceT64*)pResource.get();

	ImGui::Text("%s", pRes->GetTapeRecord()->GetDescriptor().c_str());
}
