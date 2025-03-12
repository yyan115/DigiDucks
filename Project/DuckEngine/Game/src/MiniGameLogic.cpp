#include "GameLogic.h"
#include "RestockLogic.h"
#include "MiniGameLogic.h"
#include "GameScene.h"

void MiniGameLogic::Start()
{
	gameMiniGame_Text = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_Text").get();
	if (gameMiniGame_Text)
	{
		gameMiniGame_Text_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_Text->entityID);
		if (gameMiniGame_Text_Txt)
		{
			gameMiniGame_Text_Txt->isEnabled = false;
		}
	}

	gameMiniGame_BG = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_BG").get();
	if (gameMiniGame_BG)
	{
		gameMiniGame_BG_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_BG->entityID);
		if (gameMiniGame_BG_Spt)
		{
			gameMiniGame_BG_Spt->isVisible = false;
		}
	}

	gameMiniGame_Keypad = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_Keypad").get();
	if (gameMiniGame_Keypad)
	{
		gameMiniGame_Keypad_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_Keypad->entityID);
		if (gameMiniGame_Keypad_Spt)
		{
			gameMiniGame_Keypad_Spt->isVisible = false;
		}
	}

	gameMiniGame_K1 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K1").get();
	if (gameMiniGame_K1)
	{
		gameMiniGame_K1_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K1->entityID);
		if (gameMiniGame_K1_Spt)
		{
			gameMiniGame_K1_Spt->isVisible = false;
		}
		gameMiniGame_K1_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K1->entityID);
		if (gameMiniGame_K1_Btn)
		{
			gameMiniGame_K1_Btn->onClick = [this]() {passwordInput("1"); };
		}
	}

	gameMiniGame_T1 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T1").get();
	if (gameMiniGame_T1)
	{
		gameMiniGame_T1_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T1->entityID);
		if (gameMiniGame_T1_Txt)
		{
			gameMiniGame_T1_Txt->isEnabled = false;
		}
	}

	gameMiniGame_K2 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K2").get();
	if (gameMiniGame_K2)
	{
		gameMiniGame_K2_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K2->entityID);
		if (gameMiniGame_K2_Spt)
		{
			gameMiniGame_K2_Spt->isVisible = false;
		}
		gameMiniGame_K2_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K2->entityID);
		if (gameMiniGame_K2_Btn)
		{
			gameMiniGame_K2_Btn->onClick = [this]() { passwordInput("2"); };
		}
	}

	gameMiniGame_T2 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T2").get();
	if (gameMiniGame_T2)
	{
		gameMiniGame_T2_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T2->entityID);
		if (gameMiniGame_T2_Txt)
		{
			gameMiniGame_T2_Txt->isEnabled = false;
		}
	}

	gameMiniGame_K3 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K3").get();
	if (gameMiniGame_K3)
	{
		gameMiniGame_K3_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K3->entityID);
		if (gameMiniGame_K3_Spt)
		{
			gameMiniGame_K3_Spt->isVisible = false;
		}
		gameMiniGame_K3_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K3->entityID);
		if (gameMiniGame_K3_Btn)
		{
			gameMiniGame_K3_Btn->onClick = [this]() {passwordInput("3"); };
		}
	}

	gameMiniGame_T3 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T3").get();
	if (gameMiniGame_T3)
	{
		gameMiniGame_T3_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T3->entityID);
		if (gameMiniGame_T3_Txt)
		{
			gameMiniGame_T3_Txt->isEnabled = false;
		}
	}

	gameMiniGame_K4 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K4").get();
	if (gameMiniGame_K4)
	{
		gameMiniGame_K4_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K4->entityID);
		if (gameMiniGame_K4_Spt)
		{
			gameMiniGame_K4_Spt->isVisible = false;
		}
		gameMiniGame_K4_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K4->entityID);
		if (gameMiniGame_K4_Btn)
		{
			gameMiniGame_K4_Btn->onClick = [this]() {passwordInput("4"); };
		}
	}

	gameMiniGame_T4 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T4").get();
	if (gameMiniGame_T4)
	{
		gameMiniGame_T4_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T4->entityID);
		if (gameMiniGame_T4_Txt)
		{
			gameMiniGame_T4_Txt->isEnabled = false;
		}
	}

	gameMiniGame_K5 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K5").get();
	if (gameMiniGame_K5)
	{
		gameMiniGame_K5_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K5->entityID);
		if (gameMiniGame_K5_Spt)
		{
			gameMiniGame_K5_Spt->isVisible = false;
		}
		gameMiniGame_K5_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K5->entityID);
		if (gameMiniGame_K5_Btn)
		{
			gameMiniGame_K5_Btn->onClick = [this]() {passwordInput("5"); };
		}
	}

	gameMiniGame_T5 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T5").get();
	if (gameMiniGame_T5)
	{
		gameMiniGame_T5_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T5->entityID);
		if (gameMiniGame_T5_Txt)
		{
			gameMiniGame_T5_Txt->isEnabled = false;
		}
	}

	gameMiniGame_K6 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K6").get();
	if (gameMiniGame_K6)
	{
		gameMiniGame_K6_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K6->entityID);
		if (gameMiniGame_K6_Spt)
		{
			gameMiniGame_K6_Spt->isVisible = false;
		}
		gameMiniGame_K6_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K6->entityID);
		if (gameMiniGame_K6_Btn)
		{
			gameMiniGame_K6_Btn->onClick = [this]() {passwordInput("6"); };
		}
	}

	gameMiniGame_T6 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T6").get();
	if (gameMiniGame_T6)
	{
		gameMiniGame_T6_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T6->entityID);
		if (gameMiniGame_T6_Txt)
		{
			gameMiniGame_T6_Txt->isEnabled = false;
		}
	}

	gameMiniGame_K7 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K7").get();
	if (gameMiniGame_K7)
	{
		gameMiniGame_K7_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K7->entityID);
		if (gameMiniGame_K7_Spt)
		{
			gameMiniGame_K7_Spt->isVisible = false;
		}
		gameMiniGame_K7_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K7->entityID);
		if (gameMiniGame_K7_Btn)
		{
			gameMiniGame_K7_Btn->onClick = [this]() {passwordInput("7"); };
		}
	}

	gameMiniGame_T7 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T7").get();
	if (gameMiniGame_T7)
	{
		gameMiniGame_T7_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T7->entityID);
		if (gameMiniGame_T7_Txt)
		{
			gameMiniGame_T7_Txt->isEnabled = false;
		}
	}

	gameMiniGame_K8 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K8").get();
	if (gameMiniGame_K8)
	{
		gameMiniGame_K8_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K8->entityID);
		if (gameMiniGame_K8_Spt)
		{
			gameMiniGame_K8_Spt->isVisible = false;
		}
		gameMiniGame_K8_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K8->entityID);
		if (gameMiniGame_K8_Btn)
		{
			gameMiniGame_K8_Btn->onClick = [this]() {passwordInput("8"); };
		}
	}

	gameMiniGame_T8 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T8").get();
	if (gameMiniGame_T8)
	{
		gameMiniGame_T8_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T8->entityID);
		if (gameMiniGame_T8_Txt)
		{
			gameMiniGame_T8_Txt->isEnabled = false;
		}
	}

	gameMiniGame_K9 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K9").get();
	if (gameMiniGame_K9)
	{
		gameMiniGame_K9_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K9->entityID);
		if (gameMiniGame_K9_Spt)
		{
			gameMiniGame_K9_Spt->isVisible = false;
		}
		gameMiniGame_K9_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K9->entityID);
		if (gameMiniGame_K9_Btn)
		{
			gameMiniGame_K9_Btn->onClick = [this]() {passwordInput("9"); };
		}
	}

	gameMiniGame_T9 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T9").get();
	if (gameMiniGame_T9)
	{
		gameMiniGame_T9_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T9->entityID);
		if (gameMiniGame_T9_Txt)
		{
			gameMiniGame_T9_Txt->isEnabled = false;
		}
	}

	gameMiniGame_K0 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_K0").get();
	if (gameMiniGame_K0)
	{
		gameMiniGame_K0_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_K0->entityID);
		if (gameMiniGame_K0_Spt)
		{
			gameMiniGame_K0_Spt->isVisible = false;
		}
		gameMiniGame_K0_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_K0->entityID);
		if (gameMiniGame_K0_Btn)
		{
			gameMiniGame_K0_Btn->onClick = [this]() {passwordInput("0"); };
		}
	}

	gameMiniGame_T0 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_T10").get();
	if (gameMiniGame_T0)
	{
		gameMiniGame_T0_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_T0->entityID);
		if (gameMiniGame_T0_Txt)
		{
			gameMiniGame_T0_Txt->isEnabled = false;
		}
	}

	gameMiniGame_Enter = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_Enter").get();
	if (gameMiniGame_Enter)
	{
		gameMiniGame_Enter_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_Enter->entityID);
		if (gameMiniGame_Enter_Spt)
		{
			gameMiniGame_Enter_Spt->isVisible = false;
		}
		gameMiniGame_Enter_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_Enter->entityID);
		if (gameMiniGame_Enter_Btn)
		{
			gameMiniGame_Enter_Btn->onClick = [this]() { enterPassword(); };
		}
	}

	gameMiniGame_TextEnter = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_TextEnter").get();
	if (gameMiniGame_TextEnter)
	{
		gameMiniGame_TextEnter_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_TextEnter->entityID);
		if (gameMiniGame_TextEnter_Txt)
		{
			gameMiniGame_TextEnter_Txt->isEnabled = false;
		}
	}

	gameMiniGame_Delete = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_Delete").get();
	if (gameMiniGame_Delete)
	{
		gameMiniGame_Delete_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_Delete->entityID);
		if (gameMiniGame_Delete_Spt)
		{
			gameMiniGame_Delete_Spt->isVisible = false;
		}
		gameMiniGame_Delete_Btn = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(gameMiniGame_Delete->entityID);
		if (gameMiniGame_Delete_Btn)
		{
			gameMiniGame_Delete_Btn->onClick = [this]() { deletePassword(); };
		}
	}

	gameMiniGame_TextDelete = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_TextDelete").get();
	if (gameMiniGame_TextDelete)
	{
		gameMiniGame_TextDelete_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_TextDelete->entityID);
		if (gameMiniGame_TextDelete_Txt)
		{
			gameMiniGame_TextDelete_Txt->isEnabled = false;
		}
	}

	gameMiniGame_Password = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_Password").get();
	gameMiniGame_Password_Spt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(gameMiniGame_Password->entityID);
	if (gameMiniGame_Password_Spt)
	{
		gameMiniGame_Password_Spt->isVisible = false;
	}

	gameMiniGame_TextPassword = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_TextPassword").get();
	if (gameMiniGame_TextPassword)
	{
		gameMiniGame_TextPassword_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_TextPassword->entityID);
		if (gameMiniGame_TextPassword_Txt)
		{
			gameMiniGame_TextPassword_Txt->isEnabled = false;
		}
	}

	gameMiniGame_Input = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("MiniGame_Input").get();
	if (gameMiniGame_Input)
	{
		gameMiniGame_Input_Txt = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(gameMiniGame_Input->entityID);
		if (gameMiniGame_Input_Txt)
		{
			gameMiniGame_Input_Txt->isEnabled = false;
		}
	}
}

void MiniGameLogic::Update()
{
	if (!robotRestockLogic)
	{
		auto gameRestockMenu = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Restock_Menu").get();
		if (gameRestockMenu)
		{
			robotRestockLogic = GameLogicManager::GetLogicForEntity<RestockLogic>(gameRestockMenu->entityID);
		}
	}

	if (robotRestockLogic)
	{
		MiniGame = robotRestockLogic->isMiniGame;
	}

	if (MiniGame)
	{
		MiniGame_1(true);
	}
	else
	{
		MiniGame_1(false);
	}
}

void MiniGameLogic::MiniGame_1(bool state)
{
	if (gameMiniGame_BG)
	{
		gameMiniGame_BG_Spt->isVisible = state;
	}
	if (gameMiniGame_Keypad)
	{
		gameMiniGame_Keypad_Spt->isVisible = state;
	}
	if (gameMiniGame_K1)
	{
		gameMiniGame_K1_Spt->isVisible = state;
	}
	if (gameMiniGame_K2)
	{
		gameMiniGame_K2_Spt->isVisible = state;
	}
	if (gameMiniGame_T1)
	{
		gameMiniGame_T1_Txt->isEnabled = state;
	}
	if (gameMiniGame_T2)
	{
		gameMiniGame_T2_Txt->isEnabled = state;
	}
	if (gameMiniGame_K3)
	{
		gameMiniGame_K3_Spt->isVisible = state;
	}
	if (gameMiniGame_K4)
	{
		gameMiniGame_K4_Spt->isVisible = state;
	}
	if (gameMiniGame_T3)
	{
		gameMiniGame_T3_Txt->isEnabled = state;
	}
	if (gameMiniGame_T4)
	{
		gameMiniGame_T4_Txt->isEnabled = state;
	}
	if (gameMiniGame_K5)
	{
		gameMiniGame_K5_Spt->isVisible = state;
	}
	if (gameMiniGame_K6)
	{
		gameMiniGame_K6_Spt->isVisible = state;
	}
	if (gameMiniGame_T5)
	{
		gameMiniGame_T5_Txt->isEnabled = state;
	}
	if (gameMiniGame_T6)
	{
		gameMiniGame_T6_Txt->isEnabled = state;
	}
	if (gameMiniGame_K7)
	{
		gameMiniGame_K7_Spt->isVisible = state;
	}
	if (gameMiniGame_K8)
	{
		gameMiniGame_K8_Spt->isVisible = state;
	}
	if (gameMiniGame_T7)
	{
		gameMiniGame_T7_Txt->isEnabled = state;
	}
	if (gameMiniGame_T8)
	{
		gameMiniGame_T8_Txt->isEnabled = state;
	}
	if (gameMiniGame_K9)
	{
		gameMiniGame_K9_Spt->isVisible = state;
	}
	if (gameMiniGame_K0)
	{
		gameMiniGame_K0_Spt->isVisible = state;
	}
	if (gameMiniGame_T9)
	{
		gameMiniGame_T9_Txt->isEnabled = state;
	}
	if (gameMiniGame_T0)
	{
		gameMiniGame_T0_Txt->isEnabled = state;
	}
	if (gameMiniGame_Enter)
	{
		gameMiniGame_Enter_Spt->isVisible = state;
	}
	if (gameMiniGame_Delete)
	{
		gameMiniGame_Delete_Spt->isVisible = state;
	}
	if (gameMiniGame_TextEnter)
	{
		gameMiniGame_TextEnter_Txt->isEnabled = state;
	}
	if (gameMiniGame_TextDelete)
	{
		gameMiniGame_TextDelete_Txt->isEnabled = state;
	}
	if (gameMiniGame_Text)
	{
		if (textcount == true)
		{
			gameMiniGame_Text_Txt->isEnabled = state;
		}
		else
		{
			gameMiniGame_Text_Txt->isEnabled = !state;
		}
	}
	if (gameMiniGame_Password)
	{
		gameMiniGame_Password_Spt->isVisible = state;
	}
	if (gameMiniGame_TextPassword)
	{
		gameMiniGame_TextPassword_Txt->isEnabled = state;
	}
	if (gameMiniGame_Input)
	{
		gameMiniGame_Input_Txt->isEnabled = state;
	}
}

void MiniGameLogic::passwordInput(std::string num)
{
	textcount = false;
	gameMiniGame_Text_Txt->isEnabled = false;
	gameMiniGame_Input_Txt->isEnabled = true;
	gameMiniGame_Input_Txt->text += num;
}

void MiniGameLogic::enterPassword()
{
	if (gameMiniGame_Input_Txt->text == gameMiniGame_TextPassword_Txt->text)
	{
		textcount = true;
		gameMiniGame_Input_Txt->text = "";
		robotRestockLogic->isMiniGame = false;
		robotRestockLogic->LowerMaintenanceLevel();
	}
	else
	{
		gameMiniGame_Input_Txt->text = "";
	}
}

void MiniGameLogic::deletePassword()
{
	if (!gameMiniGame_Input_Txt->text.empty())
	{
		gameMiniGame_Input_Txt->text.pop_back();
	}
}
