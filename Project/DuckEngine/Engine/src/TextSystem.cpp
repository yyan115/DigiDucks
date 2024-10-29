#include "TextSystem.h"
#include "TextComponent.h"
#include "DuckEngine_Input.h"

void TextSystem::Start()
{

}

void TextSystem::Update()
{
	for (const auto& [entityId, component] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<TextComponent>())
	{
		TextComponent* text = static_cast<TextComponent*>(component.get());

		// Skip if disabled
		if (!text->isEnabled)
		{
			continue;
		}

		DuckEngine::RenderText(text->text, text->position, text->fontSize, text->color);
	}
}