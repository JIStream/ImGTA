/*
 * Copyright (c) 2021, Rayope
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include "comms_mod.h"

#include "script.h"
#include "watch_entry.h"
#include "mission_mod.h"
#include "global_id.h"

#include "natives.h"

#include "imgui.h"
#include "imgui_extras.h"

#include <cmath>


char* GetCharactersFromBitset(int bitset) {
	static char result[8][4] = { "   ", "M  ", "F  ", "MF ", "T  ", "TM ", "TF ", "TMF"};

	return result[bitset];
}

CommsMod::CommsMod(DLLObject & dllObject, bool supportGlobals) :
	Mod(dllObject, "Comms", true, supportGlobals),
	m_gMessage1(GlobalID::_127127),
	m_gMessage2(GlobalID::_97),
	m_gMessage3(GlobalID::_2270),
	m_gMessage4(GlobalID::_120988),
	m_gMessage5(GlobalID::_2873),
	m_gMessage6(GlobalID::_1683749),
	m_gMessage7(GlobalID::_109748),
	m_gQueuedTexts(GlobalID::_103948), //Global_97353.f_5944.f_651
	m_gQueuedCalls(GlobalID::_103927), //Global_97353.f_5944
	m_gQueuedEmails(GlobalID::_104062), //Global_97353.f_5944.f_765
	m_gEmail1(GlobalID::_45154),
	m_gTextMessagesSaved(109748)
{
	m_windowFlags = ImGuiWindowFlags_MenuBar;
}

void CommsMod::Load()
{
	m_settings = m_dllObject.GetUserSettings().comms;
}

void CommsMod::Unload()
{
	m_dllObject.GetUserSettings().comms = m_settings;
}

void CommsMod::Think()
{
	if (m_constantUpdate || m_wantsUpdate)
	{
		UpdateLocationData();
		m_wantsUpdate = false;
	}
}

void CommsMod::UpdateLocationData()
{
	if (m_supportGlobals)
	{
		m_gMessage1.LoadElement();
		/*m_gMessage2.LoadElement();
		m_gMessage3.LoadElement();
		m_gMessage4.LoadElement();
		m_gMessage5.LoadElement();
		m_gMessage6.LoadElement();
		m_gMessage7.LoadElement();*/
		m_gQueuedTexts.LoadElement();
		m_gQueuedCalls.LoadElement();
		m_gQueuedEmails.LoadElement();
		m_gTextMessagesSaved.LoadElement();
		/*m_gEmail1.LoadElement(); */

		noQueuedTexts = *(int*)GetGlobalPtr(104061);
		noQueuedCalls = *(int*)GetGlobalPtr(103947);
		noQueuedEmails = *(int*)GetGlobalPtr(104163);


		currentTime = MISC::GET_GAME_TIMER();

		m_nextReceivingTime = *(int *)GetGlobalPtr(GlobalID::_35464);
		int tmp =  (m_nextReceivingTime - currentTime) / 1000;
		m_timeLeftForReceiving = tmp >= 0 ? tmp : 0;
	}
}

void CommsMod::DrawMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			ImGui::MenuItem("Empty");
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

bool CommsMod::Draw()
{
	ImGui::SetWindowFontScale(m_settings.common.menuFontSize);
	DrawMenuBar();

	ImGui::SetWindowFontScale(m_settings.common.contentFontSize);

	ImGui::Checkbox("Constant Updates?", &m_constantUpdate);
	if (!m_constantUpdate)
		if (ImGui::Button("Update"))
			m_wantsUpdate = true;

	ImGui::Separator();
	ImGui::Text("Next time comms can be received: %d", m_nextReceivingTime);
	ImGui::Text("Next comm in %d s", m_timeLeftForReceiving);
	/*ImGui::Text("Unk15750: %s", m_unk15750.c_str());
	ImGui::Text("Unk15756: %s", m_unk15756.c_str());
	ImGui::Text("Unk15774: %s", m_unk15774.c_str());
	ImGui::Text("Unk15780: %s", m_unk15780.c_str());
	ImGui::Text("Unk15840: %s", m_unk15840.c_str());*/

	if (m_supportGlobals)
	{
		ImGui::Separator();
		

		//ImGui::Separator();
		//if (ImGui::TreeNodeEx("Call numbers?", ImGuiTreeNodeFlags_SpanAvailWidth))
		//{
		//	if (m_gMessage1.id < m_gMessage2.size)
		//		m_gMessage2.id = m_gMessage1.id;
		//	ImGui::Text("Same ID as mission ID: %d", m_gMessage2.id);
		//	ImGui::SameLine();
		//	ImGui::Text("(max: %d)", m_gMessage2.size);
		//	ImGui::Text("Field 0: %s", m_gMessage2.arr.f_0);
		//	ImGui::Text("Field 4: %s", m_gMessage2.arr.f_4);
		//	ImGui::Text("Field 8: %d", m_gMessage2.arr.f_8);
		//	ImGui::Text("Field 9: %d", m_gMessage2.arr.f_9);
		//	ImGui::TreePop();
		//}

	/*	ImGui::Separator();
		if (ImGui::TreeNodeEx("Message Array 3", ImGuiTreeNodeFlags_SpanAvailWidth))
		{
			ImGui::SetNextItemWidth(m_inputIDWidgetWidth);
			if (ImGui::InputInt("Array 3 ID", &m_gMessage3.id))
			{
				ClipInt(m_gMessage3.id, 0, m_gMessage3.size - 1);
				m_wantsUpdate = true;
			}
			ImGui::SameLine();
			ImGui::Text("(max: %d)", m_gMessage3.size);

			ImGui::Text("Field 0: %s", m_gMessage3.arr.f_0);
			ImGui::Text("Field 4: %d", m_gMessage3.arr.f_4);
			ImGui::Text("Script name: %s", m_gMessage3.arr.scriptName);
			ImGui::Text("Script hash: %d", m_gMessage3.arr.scriptHash);
			ImGui::Text("Field 10: %d", m_gMessage3.arr.f_10);
			ImGui::Text("Field 11: %d", m_gMessage3.arr.f_11);
			ImGui::Text("Field 12: %d", m_gMessage3.arr.f_12);
			ImGui::Text("Field 13: %d", m_gMessage3.arr.f_13);
			ImGui::Text("Field 14: %d", m_gMessage3.arr.f_14);
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNodeEx("Message Array 4", ImGuiTreeNodeFlags_SpanAvailWidth))
		{
			ImGui::SetNextItemWidth(m_inputIDWidgetWidth);
			if (ImGui::InputInt("Array 4 ID", &m_gMessage4.id))
			{
				ClipInt(m_gMessage4.id, 0, m_gMessage4.size - 1);
				m_wantsUpdate = true;
			}
			ImGui::SameLine();
			ImGui::Text("(max: %d)", m_gMessage4.size);

			ImGui::Text("Field 0: %s", m_gMessage4.arr.f_0);
			ImGui::Text("Field 4: %s", m_gMessage4.arr.f_4);
			ImGui::Text("Timer: %d", m_gMessage4.arr.timer);
			ImGui::Text("Field 9: %d", m_gMessage4.arr.f_9);
			ImGui::Text("Field 10: %d", m_gMessage4.arr.f_10);
			ImGui::Text("Field 11: %d", m_gMessage4.arr.f_11);
			ImGui::Text("Field 12: %d", m_gMessage4.arr.f_12);
			ImGui::Text("Field 13: %d", m_gMessage4.arr.f_13);
			ImGui::Text("Field 14: %d", m_gMessage4.arr.f_14);
			ImGui::Text("Field 15: %d", m_gMessage4.arr.f_15);
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNodeEx("Message Array 5", ImGuiTreeNodeFlags_SpanAvailWidth))
		{
			ImGui::SetNextItemWidth(m_inputIDWidgetWidth);
			if (ImGui::InputInt("Array 5 ID", &m_gMessage5.id))
			{
				ClipInt(m_gMessage5.id, 0, m_gMessage5.size - 1);
				m_wantsUpdate = true;
			}
			ImGui::SameLine();
			ImGui::Text("(max: %d)", m_gMessage5.size);

			ImGui::Text("Field 0: %s", m_gMessage5.arr.f_0);
			ImGui::Text("Field 5: %d", m_gMessage5.arr.f_5);
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNodeEx("Message Array 6", ImGuiTreeNodeFlags_SpanAvailWidth))
		{
			ImGui::SetNextItemWidth(m_inputIDWidgetWidth);
			if (ImGui::InputInt("Array 6 ID", &m_gMessage6.id))
			{
				ClipInt(m_gMessage6.id, 0, m_gMessage6.size - 1);
				m_wantsUpdate = true;
			}
			ImGui::SameLine();
			ImGui::Text("(max: %d)", m_gMessage6.size);

			ImGui::Text("Field 0: %s", m_gMessage6.arr.f_0);
			ImGui::TreePop();
		}*/


		ImGui::Separator();
		if (ImGui::TreeNodeEx("Queued texts", ImGuiTreeNodeFlags_SpanAvailWidth))
		{

			ImGui::SetNextItemWidth(m_inputIDWidgetWidth);
			ImGui::Text("(Count: %d)", noQueuedTexts);

			if (noQueuedTexts > 0) {
				if (ImGui::InputInt("Queued Text ID", &m_gQueuedTexts.id))
				{
					ClipInt(m_gQueuedTexts.id, 0, noQueuedTexts - 1);
					m_wantsUpdate = true;
				}
			
				ImGui::Text("ID: %d (Name %s)", m_gQueuedTexts.arr.CommData.ID, GetTextMessageName(m_gQueuedTexts.arr.CommData.ID));
				ImGui::Text("Settings: %d", m_gQueuedTexts.arr.CommData.Settings);
				ImGui::Text("For Characters: %s", GetCharactersFromBitset(m_gQueuedTexts.arr.CommData.Player_Char_Bitset));
				ImGui::Text("Priority %d", m_gQueuedTexts.arr.CommData.Priority);

				if (m_gQueuedTexts.arr.CommData.Queue_Time > currentTime) {

					ImGui::Text("Queue_Time: In %d s (%d)", (m_gQueuedTexts.arr.CommData.Queue_Time - currentTime) / 1000, m_gQueuedTexts.arr.CommData.Queue_Time);
					if (ImGui::Button("Force ready")) {
						m_gQueuedTexts.ptr->CommData.Queue_Time = currentTime + 16;
					}
				}
				else
					ImGui::Text("Queue_Time: Ready to send");

				ImGui::Text("Requeue_Time: %d s", m_gQueuedTexts.arr.CommData.Requeue_Time);
				ImGui::Text("NPC_Character: %s", GetCommsCharacterName(m_gQueuedTexts.arr.CommData.NPC_Character));
				ImGui::Text("Restricted_Area_ID: %d", m_gQueuedTexts.arr.CommData.Restricted_Area_ID);
				ImGui::Text("Execute_On_Complete_ID: %d", m_gQueuedTexts.arr.CommData.Execute_On_Complete_ID);
				ImGui::Text("Send_Check: %d", m_gQueuedTexts.arr.CommData.Send_Check);
				ImGui::Text("ePart1: %d", m_gQueuedTexts.arr.ePart1);
				ImGui::Text("ePart2: %d", m_gQueuedTexts.arr.ePart2);
				ImGui::Text("Fail_Count: %d", m_gQueuedTexts.arr.Fail_Count);
				ImGui::Text("WhichCanCallSenderStatus: %d", m_gQueuedTexts.arr.WhichCanCallSenderStatus);
			}
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNodeEx("Queued calls", ImGuiTreeNodeFlags_SpanAvailWidth))
		{

			ImGui::SetNextItemWidth(m_inputIDWidgetWidth);
			ImGui::Text("(Count: %d)", noQueuedCalls);
			if(noQueuedCalls > 0) {
				if (ImGui::InputInt("Queued Call ID", &m_gQueuedCalls.id))
				{
					ClipInt(m_gQueuedCalls.id, 0, noQueuedCalls - 1);
					m_wantsUpdate = true;
				}
			
				ImGui::Text("ID: %d (Name %s)", m_gQueuedCalls.arr.CommData.ID, GetCallName(m_gQueuedCalls.arr.CommData.ID));
				ImGui::Text("Settings: %d", m_gQueuedCalls.arr.CommData.Settings);
				ImGui::Text("For Characters: %s", GetCharactersFromBitset(m_gQueuedCalls.arr.CommData.Player_Char_Bitset));

				if (m_gQueuedCalls.arr.CommData.Queue_Time > currentTime)
					ImGui::Text("Queue_Time: In %d s (%d)", (m_gQueuedCalls.arr.CommData.Queue_Time - currentTime) / 1000, m_gQueuedCalls.arr.CommData.Queue_Time);
				else
					ImGui::Text("Queue_Time: Ready to send");				

				ImGui::Text("Requeue_Time: %d s", m_gQueuedCalls.arr.CommData.Requeue_Time);
				ImGui::Text("NPC_Character: %s", GetCommsCharacterName(m_gQueuedCalls.arr.CommData.NPC_Character));
				ImGui::Text("Restricted_Area_ID: %d", m_gQueuedCalls.arr.CommData.Restricted_Area_ID);
				ImGui::Text("Execute_On_Complete_ID: %d", m_gQueuedCalls.arr.CommData.Execute_On_Complete_ID);
				ImGui::Text("Send_Check: %d", m_gQueuedCalls.arr.CommData.Send_Check);
			}
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNodeEx("Queued emails", ImGuiTreeNodeFlags_SpanAvailWidth))
		{
			
			ImGui::SetNextItemWidth(m_inputIDWidgetWidth);
			ImGui::Text("(Count: %d)", noQueuedEmails);
			if (noQueuedEmails > 0) {
				if (ImGui::InputInt("Queued Email ID", &m_gQueuedEmails.id))
				{
					ClipInt(m_gQueuedEmails.id, 0, noQueuedEmails - 1);
					m_wantsUpdate = true;
				}
			
				ImGui::Text("ID: %d", m_gQueuedEmails.arr.ID);
				ImGui::Text("Settings: %d", m_gQueuedEmails.arr.Settings);
				ImGui::Text("For Characters: %s", GetCharactersFromBitset(m_gQueuedEmails.arr.Player_Char_Bitset));

				if (m_gQueuedEmails.arr.Queue_Time > currentTime)
					ImGui::Text("Queue_Time: In %d s (%d)", (m_gQueuedEmails.arr.Queue_Time - currentTime) / 1000, m_gQueuedEmails.arr.Queue_Time);
				else
					ImGui::Text("Queue_Time: Ready to send");

				ImGui::Text("Requeue_Time: %d", m_gQueuedEmails.arr.Requeue_Time);
				ImGui::Text("NPC_Character: %s", GetCommsCharacterName(m_gQueuedEmails.arr.NPC_Character));
				ImGui::Text("Restricted_Area_ID: %d", m_gQueuedEmails.arr.Restricted_Area_ID);
				ImGui::Text("Execute_On_Complete_ID: %d", m_gQueuedEmails.arr.Execute_On_Complete_ID);
				ImGui::Text("Send_Check: %d", m_gQueuedEmails.arr.Send_Check);
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Charsheet Saved Contacts", ImGuiTreeNodeFlags_SpanAvailWidth))
		{
			ImGui::SetNextItemWidth(m_inputIDWidgetWidth);
			if (ImGui::InputInt("Message ID", &m_gMessage1.id))
			{
				ClipInt(m_gMessage1.id, 0, m_gMessage1.size - 1);
				m_wantsUpdate = true;
			}
			ImGui::SameLine();
			ImGui::Text("(max: %d)", m_gMessage1.size);

			ImGui::Text("Game Model Hash: %d", m_gMessage1.arr.game_model);
			ImGui::Text("Alpha: %d", m_gMessage1.arr.alpha_int);
			ImGui::Text("Original Alpha: %d", m_gMessage1.arr.orig_alpha_int);
			ImGui::Text("Character Label: %s", m_gMessage1.arr.char_label);
			ImGui::Text("Character Picture: %s", m_gMessage1.arr.char_picture);
			ImGui::Text("Is Friend?: %d", m_gMessage1.arr.is_friend);
			/*ImGui::Text("Field C: %d", m_gMessage1.arr.field_12_size);
			std::string field_C = "Field C: ";
			for (int i = 0; i < m_gMessage1.arr.field_12_size; ++i)
				field_C += std::to_string(m_gMessage1.arr.field_12[i].val) + ", ";
			ImGui::Text(field_C.c_str());
			ImGui::Text("Field 11: %d", m_gMessage1.arr.field_17);
			ImGui::Text("Field 12: %d", m_gMessage1.arr.field_18);
			ImGui::Text("Field 13: %d", m_gMessage1.arr.field_20_size);
			std::string field_13 = "Field 13: ";
			for (int i = 0; i < m_gMessage1.arr.field_20_size; ++i)
				field_13 += std::to_string(m_gMessage1.arr.field_20[i].val) + ", ";
			ImGui::Text(field_13.c_str());
			ImGui::Text("Field 18: %d", m_gMessage1.arr.field_25_size);
			std::string field_18 = "Field 18: ";
			for (int i = 0; i < m_gMessage1.arr.field_25_size; ++i)
				field_18 += std::to_string(m_gMessage1.arr.field_25[i].val) + ", ";
			ImGui::Text(field_18.c_str());*/
			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Saved Texts", ImGuiTreeNodeFlags_SpanAvailWidth))
		{
			ImGui::SetNextItemWidth(m_inputIDWidgetWidth);
			if (ImGui::InputInt("Message ID", &m_gTextMessagesSaved.id))
			{
				ClipInt(m_gTextMessagesSaved.id, 0, m_gMessage1.size - 1);
				m_wantsUpdate = true;
			}
			ImGui::SameLine();
			ImGui::Text("(max: %d)", m_gTextMessagesSaved.size);

			ImGui::Text("Text Label: %s", m_gTextMessagesSaved.arr.GXTlabel);
			ImGui::Text("Sender: %s", GetCommsCharacterName(m_gTextMessagesSaved.arr.Sender));
			ImGui::Text("Sent at %d:%d:%d on %d/%d/%d ", m_gTextMessagesSaved.arr.SentHours, m_gTextMessagesSaved.arr.SentMins, m_gTextMessagesSaved.arr.SentSecs, m_gTextMessagesSaved.arr.SentDay, m_gTextMessagesSaved.arr.SentMonth, m_gTextMessagesSaved.arr.SentYear);
		
			ImGui::TreePop();
		}
		/*
		ImGui::Separator();
		if (ImGui::TreeNodeEx("Email array", ImGuiTreeNodeFlags_SpanAvailWidth))
		{
			ImGui::SetNextItemWidth(m_inputIDWidgetWidth);
			if (ImGui::InputInt("Email ID", &m_gEmail1.id))
			{
				ClipInt(m_gEmail1.id, 0, m_gEmail1.size - 1);
				m_wantsUpdate = true;
			}
			ImGui::SameLine();
			ImGui::Text("(max: %d)", m_gEmail1.size);

			ImGui::Text("Field 0: %d", m_gEmail1.arr.f_0);
			ImGui::Text("Field 1: %d", m_gEmail1.arr.f_1);
			ImGui::Text("Field 2: %d", m_gEmail1.arr.f_2);
			ImGui::Text("Field 3: %d", m_gEmail1.arr.f_3);
			std::string field_4 = "Field 4: ";
			for (int i = 0; i < m_gEmail1.arr.f_3; ++i)
				field_4 += std::to_string(m_gEmail1.arr.f_4[i].val) + ", ";
			ImGui::Text(field_4.c_str());
			ImGui::Text("Field 9: %d", m_gEmail1.arr.f_9);
			ImGui::Text("Field 10 size: %d", m_gEmail1.arr.f_10_size);
			for (int i = 0; i < m_gEmail1.arr.f_10_size; ++i)
			{
				ImGui::Text("Field 0 of field 10: %d", m_gEmail1.arr.f_10[i].f_0);
				ImGui::Text("Field 1 of field 10: %d", m_gEmail1.arr.f_10[i].f_1);
				ImGui::Text("Field 2 of field 10: %s", m_gEmail1.arr.f_10[i].f_2);
				ImGui::Text("Field 6 of field 10: %d", m_gEmail1.arr.f_10[i].f_6);
				ImGui::Text("Field 7 size of field 10: %d", m_gEmail1.arr.f_10[i].f_7_size);
				for (int j = 0; j < m_gEmail1.arr.f_10[i].f_7_size; ++j)
					ImGui::Text("Field 0 of field 7 of field 10: %s", m_gEmail1.arr.f_10[i].f_7[j].f_0);
			}
			ImGui::TreePop();
		}*/
	}
	return true;
}

