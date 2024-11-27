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

void EndScene::Load()
{
	DuckEngine::EnableLogging(false);
	DuckEngine::SetCameraHeight(20);
}

void EndScene::Start()
{
	Scene::Start();
	DuckEngine::showDebugDraw = false;
}

void EndScene::Update()
{

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