/******************************************************************************/
/*
\file       MiniGameLogic.cpp
\author     Tan Yan Kai, yankai.tan, 2301312 (100%)
\par        yankai.tan@digipen.edu
\date       April 6 2025

\brief      Implements the MiniGameLogic class for handling all minigame-related
            logic such as keypad password entry and the Simon Says color
            sequence challenge.

            This logic enables toggling visibility, validating input, updating
            UI text, and interacting with button and sound components for each
            minigame phase. Also manages game states and feedback upon success
            or failure.

Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


//#include "GameLogic.h"
//#include "RestockLogic.h"
//#include "MiniGameLogic.h"
//
//void MiniGameLogic::Start()
//{
//	gameMiniGame_Text = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_Text").get();
//	if (gameMiniGame_Text)
//	{
//		gameMiniGame_Text_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_Text->entityID);
//		if (gameMiniGame_Text_Txt)
//		{
//			gameMiniGame_Text_Txt->isEnabled = false;
//		}
//	}
//
//	gameMiniGame_BG = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_BG").get();
//	if (gameMiniGame_BG)
//	{
//		gameMiniGame_BG_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_BG->entityID);
//		if (gameMiniGame_BG_Spt)
//		{
//			gameMiniGame_BG_Spt->isVisible = false;
//		}
//	}
//
//	gameMiniGame_Keypad = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_Keypad").get();
//	if (gameMiniGame_Keypad)
//	{
//		gameMiniGame_Keypad_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_Keypad->entityID);
//		if (gameMiniGame_Keypad_Spt)
//		{
//			gameMiniGame_Keypad_Spt->isVisible = false;
//		}
//	}
//
//	gameMiniGame_K1 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K1").get();
//	if (gameMiniGame_K1)
//	{
//		gameMiniGame_K1_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K1->entityID);
//		if (gameMiniGame_K1_Spt)
//		{
//			gameMiniGame_K1_Spt->isVisible = false;
//		}
//		gameMiniGame_K1_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K1->entityID);
//		if (gameMiniGame_K1_Btn)
//		{
//			auto gameMiniGame_K1_Sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameMiniGame_K1->entityID);
//			gameMiniGame_K1_Btn->onClick = [this, gameMiniGame_K1_Sound]() {
//				gameMiniGame_K1_Sound->Play(-1);
//				passwordInput("1"); };
//		}
//	}
//
//	gameMiniGame_T1 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T1").get();
//	if (gameMiniGame_T1)
//	{
//		gameMiniGame_T1_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T1->entityID);
//		if (gameMiniGame_T1_Txt)
//		{
//			gameMiniGame_T1_Txt->isEnabled = false;
//		}
//	}
//
//	gameMiniGame_K2 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K2").get();
//	if (gameMiniGame_K2)
//	{
//		gameMiniGame_K2_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K2->entityID);
//		if (gameMiniGame_K2_Spt)
//		{
//			gameMiniGame_K2_Spt->isVisible = false;
//		}
//		gameMiniGame_K2_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K2->entityID);
//		if (gameMiniGame_K2_Btn)
//		{
//			auto gameMiniGame_K2_Sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameMiniGame_K2->entityID);
//			gameMiniGame_K2_Btn->onClick = [this, gameMiniGame_K2_Sound]() {
//				gameMiniGame_K2_Sound->Play(-1);
//				passwordInput("2"); };
//		}
//	}
//
//	gameMiniGame_T2 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T2").get();
//	if (gameMiniGame_T2)
//	{
//		gameMiniGame_T2_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T2->entityID);
//		if (gameMiniGame_T2_Txt)
//		{
//			gameMiniGame_T2_Txt->isEnabled = false;
//		}
//	}
//
//	gameMiniGame_K3 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K3").get();
//	if (gameMiniGame_K3)
//	{
//		gameMiniGame_K3_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K3->entityID);
//		if (gameMiniGame_K3_Spt)
//		{
//			gameMiniGame_K3_Spt->isVisible = false;
//		}
//		gameMiniGame_K3_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K3->entityID);
//		if (gameMiniGame_K3_Btn)
//		{
//			auto gameMiniGame_K3_Sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameMiniGame_K3->entityID);
//			gameMiniGame_K3_Btn->onClick = [this, gameMiniGame_K3_Sound]() {
//				gameMiniGame_K3_Sound->Play(-1);
//				passwordInput("3"); };
//		}
//	}
//
//	gameMiniGame_T3 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T3").get();
//	if (gameMiniGame_T3)
//	{
//		gameMiniGame_T3_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T3->entityID);
//		if (gameMiniGame_T3_Txt)
//		{
//			gameMiniGame_T3_Txt->isEnabled = false;
//		}
//	}
//
//	gameMiniGame_K4 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K4").get();
//	if (gameMiniGame_K4)
//	{
//		gameMiniGame_K4_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K4->entityID);
//		if (gameMiniGame_K4_Spt)
//		{
//			gameMiniGame_K4_Spt->isVisible = false;
//		}
//		gameMiniGame_K4_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K4->entityID);
//		if (gameMiniGame_K4_Btn)
//		{
//			auto gameMiniGame_K4_Sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameMiniGame_K4->entityID);
//			gameMiniGame_K4_Btn->onClick = [this, gameMiniGame_K4_Sound]() {
//				gameMiniGame_K4_Sound->Play(-1);
//				passwordInput("4"); };
//		}
//	}
//
//	gameMiniGame_T4 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T4").get();
//	if (gameMiniGame_T4)
//	{
//		gameMiniGame_T4_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T4->entityID);
//		if (gameMiniGame_T4_Txt)
//		{
//			gameMiniGame_T4_Txt->isEnabled = false;
//		}
//	}
//
//	gameMiniGame_K5 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K5").get();
//	if (gameMiniGame_K5)
//	{
//		gameMiniGame_K5_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K5->entityID);
//		if (gameMiniGame_K5_Spt)
//		{
//			gameMiniGame_K5_Spt->isVisible = false;
//		}
//		gameMiniGame_K5_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K5->entityID);
//		if (gameMiniGame_K5_Btn)
//		{
//			auto gameMiniGame_K5_Sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameMiniGame_K5->entityID);
//			gameMiniGame_K5_Btn->onClick = [this, gameMiniGame_K5_Sound]() {
//				gameMiniGame_K5_Sound->Play(-1);
//				passwordInput("5"); };
//		}
//	}
//
//	gameMiniGame_T5 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T5").get();
//	if (gameMiniGame_T5)
//	{
//		gameMiniGame_T5_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T5->entityID);
//		if (gameMiniGame_T5_Txt)
//		{
//			gameMiniGame_T5_Txt->isEnabled = false;
//		}
//	}
//
//	gameMiniGame_K6 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K6").get();
//	if (gameMiniGame_K6)
//	{
//		gameMiniGame_K6_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K6->entityID);
//		if (gameMiniGame_K6_Spt)
//		{
//			gameMiniGame_K6_Spt->isVisible = false;
//		}
//		gameMiniGame_K6_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K6->entityID);
//		if (gameMiniGame_K6_Btn)
//		{
//			auto gameMiniGame_K6_Sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameMiniGame_K6->entityID);
//			gameMiniGame_K6_Btn->onClick = [this, gameMiniGame_K6_Sound]() {
//				gameMiniGame_K6_Sound->Play(-1);
//				passwordInput("6"); };
//		}
//	}
//
//	gameMiniGame_T6 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T6").get();
//	if (gameMiniGame_T6)
//	{
//		gameMiniGame_T6_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T6->entityID);
//		if (gameMiniGame_T6_Txt)
//		{
//			gameMiniGame_T6_Txt->isEnabled = false;
//		}
//	}
//
//	gameMiniGame_K7 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K7").get();
//	if (gameMiniGame_K7)
//	{
//		gameMiniGame_K7_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K7->entityID);
//		if (gameMiniGame_K7_Spt)
//		{
//			gameMiniGame_K7_Spt->isVisible = false;
//		}
//		gameMiniGame_K7_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K7->entityID);
//		if (gameMiniGame_K7_Btn)
//		{
//			auto gameMiniGame_K7_Sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameMiniGame_K7->entityID);
//			gameMiniGame_K7_Btn->onClick = [this, gameMiniGame_K7_Sound]() {
//				gameMiniGame_K7_Sound->Play(-1);
//				passwordInput("7"); };
//		}
//	}
//
//	gameMiniGame_T7 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T7").get();
//	if (gameMiniGame_T7)
//	{
//		gameMiniGame_T7_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T7->entityID);
//		if (gameMiniGame_T7_Txt)
//		{
//			gameMiniGame_T7_Txt->isEnabled = false;
//		}
//	}
//
//	gameMiniGame_K8 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K8").get();
//	if (gameMiniGame_K8)
//	{
//		gameMiniGame_K8_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K8->entityID);
//		if (gameMiniGame_K8_Spt)
//		{
//			gameMiniGame_K8_Spt->isVisible = false;
//		}
//		gameMiniGame_K8_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K8->entityID);
//		if (gameMiniGame_K8_Btn)
//		{
//			auto gameMiniGame_K8_Sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameMiniGame_K8->entityID);
//			gameMiniGame_K8_Btn->onClick = [this, gameMiniGame_K8_Sound]() {
//				gameMiniGame_K8_Sound->Play(-1);
//				passwordInput("8"); };
//		}
//	}
//
//	gameMiniGame_T8 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T8").get();
//	if (gameMiniGame_T8)
//	{
//		gameMiniGame_T8_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T8->entityID);
//		if (gameMiniGame_T8_Txt)
//		{
//			gameMiniGame_T8_Txt->isEnabled = false;
//		}
//	}
//
//	gameMiniGame_K9 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K9").get();
//	if (gameMiniGame_K9)
//	{
//		gameMiniGame_K9_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K9->entityID);
//		if (gameMiniGame_K9_Spt)
//		{
//			gameMiniGame_K9_Spt->isVisible = false;
//		}
//		gameMiniGame_K9_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K9->entityID);
//		if (gameMiniGame_K9_Btn)
//		{
//			auto gameMiniGame_K9_Sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameMiniGame_K9->entityID);
//			gameMiniGame_K9_Btn->onClick = [this, gameMiniGame_K9_Sound]() {
//				gameMiniGame_K9_Sound->Play(-1);
//				passwordInput("9"); };
//		}
//	}
//
//	gameMiniGame_T9 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T9").get();
//	if (gameMiniGame_T9)
//	{
//		gameMiniGame_T9_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T9->entityID);
//		if (gameMiniGame_T9_Txt)
//		{
//			gameMiniGame_T9_Txt->isEnabled = false;
//		}
//	}
//
//	gameMiniGame_K0 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K0").get();
//	if (gameMiniGame_K0)
//	{
//		gameMiniGame_K0_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K0->entityID);
//		if (gameMiniGame_K0_Spt)
//		{
//			gameMiniGame_K0_Spt->isVisible = false;
//		}
//		gameMiniGame_K0_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K0->entityID);
//		if (gameMiniGame_K0_Btn)
//		{
//			auto gameMiniGame_K0_Sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameMiniGame_K0->entityID);
//			gameMiniGame_K0_Btn->onClick = [this, gameMiniGame_K0_Sound]() {
//				gameMiniGame_K0_Sound->Play(-1);
//				passwordInput("0"); };
//		}
//	}
//
//	gameMiniGame_T0 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T10").get();
//	if (gameMiniGame_T0)
//	{
//		gameMiniGame_T0_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T0->entityID);
//		if (gameMiniGame_T0_Txt)
//		{
//			gameMiniGame_T0_Txt->isEnabled = false;
//		}
//	}
//
//	gameMiniGame_Enter = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_Enter").get();
//	if (gameMiniGame_Enter)
//	{
//		gameMiniGame_Enter_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_Enter->entityID);
//		if (gameMiniGame_Enter_Spt)
//		{
//			gameMiniGame_Enter_Spt->isVisible = false;
//		}
//		gameMiniGame_Enter_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_Enter->entityID);
//		if (gameMiniGame_Enter_Btn)
//		{
//			gameMiniGame_Enter_BtnSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameMiniGame_Enter->entityID);
//			gameMiniGame_Enter_Btn->onClick = [this]() { 
//				gameMiniGame_Enter_BtnSound->Play();
//				enterPassword(); };
//		}
//	}
//
//	gameMiniGame_TextEnter = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_TextEnter").get();
//	if (gameMiniGame_TextEnter)
//	{
//		gameMiniGame_TextEnter_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_TextEnter->entityID);
//		if (gameMiniGame_TextEnter_Txt)
//		{
//			gameMiniGame_TextEnter_Txt->isEnabled = false;
//		}
//	}
//
//	gameMiniGame_Delete = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_Delete").get();
//	if (gameMiniGame_Delete)
//	{
//		gameMiniGame_Delete_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_Delete->entityID);
//		if (gameMiniGame_Delete_Spt)
//		{
//			gameMiniGame_Delete_Spt->isVisible = false;
//		}
//		gameMiniGame_Delete_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_Delete->entityID);
//		if (gameMiniGame_Delete_Btn)
//		{
//			auto gameMiniGame_Delete_BtnSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(gameMiniGame_Delete->entityID);
//			gameMiniGame_Delete_Btn->onClick = [this, gameMiniGame_Delete_BtnSound]() { 
//				gameMiniGame_Delete_BtnSound->Play(-1);
//				deletePassword(); };
//		}
//	}
//
//	gameMiniGame_TextDelete = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_TextDelete").get();
//	if (gameMiniGame_TextDelete)
//	{
//		gameMiniGame_TextDelete_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_TextDelete->entityID);
//		if (gameMiniGame_TextDelete_Txt)
//		{
//			gameMiniGame_TextDelete_Txt->isEnabled = false;
//		}
//	}
//
//	gameMiniGame_Password = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_Password").get();
//	gameMiniGame_Password_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_Password->entityID);
//	if (gameMiniGame_Password_Spt)
//	{
//		gameMiniGame_Password_Spt->isVisible = false;
//	}
//
//	gameMiniGame_TextPassword = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_TextPassword").get();
//	if (gameMiniGame_TextPassword)
//	{
//		gameMiniGame_TextPassword_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_TextPassword->entityID);
//		if (gameMiniGame_TextPassword_Txt)
//		{
//			gameMiniGame_TextPassword_Txt->isEnabled = false;
//			gameMiniGame_TextPassword_Txt->text = GenerateRandomNumericString(5, 5);
//		}
//	}
//
//	gameMiniGame_Input = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_Input").get();
//	if (gameMiniGame_Input)
//	{
//		gameMiniGame_Input_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_Input->entityID);
//		if (gameMiniGame_Input_Txt)
//		{
//			gameMiniGame_Input_Txt->isEnabled = false;
//		}
//	}
//
//	gameSS_BG = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SS_BG").get();
//	if (gameSS_BG)
//	{
//		gameSS_BG_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameSS_BG->entityID);
//		if (gameSS_BG_Spt)
//		{
//			gameSS_BG_Spt->isVisible = false;
//		}
//	}
//
//	gameSS_Pad = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SS_Pad").get();
//	if (gameSS_Pad)
//	{
//		gameSS_Pad_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameSS_Pad->entityID);
//		if (gameSS_Pad_Spt)
//		{
//			gameSS_Pad_Spt->isVisible = false;
//		}
//	}
//	
//	gameSS_RedButtonL = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SS_RedL").get();
//	if (gameSS_RedButtonL)
//	{
//		gameSS_RedButtonL_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameSS_RedButtonL->entityID);
//		if (gameSS_RedButtonL_Spt)
//		{
//			gameSS_RedButtonL_Spt->isVisible = false;
//		}
//	}
//
//	gameSS_YellowButtonL = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SS_YellowL").get();
//	if (gameSS_YellowButtonL)
//	{
//		gameSS_YellowButtonL_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameSS_YellowButtonL->entityID);
//		if (gameSS_YellowButtonL_Spt)
//		{
//			gameSS_YellowButtonL_Spt->isVisible = false;
//		}
//	}
//
//	gameSS_GreenButtonL = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SS_GreenL").get();
//	if (gameSS_GreenButtonL)
//	{
//		gameSS_GreenButtonL_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameSS_GreenButtonL->entityID);
//		if (gameSS_GreenButtonL_Spt)
//		{
//			gameSS_GreenButtonL_Spt->isVisible = false;
//		}
//	}
//
//	gameSS_BlueButtonL = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SS_BlueL").get();
//	if (gameSS_BlueButtonL)
//	{
//		gameSS_BlueButtonL_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameSS_BlueButtonL->entityID);
//		if (gameSS_BlueButtonL_Spt)
//		{
//			gameSS_BlueButtonL_Spt->isVisible = false;
//		}
//	}
//
//	gameSS_RedButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SS_Red").get();
//	if (gameSS_RedButton)
//	{
//		gameSS_RedButton_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameSS_RedButton->entityID);
//		if (gameSS_RedButton_Spt)
//		{
//			gameSS_RedButton_Spt->isVisible = false;
//		}
//
//		gameSS_RedButton_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameSS_RedButton->entityID);
//		if (gameSS_RedButton_Btn)
//		{
//			gameSS_RedButton_Btn->onClick = [this]()
//			{
//					//gameSS_RedButtonL_Spt->isVisible = true;
//					ButtonLight('1');
//			};
//		}
//	}
//
//	gameSS_YellowButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SS_Yellow").get();
//	if (gameSS_YellowButton)
//	{
//		gameSS_YellowButton_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameSS_YellowButton->entityID);
//		if (gameSS_YellowButton_Spt)
//		{
//			gameSS_YellowButton_Spt->isVisible = false;
//		}
//
//		gameSS_YellowButton_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameSS_YellowButton->entityID);
//		if (gameSS_YellowButton_Btn)
//		{
//			gameSS_YellowButton_Btn->onClick = [this]()
//			{
//				gameSS_YellowButtonL_Spt->isVisible = true;
//			};
//		}
//	}
//
//	gameSS_GreenButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SS_Green").get();
//	if (gameSS_GreenButton)
//	{
//		gameSS_GreenButton_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameSS_GreenButton->entityID);
//		if (gameSS_GreenButton_Spt)
//		{
//			gameSS_GreenButton_Spt->isVisible = false;
//		}
//
//		gameSS_GreenButton_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameSS_GreenButton->entityID);
//		if (gameSS_GreenButton_Btn)
//		{
//			gameSS_GreenButton_Btn->onClick = [this]()
//			{
//				gameSS_GreenButtonL_Spt->isVisible = true;
//			};
//		}
//	}
//
//	gameSS_BlueButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("SS_Blue").get();
//	if (gameSS_BlueButton)
//	{
//		gameSS_BlueButton_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameSS_BlueButton->entityID);
//		if (gameSS_BlueButton_Spt)
//		{
//			gameSS_BlueButton_Spt->isVisible = false;
//		}
//
//		gameSS_BlueButton_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameSS_BlueButton->entityID);
//		if (gameSS_BlueButton_Btn)
//		{
//			gameSS_BlueButton_Btn->onClick = [this]()
//			{
//				gameSS_BlueButtonL_Spt->isVisible = true;
//			};
//		}
//	}
//
//}
//
//void MiniGameLogic::Update()
//{
//	if (!robotRestockLogic)
//	{
//		auto gameRestockMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Menu").get();
//		if (gameRestockMenu)
//		{
//			robotRestockLogic = GameLogicManager::GetLogicForEntity<RestockLogic>(gameRestockMenu->entityID);
//		}
//	}
//
//	if (robotRestockLogic)
//	{
//		MiniGame = robotRestockLogic->isMiniGame;
//	}
//
//	if (MiniGame)
//	{
//		MiniGame_1(true);
//	}
//	else
//	{
//		MiniGame_1(false);
//	}
//}
//
//void MiniGameLogic::MiniGame_1(bool state)
//{
//	if (gameMiniGame_BG)
//	{
//		gameMiniGame_BG_Spt->isVisible = state;
//	}
//	if (gameMiniGame_Keypad)
//	{
//		gameMiniGame_Keypad_Spt->isVisible = state;
//	}
//	if (gameMiniGame_K1)
//	{
//		gameMiniGame_K1_Spt->isVisible = state;
//	}
//	if (gameMiniGame_K2)
//	{
//		gameMiniGame_K2_Spt->isVisible = state;
//	}
//	if (gameMiniGame_T1)
//	{
//		gameMiniGame_T1_Txt->isEnabled = state;
//	}
//	if (gameMiniGame_T2)
//	{
//		gameMiniGame_T2_Txt->isEnabled = state;
//	}
//	if (gameMiniGame_K3)
//	{
//		gameMiniGame_K3_Spt->isVisible = state;
//	}
//	if (gameMiniGame_K4)
//	{
//		gameMiniGame_K4_Spt->isVisible = state;
//	}
//	if (gameMiniGame_T3)
//	{
//		gameMiniGame_T3_Txt->isEnabled = state;
//	}
//	if (gameMiniGame_T4)
//	{
//		gameMiniGame_T4_Txt->isEnabled = state;
//	}
//	if (gameMiniGame_K5)
//	{
//		gameMiniGame_K5_Spt->isVisible = state;
//	}
//	if (gameMiniGame_K6)
//	{
//		gameMiniGame_K6_Spt->isVisible = state;
//	}
//	if (gameMiniGame_T5)
//	{
//		gameMiniGame_T5_Txt->isEnabled = state;
//	}
//	if (gameMiniGame_T6)
//	{
//		gameMiniGame_T6_Txt->isEnabled = state;
//	}
//	if (gameMiniGame_K7)
//	{
//		gameMiniGame_K7_Spt->isVisible = state;
//	}
//	if (gameMiniGame_K8)
//	{
//		gameMiniGame_K8_Spt->isVisible = state;
//	}
//	if (gameMiniGame_T7)
//	{
//		gameMiniGame_T7_Txt->isEnabled = state;
//	}
//	if (gameMiniGame_T8)
//	{
//		gameMiniGame_T8_Txt->isEnabled = state;
//	}
//	if (gameMiniGame_K9)
//	{
//		gameMiniGame_K9_Spt->isVisible = state;
//	}
//	if (gameMiniGame_K0)
//	{
//		gameMiniGame_K0_Spt->isVisible = state;
//	}
//	if (gameMiniGame_T9)
//	{
//		gameMiniGame_T9_Txt->isEnabled = state;
//	}
//	if (gameMiniGame_T0)
//	{
//		gameMiniGame_T0_Txt->isEnabled = state;
//	}
//	if (gameMiniGame_Enter)
//	{
//		gameMiniGame_Enter_Spt->isVisible = state;
//	}
//	if (gameMiniGame_Delete)
//	{
//		gameMiniGame_Delete_Spt->isVisible = state;
//	}
//	if (gameMiniGame_TextEnter)
//	{
//		gameMiniGame_TextEnter_Txt->isEnabled = state;
//	}
//	if (gameMiniGame_TextDelete)
//	{
//		gameMiniGame_TextDelete_Txt->isEnabled = state;
//	}
//	if (gameMiniGame_Text)
//	{
//		if (textcount == true)
//		{
//			gameMiniGame_Text_Txt->isEnabled = state;
//		}
//		else
//		{
//			gameMiniGame_Text_Txt->isEnabled = !state;
//		}
//	}
//	if (gameMiniGame_Password)
//	{
//		gameMiniGame_Password_Spt->isVisible = state;
//	}
//	if (gameMiniGame_TextPassword)
//	{
//		gameMiniGame_TextPassword_Txt->isEnabled = state;
//	}
//	if (gameMiniGame_Input)
//	{
//		gameMiniGame_Input_Txt->isEnabled = state;
//	}
//}
//
//void MiniGameLogic::passwordInput(std::string num)
//{
//	textcount = false;
//	gameMiniGame_Text_Txt->isEnabled = false;
//	gameMiniGame_Input_Txt->isEnabled = true;
//	gameMiniGame_Input_Txt->text += num;
//}
//
//void MiniGameLogic::enterPassword()
//{
//	if (gameMiniGame_Input_Txt->text == gameMiniGame_TextPassword_Txt->text)
//	{
//		gameMiniGame_Enter_BtnSound->Stop();
//		gameMiniGame_Enter_BtnSound->Play(1);
//		textcount = true;
//		gameMiniGame_Input_Txt->text = "";
//		robotRestockLogic->isMiniGame = false;
//		robotRestockLogic->LowerMaintenanceLevel();
//		gameMiniGame_TextPassword_Txt->text = GenerateRandomNumericString(5, 5);
//		
//	}
//	else
//	{
//		gameMiniGame_Enter_BtnSound->Stop();
//		gameMiniGame_Enter_BtnSound->Play(2);
//		gameMiniGame_Input_Txt->text = "";
//		
//	}
//}
//
//void MiniGameLogic::deletePassword()
//{
//	if (!gameMiniGame_Input_Txt->text.empty())
//	{
//		gameMiniGame_Input_Txt->text.pop_back();
//	}
//}
//
//std::string MiniGameLogic::GenerateRandomNumericString(int minLength, int maxLength) {
//	std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed the random generator
//
//	int length = minLength + (std::rand() % (maxLength - minLength + 1)); // Random length
//	std::string results;
//	results.reserve(length);
//
//	for (int i = 0; i < length; ++i) {
//		results += std::to_string(std::rand() % 10); // Generate a random digit (0-9)
//	}
//
//	return results;
//}
//
//void MiniGameLogic::MiniGame_2(bool state)
//{
//	if (gameSS_BG)
//	{
//		gameSS_BG_Spt->isVisible = state;
//	}
//	if (gameSS_Pad)
//	{
//		gameSS_Pad_Spt->isVisible = state;
//	}
//	if (gameSS_RedButton)
//	{
//		gameSS_RedButton_Spt->isVisible = state;
//	}
//	if (gameSS_YellowButton)
//	{
//		gameSS_YellowButton_Spt->isVisible = state;
//	}
//	if (gameSS_GreenButton)
//	{
//		gameSS_GreenButton_Spt->isVisible = state;
//	}
//	if (gameSS_BlueButton)
//	{
//		gameSS_BlueButton_Spt->isVisible = state;
//	}
//
//}
//
//std::string MiniGameLogic:: SimonSaysColor()
//{
//	std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed the random generator
//	result += std::to_string(std::rand() % 4 + 1);
//
//	for (size_t i = 0; i < result.size(); i++)
//	{
//		ButtonLight(result[i]);
//
//		buffertime();
//
//		ButtonOff(result[i]);
//	}
//
//	return result;
//}
//
//void MiniGameLogic::ButtonLight(char light)
//{
//	if (light == '1')
//	{
//		gameSS_RedButtonL_Spt->isVisible = true;
//	}
//	else if (light == '2')
//	{
//		gameSS_YellowButtonL_Spt->isVisible = true;
//	}
//	else if (light == '3')
//	{
//		gameSS_GreenButtonL_Spt->isVisible = true;
//	}
//	else if (light == '4')
//	{
//		gameSS_BlueButtonL_Spt->isVisible = true;
//	}
//}
//
//void MiniGameLogic::ButtonOff(char light)
//{
//	if (light == '1')
//	{
//		gameSS_RedButtonL_Spt->isVisible = false;
//	}
//	else if (light == '2')
//	{
//		gameSS_YellowButtonL_Spt->isVisible = false;
//	}
//	else if (light == '3')
//	{
//		gameSS_GreenButtonL_Spt->isVisible = false;
//	}
//	else if (light == '4')
//	{
//		gameSS_BlueButtonL_Spt->isVisible = false;
//	}
//}
//
//void MiniGameLogic::buffertime()
//{
//	while (buffer < 3)
//	{
//		buffer += DuckEngine::DeltaTime();
//	}
//}
//
