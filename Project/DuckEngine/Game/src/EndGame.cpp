#include "EndGame.h"
#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "ImageLoader.h"
#include "MessagingSystem.h"
#include <random>
#include <chrono>
#include <map>
#include "Scene.h"
#include "GameManager.h"
#include "GameScene.h"



Entity* QuitButton;
SoundComponent* quitsound;
ButtonComponent* quit;

Entity* Star_1;
SpriteRendererComponent* Star1;

Entity* Star_2;
SpriteRendererComponent* Star2;

Entity* Star_3;
SpriteRendererComponent* Star3;

Entity* Score;
TextComponent* ScoreText;
extern int scoreValue;

Entity* Background;
SoundComponent* BGMSound;

bool isQuitButtonClicked = false;
bool isHover = false;

void EndScene::Load()
{
	DuckEngine::EnableLogging(false);
	DuckEngine::SetCameraHeight(20);

	QuitButton = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Quit");
	quit = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(QuitButton->entityID);
	quitsound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(QuitButton->entityID);
	quit->onClick = []() 
		{ 
			std::cout << "Button clicked QUIT!!!!!!\n"; 
			quitsound->Play(); 
			isQuitButtonClicked = true;
		};

	quit->onHover = []()
		{
			std::cout << "hover" << std::endl;
			quitsound->PlayHold();
		};

	Score = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Score");
	ScoreText = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(Score->entityID);
	
	if (ScoreText) {
		ScoreText->text = "Score: " + std::to_string(scoreValue);
	}

	Star_1 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Star_1");
	Star_2 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Star_2");
	Star_3 = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("Star_3");

	Star1 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(Star_1->entityID);
	Star2 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(Star_2->entityID);
	Star3 = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(Star_3->entityID);

	Background = DuckEngine::DUCKENGINE_EntityManager.GetEntityByName("BackgroundGameObject");
	BGMSound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(Background->entityID);

}

void EndScene::Start()
{
	Scene::Start();
	//DuckEngine::showDebugColliders = false;
}

void EndScene::Update()
{
	if (isQuitButtonClicked && !quitsound->IsSoundPlaying())
	{
		GameManager::SetActiveScene("MainMenu");
		isQuitButtonClicked = false;
	}
	 



	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<SoundComponent>()) {
		SoundComponent* soundComponent = static_cast<SoundComponent*>(component.get());

		if (soundComponent->playOnStart && !soundComponent->IsSoundPlaying()) {
			soundComponent->Play();
		}
	}

	if (scoreValue > 10) {

		Star1->texture = AssetManager::GetTextureByName("star");
	}

	if (scoreValue > 30) {

		//Star1->texture = AssetManager::GetTextureByName("star");
		Star2->texture = AssetManager::GetTextureByName("star");

	}
	
	if (scoreValue > 50) {
		//Star1->texture = AssetManager::GetTextureByName("star");
		//Star2->texture = AssetManager::GetTextureByName("star");
		Star3->texture = AssetManager::GetTextureByName("star");
	}
	
}

void EndScene::PostUpdate()
{

}

void EndScene::Exit()
{

}

void EndScene::Unload()
{
	// base unload
	Scene::Unload();
}