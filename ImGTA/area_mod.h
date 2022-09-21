/*
 * Copyright (c) 2021, Rayope
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once
#include "mod.h"
#include "user_settings.h"

#include "types.h"

#include<vector>

class AreaMod : public Mod
{
public:
	AreaMod(DLLObject & dllObject, bool supportGlobals) : Mod(dllObject, "Area", true, supportGlobals)
	{
		m_windowFlags = ImGuiWindowFlags_MenuBar;
	}

	bool Draw() override;
	void Think() override;
	void Load() override;
	void Unload() override;

private:
	void DrawMenuBar();
	CommonSettings & GetCommonSettings() override { return m_settings.common; }

	AreaSettings m_settings;
	Vector3 m_currentPos = {};

	// Box
	Vector3 m_startBoxPoint = {};
	Vector3 m_endBoxPoint = {};

	// Angled area
	struct AngledArea {
		bool m_isInAngledArea = false;
		Vector3 m_angledAreaStartPoint = {};
		Vector3 m_angledAreaEndPoint = {};
		float m_angledAreaWidth = 10;
		bool m_angledAreaDebug = false;
		bool m_angledAreaIncludeZ = true;
		int m_angledAreaUnk0 = 0;

		int red;
		int green;
		int blue;
	};

	std::vector<AngledArea> m_angledAreas;
	AngledArea newAngledArea;


	//Sphere
	Vector3 m_centerPoint = {};
	float m_sphereRadius = {};

	// Area
	bool m_areaSameBox = false;
	bool m_isInArea = false;
	Vector3 m_areaStartPoint = {};
	Vector3 m_areaEndPoint = {};
	bool m_areaUnk0 = false;
	bool m_areaUnk1 = false;
	int m_areaUnk2 = 0;

	// Coord
	bool m_isAtCoord = false;
	Vector3 m_coordPoint = {};
	Vector3 m_coordSize = {};
	bool m_coordUnk0 = false;
	bool m_coordUnk1 = true;
	int m_coordUnk2 = 0;

	// ImGui inputs / internals
	bool m_wantsUpdate = false;
	int m_handleInput = 0;
	char m_animDictInput[256] = "";
	char m_animNameInput[256] = "";
	unsigned int m_animFlags = 0;
	bool m_animLoop = false;

	// Share
	char m_importString[512] = "";
};

