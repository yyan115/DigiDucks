#include "TutorialLogic.h"
#include "DuckEngine.h"


std::vector<SpriteRendererComponent*> tutorialTextures;
int currentTextureIndex = 0;


void TutorialLogic::Start()
{
	Entity* currentEntity = DuckEngine::DUCKENGINE_EntityManager.GetEntity(GetComponentID()).get();

	tutorialTextures.clear();

	for (auto& child : currentEntity->childEntities)
	{
		SpriteRendererComponent* texture = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(child->entityID);
		if (texture != nullptr)
		{
			tutorialTextures.push_back(texture);
		}
	}

	for (size_t i = 0; i < tutorialTextures.size(); ++i)
	{
		tutorialTextures[i]->isVisible = (i == 0);
	}

	currentTextureIndex = 0;
}

void TutorialLogic::Update()
{

}


void TutorialLogic::FixedUpdate()
{

}
void TutorialLogic::ShowNextTexture()
{
	if (tutorialTextures.empty())
	{
		return;
	}

	tutorialTextures[currentTextureIndex]->isVisible = false;
	currentTextureIndex = (currentTextureIndex + 1) % tutorialTextures.size();
	tutorialTextures[currentTextureIndex]->isVisible = true;
}



