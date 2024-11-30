/******************************************************************************/
/*!
\file       ComponentFactory.cpp
\author     Jovan Chua 2301244 (50%)
\par        c.shengkaijovan@digipen.edu
\author     Lucas Yee 2301212 (50%)
\par        l.yee@digipen.edu
\date       November 11 2024
\brief      Implementation of the ComponentFactory class to add components to entities based on JSON data.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "ComponentFactory.h"

// Implementation of the function to add components to an entity based on the JSON data
/************************************************************************
@brief Adds various components to an entity based on the provided JSON data.
@param entity, a pointer to the entity to which the components will be added.
@param componentsData, a JSON array containing the component data.
@return nothing
*************************************************************************/
void ComponentFactory::AddComponentsToEntity(Entity* entity, const nlohmann::json& componentsData)
{
	for (const auto& componentData : componentsData)
	{
		std::shared_ptr<Component> component = CreateComponentFromJson(componentData);
		if (component)
		{
			if (auto transform = std::dynamic_pointer_cast<TransformComponent>(component))
			{
				DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TransformComponent>(entity->entityID, *transform);
			}
			else if (auto spriteRenderer = std::dynamic_pointer_cast<SpriteRendererComponent>(component))
			{
				if (!spriteRenderer->texturePath.empty())
				{
					auto loadedTextures = DuckEngine::DUCKENGINE_AssetManager.LoadTexture(spriteRenderer->texturePath);
					if (!loadedTextures.empty())
					{
						spriteRenderer->texture = *loadedTextures[0];
					}
				}
				DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SpriteRendererComponent>(entity->entityID, *spriteRenderer);
			}
			else if (auto animator = std::dynamic_pointer_cast<AnimatorComponent>(component))
			{
				for (auto& [name, animation] : animator->animations)
				{
					for (const auto& texturePath : animation.texturePaths)
					{
						auto loadedTextures = DuckEngine::DUCKENGINE_AssetManager.LoadTexture(texturePath);
						if (!loadedTextures.empty())
						{
							animation.Frames.push_back(loadedTextures[0]);
						}
					}
				}

				DuckEngine::DUCKENGINE_ComponentManager.AddComponent<AnimatorComponent>(entity->entityID, *animator);
			}
			else if (auto soundComponent = std::dynamic_pointer_cast<SoundComponent>(component))
			{
				DuckEngine::DUCKENGINE_ComponentManager.AddComponent<SoundComponent>(entity->entityID, *soundComponent);
			}
			else if (auto boundingBox = std::dynamic_pointer_cast<BoundingBox>(component))
			{
				DuckEngine::DUCKENGINE_ComponentManager.AddComponent<BoundingBox>(entity->entityID, *boundingBox);
			}
			else if (auto boundingCircle = std::dynamic_pointer_cast<BoundingCircle>(component))
			{
				DuckEngine::DUCKENGINE_ComponentManager.AddComponent<BoundingCircle>(entity->entityID, *boundingCircle);
			}
			else if (auto rigidbody = std::dynamic_pointer_cast<RigidbodyComponent>(component))
			{
				DuckEngine::DUCKENGINE_ComponentManager.AddComponent<RigidbodyComponent>(entity->entityID, *rigidbody);
			}
			else if (auto textComponent = std::dynamic_pointer_cast<TextComponent>(component))
			{
				DuckEngine::DUCKENGINE_ComponentManager.AddComponent<TextComponent>(entity->entityID, *textComponent);
			}
			else if (auto buttonComponent = std::dynamic_pointer_cast<ButtonComponent>(component))
			{
				DuckEngine::DUCKENGINE_ComponentManager.AddComponent<ButtonComponent>(entity->entityID, *buttonComponent);
			}

			else if (auto gameLogicComponent = std::dynamic_pointer_cast<GameLogicComponent>(component))
			{
				DuckEngine::DUCKENGINE_ComponentManager.AddComponent<GameLogicComponent>(entity->entityID, *gameLogicComponent);
			}

		}
		else
		{
			std::cerr << "Failed to create component from JSON data" << std::endl;
		}
	}
}

void ComponentFactory::SaveComponentsToJson(int entityID, json& componentsArray)
{
	componentsArray.clear();

	// Save TransformComponent.
	if (auto* transform = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TransformComponent>(entityID))
	{
		json transformData;
		transformData["type"] = "TransformComponent";
		transformData["properties"]["position"] =
		{
			{"x", transform->GetPosition().x},
			{"y", transform->GetPosition().y}
		};
		transformData["properties"]["scale"] =
		{
			{"x", transform->scale.x},
			{"y", transform->scale.y}
		};
		transformData["properties"]["rotation"] = transform->angle;
		transformData["properties"]["relativeToCamera"] = transform->relativeToCamera;
		componentsArray.push_back(transformData);
	}

	// Save SpriteRendererComponent.
	if (auto* spriteRenderer = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SpriteRendererComponent>(entityID))
	{
		json spriteData;
		spriteData["type"] = "SpriteRendererComponent";
		spriteData["properties"]["sprite"] = spriteRenderer->sprite;
		spriteData["properties"]["sortingOrder"] = spriteRenderer->sortingOrder;
		spriteData["properties"]["texture"] = spriteRenderer->GetFilePath();
		spriteData["properties"]["useColor"] = spriteRenderer->useColor;
		spriteData["properties"]["isVisible"] = spriteRenderer->isVisible;
		spriteData["properties"]["color"] =
		{
			{"r", spriteRenderer->color.r},
			{"g", spriteRenderer->color.g},
			{"b", spriteRenderer->color.b},
			{"a", spriteRenderer->color.a}
		};
		componentsArray.push_back(spriteData);
	}

	// Save BoundingBox.
	if (auto* boundingBox = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entityID))
	{
		json boundingBoxData;
		boundingBoxData["type"] = "BoundingBox";
		boundingBoxData["properties"]["center"] =
		{
			{"x", boundingBox->getCenter().x},
			{"y", boundingBox->getCenter().y}
		};
		boundingBoxData["properties"]["size"] =
		{
			{"x", boundingBox->getSize().x},
			{"y", boundingBox->getSize().y}
		};
		componentsArray.push_back(boundingBoxData);
	}

	// Save BoundingCircle.
	if (auto* boundingCircle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entityID))
	{
		json boundingCircleData;
		boundingCircleData["type"] = "BoundingCircle";
		boundingCircleData["properties"]["center"] =
		{
			{"x", boundingCircle->getCenter().x},
			{"y", boundingCircle->getCenter().y}
		};
		boundingCircleData["properties"]["radius"] = boundingCircle->getRadius();
		componentsArray.push_back(boundingCircleData);
	}

	// Save RigidbodyComponent.
	if (auto* rigidbody = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<RigidbodyComponent>(entityID))
	{
		json rigidbodyData;
		rigidbodyData["type"] = "RigidbodyComponent";
		rigidbodyData["properties"]["isStatic"] = rigidbody->isStatic;
		componentsArray.push_back(rigidbodyData);
	}

	// Save SoundComponent.
	if (auto* sound = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<SoundComponent>(entityID))
	{
		json soundData;
		soundData["type"] = "SoundComponent";
		soundData["properties"]["soundID"] = sound->soundID;
		soundData["properties"]["loop"] = sound->loop;
		soundData["properties"]["playOnStart"] = sound->playOnStart;
		soundData["properties"]["volume"] = sound->volume;
		componentsArray.push_back(soundData);
	}

	// Save AnimatorComponent.
	if (auto* animator = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<AnimatorComponent>(entityID))
	{
		nlohmann::json animatorData;
		animatorData["type"] = "AnimatorComponent";
		animatorData["properties"]["animations"] = nlohmann::json::array();

		for (const auto& [animName, animation] : animator->animations)
		{
			nlohmann::json animationData;
			animationData["name"] = animName;
			animationData["frameDuration"] = animation.frameDuration;

			animationData["textures"] = nlohmann::json::array();
			for (const auto& texturePath : animation.texturePaths)
			{
				animationData["textures"].push_back(texturePath);
			}

			animatorData["properties"]["animations"].push_back(animationData);
		}

		componentsArray.push_back(animatorData);
	}

	// Save TextComponent.
	if (auto* textComponent = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<TextComponent>(entityID))
	{
		std::cout << "font name: " << textComponent->fontName << "\n";
		json textData;
		textData["type"] = "TextComponent";
		textData["properties"]["fontName"] = textComponent->fontName;
		textData["properties"]["text"] = textComponent->text;
		textData["properties"]["position"] =
		{
			{"x", textComponent->position.x},
			{"y", textComponent->position.y}
		};
		textData["properties"]["fontSize"] = textComponent->fontSize;
		textData["properties"]["color"] =
		{
			{"r", textComponent->color.r},
			{"g", textComponent->color.g},
			{"b", textComponent->color.b},
			{"a", textComponent->color.a}
		};
		componentsArray.push_back(textData);
	}

	// Save ButtonComponent.
	if (auto* buttonComponent = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<ButtonComponent>(entityID))
	{
		json buttonData;
		buttonData["type"] = "ButtonComponent";
		buttonData["properties"]["minPos"] =
		{
			{"x", buttonComponent->minPos.x},
			{"y", buttonComponent->minPos.y}
		};
		buttonData["properties"]["maxPos"] =
		{
			{"x", buttonComponent->maxPos.x},
			{"y", buttonComponent->maxPos.y}
		};
		componentsArray.push_back(buttonData);
	}

	if (auto* gameLogicComponent = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<GameLogicComponent>(entityID))
	{
		json gameLogicData;
		gameLogicData["type"] = "GameLogicComponent";
		gameLogicData["properties"]["logicNames"] = json::array();

		for (const auto& logicName : gameLogicComponent->logicNames)
		{
			gameLogicData["properties"]["logicNames"].push_back(logicName);
		}

		componentsArray.push_back(gameLogicData);
	}


}

std::shared_ptr<Component> ComponentFactory::CreateComponentFromJson(const nlohmann::json& componentJson)
{
	std::string type = componentJson["type"];

	if (type == "TransformComponent")
	{
		Vec2 position = Serialization::GetVec2(componentJson["properties"], "position", Vec2(0.0f, 0.0f));
		Vec2 scale = Serialization::GetVec2(componentJson["properties"], "scale", Vec2(1.0f, 1.0f));
		float rotation = componentJson["properties"].value("rotation", 0.0f);
		bool relativeToCamera = componentJson["properties"].value("relativeToCamera", true);

		auto transformComponent = std::make_shared<TransformComponent>(position, scale);
		transformComponent->angle = rotation;
		transformComponent->relativeToCamera = relativeToCamera;
		transformComponent->previousPosition = transformComponent->GetPosition();
		return transformComponent;
	}
	else if (type == "SpriteRendererComponent")
	{
		bool sprite = componentJson["properties"].value("sprite", true);
		int layer = componentJson["properties"].value("sortingOrder", 0);
		std::string texturePath = componentJson["properties"].value("texture", "");
		bool useColor = componentJson["properties"].value("useColor", false);
		bool isVisible = componentJson["properties"].value("isVisible", true);

		Color color{ 255, 255, 255, 255 };
		if (componentJson["properties"].contains("color"))
		{
			color.r = static_cast<float>(componentJson["properties"]["color"].value("r", 255));
			color.g = static_cast<float>(componentJson["properties"]["color"].value("g", 255));
			color.b = static_cast<float>(componentJson["properties"]["color"].value("b", 255));
			color.a = static_cast<float>(componentJson["properties"]["color"].value("a", 255));
		}

		auto spriteRenderer = std::make_shared<SpriteRendererComponent>(sprite, layer, useColor, color, isVisible);
		spriteRenderer->texturePath = texturePath;
		return spriteRenderer;
	}
	else if (type == "BoundingBox")
	{
		Vec2 center = Serialization::GetVec2(componentJson["properties"], "center", Vec2(0.0f, 0.0f));
		Vec2 size = Serialization::GetVec2(componentJson["properties"], "size", Vec2(1.0f, 1.0f));

		auto boundingBox = std::make_shared<BoundingBox>(center, size);
		return boundingBox;
	}
	else if (type == "BoundingCircle")
	{
		Vec2 center = Serialization::GetVec2(componentJson["properties"], "center", Vec2(0.0f, 0.0f));
		float radius = componentJson["properties"].value("radius", 1.0f);

		auto boundingCircle = std::make_shared<BoundingCircle>(center, radius);
		return boundingCircle;
	}
	else if (type == "RigidbodyComponent")
	{
		auto rbComponent = std::make_shared<RigidbodyComponent>();
		rbComponent->isStatic = componentJson["properties"].value("isStatic", false);
		return rbComponent;
	}
	else if (type == "AnimatorComponent")
	{
		auto animator = std::make_shared<AnimatorComponent>();
		if (componentJson["properties"].contains("animations"))
		{
			for (const auto& animData : componentJson["properties"]["animations"])
			{
				std::string animName = animData["name"];
				float frameDuration = animData["frameDuration"];
				std::vector<std::string> texturePaths;

				if (animData.contains("textures") && animData["textures"].is_array())
				{
					for (const auto& texturePath : animData["textures"])
					{
						texturePaths.push_back(texturePath.get<std::string>());
					}
				}

				animator->AddAnimation(animName, texturePaths, frameDuration);
			}
		}
		return animator;
	}
	else if (type == "SoundComponent")
	{
		std::string soundID = componentJson["properties"].value("soundID", "");
		bool loop = componentJson["properties"].value("loop", false);
		bool playOnStart = componentJson["properties"].value("playOnStart", false);
		float volume = componentJson["properties"].value("volume", 1.0f);

		auto soundComponent = std::make_shared<SoundComponent>(soundID, loop, playOnStart, volume);
		return soundComponent;
	}

	else if (type == "TextComponent")
	{
		std::string fontName = componentJson["properties"].value("fontName", "Roboto-Black");
		std::string text = componentJson["properties"].value("text", "");
		Vec2 position = Serialization::GetVec2(componentJson["properties"], "position", Vec2(0.0f, 0.0f));
		int fontSize = componentJson["properties"].value("fontSize", 12);

		Color color{ 255, 255, 255, 255 };
		if (componentJson["properties"].contains("color"))
		{
			color.r = static_cast<float>(componentJson["properties"]["color"].value("r", 255));
			color.g = static_cast<float>(componentJson["properties"]["color"].value("g", 255));
			color.b = static_cast<float>(componentJson["properties"]["color"].value("b", 255));
			color.a = static_cast<float>(componentJson["properties"]["color"].value("a", 255));
		}

		auto textComponent = std::make_shared<TextComponent>(fontName, text, position, static_cast<float>(fontSize), color);
		return textComponent;
	}
	else if (type == "ButtonComponent")
	{
		Vec2 minPos = Serialization::GetVec2(componentJson["properties"], "minPos", Vec2(0.0f, 0.0f));
		Vec2 maxPos = Serialization::GetVec2(componentJson["properties"], "maxPos", Vec2(0.0f, 0.0f));

		auto buttonComponent = std::make_shared<ButtonComponent>(minPos, maxPos);
		return buttonComponent;
	}

	else if (type == "GameLogicComponent")
	{
		auto gameLogicComponent = std::make_shared<GameLogicComponent>();

		if (componentJson["properties"].contains("logicNames") && componentJson["properties"]["logicNames"].is_array())
		{
			for (const auto& logicName : componentJson["properties"]["logicNames"])
			{
				gameLogicComponent->logicNames.push_back(logicName.get<std::string>());
			}
		}

		return gameLogicComponent;
		}




	std::cerr << "Error: Unknown component type: " << type << std::endl;
	return nullptr;
}
