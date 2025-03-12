#pragma once
#include "Scene.h"
#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "ImageLoader.h"
#include "MessagingSystem.h"
#include "GameManager.h"
#include "SubmitLogic.h"
#include "SpriteRendererComponent.h"
#include "SoundSystem.h"
#include "CustomerLogic.h"
#include "RestockLogic.h"

class MiniGameLogic : public GameLogic
{
public:
	void MiniGame_1(bool state);
	void passwordInput(std::string num);
	void enterPassword();
	void deletePassword();

	MiniGameLogic() : GameLogic(nullptr) {}

	MiniGameLogic(GameLogicComponent* component) : GameLogic(nullptr)
	{
		UNREFERENCED_PARAMETER(component);
	}

	std::shared_ptr<GameLogic> Clone() const override
	{
		auto clone = std::make_shared<MiniGameLogic>(*this);
		clone->component = nullptr;
		return clone;
	}

	void Start() override;
	void Update() override;

	bool MiniGame = false;

private:
	std::shared_ptr<RestockLogic> robotRestockLogic = nullptr;

	Entity* gameMiniGame_BG = nullptr;
	SpriteRendererComponent* gameMiniGame_BG_Spt = nullptr;

	Entity* gameMiniGame_Keypad = nullptr;
	SpriteRendererComponent* gameMiniGame_Keypad_Spt = nullptr;

	Entity* gameMiniGame_Text = nullptr;
	TextComponent* gameMiniGame_Text_Txt = nullptr;

	Entity* gameMiniGame_Input = nullptr;
	TextComponent* gameMiniGame_Input_Txt = nullptr;

	Entity* gameMiniGame_K1 = nullptr;
	SpriteRendererComponent* gameMiniGame_K1_Spt = nullptr;
	ButtonComponent* gameMiniGame_K1_Btn = nullptr;
	Entity* gameMiniGame_T1 = nullptr;
	TextComponent* gameMiniGame_T1_Txt = nullptr;

	Entity* gameMiniGame_K2 = nullptr;
	SpriteRendererComponent* gameMiniGame_K2_Spt = nullptr;
	ButtonComponent* gameMiniGame_K2_Btn = nullptr;
	Entity* gameMiniGame_T2 = nullptr;
	TextComponent* gameMiniGame_T2_Txt = nullptr;

	Entity* gameMiniGame_K3 = nullptr;
	SpriteRendererComponent* gameMiniGame_K3_Spt = nullptr;
	ButtonComponent* gameMiniGame_K3_Btn = nullptr;
	Entity* gameMiniGame_T3 = nullptr;
	TextComponent* gameMiniGame_T3_Txt = nullptr;

	Entity* gameMiniGame_K4 = nullptr;
	SpriteRendererComponent* gameMiniGame_K4_Spt = nullptr;
	ButtonComponent* gameMiniGame_K4_Btn = nullptr;
	Entity* gameMiniGame_T4 = nullptr;
	TextComponent* gameMiniGame_T4_Txt = nullptr;

	Entity* gameMiniGame_K5 = nullptr;
	SpriteRendererComponent* gameMiniGame_K5_Spt = nullptr;
	ButtonComponent* gameMiniGame_K5_Btn = nullptr;
	Entity* gameMiniGame_T5 = nullptr;
	TextComponent* gameMiniGame_T5_Txt = nullptr;

	Entity* gameMiniGame_K6 = nullptr;
	SpriteRendererComponent* gameMiniGame_K6_Spt = nullptr;
	ButtonComponent* gameMiniGame_K6_Btn = nullptr;
	Entity* gameMiniGame_T6 = nullptr;
	TextComponent* gameMiniGame_T6_Txt = nullptr;

	Entity* gameMiniGame_K7 = nullptr;
	SpriteRendererComponent* gameMiniGame_K7_Spt = nullptr;
	ButtonComponent* gameMiniGame_K7_Btn = nullptr;
	Entity* gameMiniGame_T7 = nullptr;
	TextComponent* gameMiniGame_T7_Txt = nullptr;

	Entity* gameMiniGame_K8 = nullptr;
	SpriteRendererComponent* gameMiniGame_K8_Spt = nullptr;
	ButtonComponent* gameMiniGame_K8_Btn = nullptr;
	Entity* gameMiniGame_T8 = nullptr;
	TextComponent* gameMiniGame_T8_Txt = nullptr;

	Entity* gameMiniGame_K9 = nullptr;
	SpriteRendererComponent* gameMiniGame_K9_Spt = nullptr;
	ButtonComponent* gameMiniGame_K9_Btn = nullptr;
	Entity* gameMiniGame_T9 = nullptr;
	TextComponent* gameMiniGame_T9_Txt = nullptr;

	Entity* gameMiniGame_K0 = nullptr;
	SpriteRendererComponent* gameMiniGame_K0_Spt = nullptr;
	ButtonComponent* gameMiniGame_K0_Btn = nullptr;
	Entity* gameMiniGame_T0 = nullptr;
	TextComponent* gameMiniGame_T0_Txt = nullptr;

	Entity* gameMiniGame_Enter = nullptr;
	SpriteRendererComponent* gameMiniGame_Enter_Spt = nullptr;
	ButtonComponent* gameMiniGame_Enter_Btn = nullptr;
	Entity* gameMiniGame_TextEnter = nullptr;
	TextComponent* gameMiniGame_TextEnter_Txt = nullptr;

	Entity* gameMiniGame_Delete = nullptr;
	SpriteRendererComponent* gameMiniGame_Delete_Spt = nullptr;
	ButtonComponent* gameMiniGame_Delete_Btn = nullptr;
	Entity* gameMiniGame_TextDelete = nullptr;
	TextComponent* gameMiniGame_TextDelete_Txt = nullptr;

	Entity* gameMiniGame_Password = nullptr;
	SpriteRendererComponent* gameMiniGame_Password_Spt = nullptr;
	Entity* gameMiniGame_TextPassword = nullptr;
	TextComponent* gameMiniGame_TextPassword_Txt = nullptr;

	bool textcount = true;
};
