#pragma once
#include <imgui.h>
#include "widgets.h"

bool Widgets::AutoUpdateCheckBox(bool* autoUpdateSetting)
{
	return ImGui::Checkbox("Constant Updates?", autoUpdateSetting);
}