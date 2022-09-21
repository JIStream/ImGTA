/*
 * Copyright (c) 2021, Rayope
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#pragma once
#include "mod.h"
#include "utils.h"
#include "user_settings.h"

struct CharsheetSavedArray
{
	// Global_127127 -> Global_97353.f_29774
	int game_model; // Hash example 'ig_devin' -> Look in standard_global_reg for correspondance
	DWORD _padding0;
	int alpha_int;
	DWORD _padding1;
	int orig_alpha_int;
	DWORD _padding2;
	char char_label[32];
	char char_picture[32];
	int is_friend; // 11
	DWORD _padding11;
	int PhoneBookStatesize; // 12 // size 4?
	DWORD _padding12;
	PaddedInt PhoneBookStateArray[4];
	int bank_acc; // 17
	DWORD _padding17;
	int Picmsg_Status; // 18
	DWORD _padding18;
	int MissedCallStatusSize = 4; // 19
	DWORD _padding20;
	PaddedInt MissedCallStatusArray[4]; // 20
	int StatusAsCallerSize; // 25 // size 4?
	DWORD _padding25;
	PaddedInt StatusAsCallerArray[4];
}; // Size 29 * 8 bytes

struct MessageArray2 {
	// Global_97
	char f_0[32]; // 0
	char f_4[32]; // 3
	int f_8; // 8
	DWORD _padding8;
	int f_9; // 9
	DWORD _padding9;
}; // Size 10 * 8 bytes

struct MessageArray3 {
	// Global_2270
	char f_0[32];
	int f_4;
	DWORD _padding4;
	char scriptName[32];
	int scriptHash; // 9
	DWORD _padding9;
	int f_10;
	DWORD _padding10;
	int f_11;
	DWORD _padding11;
	int f_12;
	DWORD _padding12;
	int f_13;
	DWORD _padding13;
	int f_14;
	DWORD _padding14;
}; // Size 15 * 8 bytes

struct MessageArray4 {
	// Global_120988 -> Global_97353.f_23635.f_0
	char f_0[32]; // 0
	char f_4[32]; // 4
	int timer; // 8
	DWORD _padding8;
	int f_9;
	DWORD _padding9;
	int f_10;
	DWORD _padding10;
	int f_11;
	DWORD _padding11;
	int f_12;
	DWORD _padding12;
	int f_13;
	DWORD _padding13;
	int f_14;
	DWORD _padding14;
	int f_15;
	DWORD _padding15;
}; // Size 16 * 8 bytes

struct MessageArray5 {
	// Global_2873
	char f_0[40];
	int f_5; // 5
	DWORD _padding5;
}; // Size 6 * 8 bytes

struct MessageArray6 {
	// Global_1683749
	char f_0[32];
}; // Size 4 * 8 bytes

struct TimeAndDate {
	int seconds;
	DWORD _padding0;
	int minutes;
	DWORD _padding1;
	int hours;
	DWORD _padding2;
	int day;
	DWORD _padding3;
	int month;
	DWORD _padding4;
	int year;
	DWORD _padding5;
};

struct MessageArray7 {
	// Global_109748 -> Global_97353.f_12395
	char name[128]; // 0
	int notificationId; // 16
	DWORD _padding16;
	int f_17;
	DWORD _padding17;
	TimeAndDate timeAndDate; // 18
	int f_24;
	DWORD _padding24;
	int f_25;
	DWORD _padding25;
	int f_26;
	DWORD _padding26;
	int f_27;
	DWORD _padding27;
	int f_28;
	DWORD _padding28;
	int f_29;
	DWORD _padding29;
	int f_30;
	DWORD _padding30;
	int f_31;
	DWORD _padding31;
	int f_32;
	DWORD _padding32;
	char f_33[128]; // 33
	int f_49;
	DWORD _padding49;
	char f_50[128]; // 50
	int f_66;
	DWORD _padding66;
	char f_67[128]; // 67
	char f_83[128]; // 83
	int f_99_size = 4;
	DWORD _padding99;
	PaddedInt type[4]; // 99
}; // Size 104 * 8

struct CommunicationDataStruct {
	int ID;
	DWORD _padding0;
	int Settings;
	DWORD _padding1;
	int Player_Char_Bitset;
	DWORD _padding2;
	int Priority;
	DWORD _padding3;
	int Queue_Time;
	DWORD _padding4;
	int Requeue_Time;
	DWORD _padding5;
	int NPC_Character;
	DWORD _padding6;
	int Restricted_Area_ID;
	DWORD _padding7;
	int Execute_On_Complete_ID;
	DWORD _padding8;
	int Send_Check;
	DWORD _padding9;
};

struct TextMessageDataStruct {
	CommunicationDataStruct CommData;
	int ePart1;
	DWORD _padding10;
	int ePart2;
	DWORD _padding11;
	int Fail_Count;
	DWORD _padding12;
	int WhichCanCallSenderStatus;
	DWORD _padding13;
};

struct CallDataStruct {
	CommunicationDataStruct CommData;
	int eCommExtra;
	DWORD _padding10;
	int eCommExtra2;
	DWORD _padding11;
	int eYesResponse;
	DWORD _padding12;
	int eNoResponse;
	DWORD _padding13; 
	int Speaker_ID;
	DWORD _padding14;
};
//size: 832
struct SentTextMessageStruct {
	char GXTlabel[16]; // 0 1 2 3 ... 14 15
	DWORD _padding0[28]; // 16 20 24 28 32 36 40 44 48 52 56 60 64 68 72 76 80 84 88 92 96 100 104 108 112 116 120 124
	int FeedId_Int; //128
	DWORD _padding16; //132
	int Sender; //136
	DWORD _padding17; // 140
	int SentSecs; //144
	DWORD _padding18;//148
	int SentMins; //152
	DWORD _padding18_1;//156
	int SentHours; //160
	DWORD _padding18_2;//164
	int SentDay; //168
	DWORD _padding18_3;//172
	int SentMonth; //176
	DWORD _padding18_4;//180
	int SentYear; //184
	DWORD _padding18_5;//188

	PaddedInt _rest[77]; // 192 - 800
	DWORD _paddinglast[6]; // 808

};

struct EmailArrayBis2 {
	char f_0[32];
}; // Size 4 * 8 bytes

struct EmailArrayBis {
	int f_0;
	DWORD _padding0;
	int f_1; // Boolean values
	DWORD _padding1;
	char f_2[32];
	int f_6; // 6 // Next ID of f_7?
	DWORD _padding6;
	int f_7_size = 10;
	DWORD _padding7;
	EmailArrayBis2 f_7[10];
}; // Size 48 * 8 bytes

struct EmailArray {
	// Global_45154
	int f_0;
	int f_1;
	int f_2;
	int f_3 = 5; // Size of f_4 ?
	PaddedInt f_4[5];
	int f_9; // Size of f_10 ?
	int f_10_size = 4; // 4
	EmailArrayBis f_10[4];
}; // Size 203 * 8 bytes

// Global_97353.f_5944

class CommsMod : public Mod
{
public:
	CommsMod(DLLObject & dllObject, bool supportGlobals);

	bool Draw() override;
	void Think() override;
	void Load() override;
	void Unload() override;

private:
	void DrawMenuBar();
	void UpdateLocationData();
	CommonSettings & GetCommonSettings() override { return m_settings.common; }

	CommsSettings m_settings;
	GlobalArray<CharsheetSavedArray> m_gMessage1;
	GlobalArray<MessageArray2> m_gMessage2;
	GlobalArray<MessageArray3> m_gMessage3;
	GlobalArray<MessageArray4> m_gMessage4;
	GlobalArray<MessageArray5> m_gMessage5;
	GlobalArray<MessageArray6> m_gMessage6;
	GlobalArray<MessageArray7> m_gMessage7;
	GlobalArray<TextMessageDataStruct> m_gQueuedTexts;
	GlobalArray<CallDataStruct> m_gQueuedCalls;
	GlobalArray<CallDataStruct> m_gChatCalls;
	GlobalArray<CallDataStruct> m_gMissedCalls;
	GlobalArray<CommunicationDataStruct> m_gQueuedEmails;
	GlobalArray<SentTextMessageStruct> m_gTextMessagesSaved;

	GlobalArray<EmailArray> m_gEmail1;

	std::string m_unk15750 = "";
	std::string m_unk15756 = "";
	std::string m_unk15774 = "";
	std::string m_unk15780 = "";
	std::string m_unk15840 = "";
	int noQueuedTexts = 0;
	int noQueuedCalls = 0;
	int noChatCalls = 0;
	int noMissedCalls = 0;
	int noQueuedEmails = 0;

	int m_nextReceivingTime = 0;
	int m_timeLeftForReceiving = 0;
	int currentTime = 0;

	// ImGui inputs / internals
	bool m_wantsUpdate = false;
	bool m_constantUpdate = true;
	char m_animDictInput[256] = "";
	char m_animNameInput[256] = "";
	unsigned int m_animFlags = 0;
	bool m_animLoop = false;
};
