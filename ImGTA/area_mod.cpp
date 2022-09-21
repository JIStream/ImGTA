/*
 * Copyright (c) 2021, Rayope
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "area_mod.h"

#include "script.h"
#include "anim_dict.h"
#include "utils.h"

#include "natives.h"

#include "imgui_extras.h"
#include "widgets.h"

#include "graphics.h"

void AreaMod::Load()
{
	m_settings = m_dllObject.GetUserSettings().area;
}

void AreaMod::Unload()
{
	m_dllObject.GetUserSettings().area = m_settings;
}

void AreaMod::Think()
{
	if ((m_settings.common.autoUpdate || m_wantsUpdate))
	{
		Ped ped = PLAYER::PLAYER_PED_ID();
		if (ENTITY::DOES_ENTITY_EXIST(ped))
		{
			m_currentPos = ENTITY::GET_ENTITY_COORDS(ped, false);

			m_isInArea = ENTITY::IS_ENTITY_IN_AREA(ped,
				m_areaStartPoint.x, m_areaStartPoint.y, m_areaStartPoint.z,
				m_areaEndPoint.x, m_areaEndPoint.x, m_areaEndPoint.x,
				m_areaUnk0, m_areaUnk1, (Any)m_areaUnk2);

			m_isAtCoord = ENTITY::IS_ENTITY_AT_COORD(ped,
				m_coordPoint.x, m_coordPoint.y, m_coordPoint.z,
				m_coordSize.x, m_coordSize.y, m_coordSize.z,
				m_coordUnk0, m_coordUnk1, m_coordUnk2);
		}

		if (m_settings.drawBox)
		{
			GRAPHICS::DRAW_BOX(m_startBoxPoint.x, m_startBoxPoint.y, m_startBoxPoint.z,
				m_endBoxPoint.x, m_endBoxPoint.y, m_endBoxPoint.z,
				220, 50, 50, 125);

			// Marker types: https://docs.fivem.net/docs/game-references/markers/
			GRAPHICS::DRAW_MARKER(0, m_startBoxPoint.x, m_startBoxPoint.y, m_startBoxPoint.z,
				0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 1.0f,
				50, 220, 50, 125,
				TRUE, TRUE, 2, TRUE,
				NULL, NULL, FALSE);

			GRAPHICS::DRAW_MARKER(0, m_endBoxPoint.x, m_endBoxPoint.y, m_endBoxPoint.z,
				0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 1.0f,
				50, 220, 50, 125,
				TRUE, TRUE, 2, TRUE,
				NULL, NULL, FALSE);

			if (m_areaSameBox)
			{
				m_areaStartPoint = m_startBoxPoint;
				m_areaEndPoint = m_endBoxPoint;
			}
		}

		if (m_settings.drawAngledArea)
		{
			for (auto area : m_angledAreas) {
				drawAngledArea(area.m_angledAreaStartPoint, area.m_angledAreaEndPoint, area.m_angledAreaWidth, area.red, area.blue, area.green, 125);
			}
		}

		if (m_settings.drawSphere) {
			float scaleFixed = m_sphereRadius / 0.996095f;
			GRAPHICS::DRAW_MARKER(28, m_centerPoint.x, m_centerPoint.y, m_centerPoint.z, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, scaleFixed, scaleFixed, scaleFixed, 10, 200, 10, 128, false, false, 2, false, NULL, NULL, false);
		}

		m_wantsUpdate = false;
	}
}

void AreaMod::DrawMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		ImGui::Checkbox("##Enable HUD", &m_settings.common.showInGame);

		if (ImGui::BeginMenu("HUD"))
		{
			DrawCommonSettingsMenus(m_settings.common);

			ImGui::Separator();
			ImGui::InputFloat("Z offset", &m_settings.drawOffsetZ, 0.1f);

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

bool AreaMod::Draw()
{
	ImGui::SetWindowFontScale(m_settings.common.menuFontSize);
	DrawMenuBar();

	ImGui::SetWindowFontScale(m_settings.common.contentFontSize);

	Widgets::AutoUpdateCheckBox(&m_settings.common.autoUpdate);
	if (!m_settings.common.autoUpdate)
		if (ImGui::Button("Update"))
			m_wantsUpdate = true;

	if (ImGui::TreeNodeEx("Draw Box", ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGuiExtras::InputVector3("Start point", &m_startBoxPoint);
		ImGui::SameLine();
		if (ImGui::Button("Current"))
			m_startBoxPoint = m_currentPos;

		ImGuiExtras::InputVector3("End point", &m_endBoxPoint);
		ImGui::SameLine();
		if (ImGui::Button("Current"))
			m_endBoxPoint = m_currentPos;

		if (ImGui::Button("Draw"))
			m_settings.drawBox = true;
		if (ImGui::Button("Erase"))
			m_settings.drawBox = false;
		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Angled Area", ImGuiTreeNodeFlags_SpanAvailWidth))
	{
		ImGuiExtras::InputVector3("Start point", &newAngledArea.m_angledAreaStartPoint);
		ImGui::SameLine();
		if (ImGui::Button("Current"))
			newAngledArea.m_angledAreaStartPoint = m_currentPos;

		ImGuiExtras::InputVector3("End point", &newAngledArea.m_angledAreaEndPoint);
		ImGui::SameLine();
		if (ImGui::Button("Current"))
			newAngledArea.m_angledAreaEndPoint = m_currentPos;

		ImGui::InputFloat("Width", &newAngledArea.m_angledAreaWidth);
		ImGui::Checkbox("Debug", &newAngledArea.m_angledAreaDebug);
		ImGui::Checkbox("Include Z", &newAngledArea.m_angledAreaIncludeZ);
		ImGui::InputInt("Unk0", &newAngledArea.m_angledAreaUnk0);
		if (ImGui::Button("Draw")) {
			m_settings.drawAngledArea = true;

			newAngledArea.red = rand() % 255;
			newAngledArea.blue = rand() % 255;
			newAngledArea.green = rand() % 255;

			m_angledAreas.push_back(newAngledArea);
		}

		if (ImGui::Button("Erase")) {
			m_settings.drawAngledArea = false;
			m_angledAreas.clear();
		}
		ImGui::TreePop();
	}

	ImGui::Separator();
	if (ImGui::TreeNodeEx("Sphere", ImGuiTreeNodeFlags_SpanAvailWidth))
	{
		ImGuiExtras::InputVector3("Center point", &m_centerPoint);
		ImGui::SameLine();
		if (ImGui::Button("Current"))
			m_centerPoint = m_currentPos;

		ImGui::InputFloat("Radius", &m_sphereRadius);
		
		if (ImGui::Button("Draw"))
			m_settings.drawSphere = true;
		if (ImGui::Button("Erase"))
			m_settings.drawSphere = false;

		ImGui::TreePop();
	}

	ImGui::Separator();
	if (ImGui::TreeNodeEx("Area", ImGuiTreeNodeFlags_SpanAvailWidth))
	{
		ImGui::Checkbox("Same as box", &m_areaSameBox);
		ImGuiExtras::InputVector3("Start point", &m_areaStartPoint);
		ImGui::SameLine();
		if (ImGui::Button("Current"))
			m_areaStartPoint = m_currentPos;

		ImGuiExtras::InputVector3("End point", &m_areaEndPoint);
		ImGui::SameLine();
		if (ImGui::Button("Current"))
			m_areaEndPoint = m_currentPos;

		ImGui::Checkbox("Unk", &m_areaUnk0);
		ImGui::Checkbox("Unk1", &m_areaUnk1);
		ImGui::InputInt("Unk2", &m_areaUnk2);
		ImGui::Separator();
		ImGui::Text("Is player in area: %s", BoolToStr(m_isInArea));
		ImGui::TreePop();
	}

	ImGui::Separator();
	if (ImGui::TreeNodeEx("Coord", ImGuiTreeNodeFlags_SpanAvailWidth))
	{
		ImGuiExtras::InputVector3("Coord point", &m_coordPoint);
		ImGui::SameLine();
		if (ImGui::Button("Current"))
			m_coordPoint = m_currentPos;

		ImGuiExtras::InputVector3("Size", &m_coordSize);

		ImGui::Checkbox("Unk", &m_coordUnk0);
		ImGui::Checkbox("Unk1", &m_coordUnk1);
		ImGui::InputInt("Unk2", &m_coordUnk2);
		ImGui::Separator();
		ImGui::Text("Is player at coord: %s", BoolToStr(m_isAtCoord));
		ImGui::TreePop();
	}

	ImGui::Separator();

	ImGui::Text("Import");
	if (ImGui::InputText("Paste here", m_importString, sizeof(m_importString),
		ImGuiInputTextFlags_EnterReturnsTrue))
	{
		std::string importStr = std::string(m_importString);

		std::vector<std::string> values;

		std::stringstream  ss(importStr);
		std::string str;
		while (getline(ss, str, ',')) {
			values.push_back(str);
		}

		if (values[0] == "Angled" && values.size() == 8) {
			newAngledArea.m_angledAreaStartPoint = Vector3();
			newAngledArea.m_angledAreaStartPoint.x = std::stof(values[1].c_str());
			newAngledArea.m_angledAreaStartPoint.y = std::stof(values[2].c_str());
			newAngledArea.m_angledAreaStartPoint.z = std::stof(values[3].c_str());
			newAngledArea.m_angledAreaEndPoint = Vector3();
			newAngledArea.m_angledAreaEndPoint.x = std::stof(values[4].c_str());
			newAngledArea.m_angledAreaEndPoint.y = std::stof(values[5].c_str());
			newAngledArea.m_angledAreaEndPoint.z = std::stof(values[6].c_str());
			newAngledArea.m_angledAreaWidth = std::stof(values[7].c_str());

			newAngledArea.red = rand() % 255;
			newAngledArea.blue = rand() % 255;
			newAngledArea.green = rand() % 255;

			m_settings.drawAngledArea = true;
			m_angledAreas.push_back(newAngledArea);
		}
		else if (values[0] == "Box" && values.size() == 7) {
			m_startBoxPoint = Vector3();
			m_startBoxPoint.x = std::stof(values[1].c_str());
			m_startBoxPoint.y = std::stof(values[2].c_str());
			m_startBoxPoint.z = std::stof(values[3].c_str());
			m_endBoxPoint = Vector3();
			m_endBoxPoint.x = std::stof(values[4].c_str());
			m_endBoxPoint.y = std::stof(values[5].c_str());
			m_endBoxPoint.z = std::stof(values[6].c_str());
			m_settings.drawBox = true;
		}
		else if (values[0] == "Sphere" && values.size() == 5) {
			m_centerPoint = Vector3();
			m_centerPoint.x = std::stof(values[1].c_str());
			m_centerPoint.y = std::stof(values[2].c_str());
			m_centerPoint.z = std::stof(values[3].c_str());
			m_sphereRadius = std::stof(values[4].c_str());
			m_settings.drawSphere = true;
		}

	}

	return true;
}