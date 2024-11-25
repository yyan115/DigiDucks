#include "PrefabEditor.h"
#include "DuckEngine.h"
#include "GameManager.h"
#include <imgui_internal.h>

bool PrefabEditor::isOpen = false;
std::string PrefabEditor::currentPrefabName = "";
std::shared_ptr<Prefab> PrefabEditor::currentPrefab = nullptr;

void PrefabEditor::OpenPrefabEditor(const std::string& prefabName)
{
    currentPrefabName = prefabName;
    currentPrefab = PrefabManager::GetPrefab(prefabName);
    isOpen = true;
}

void PrefabEditor::Render()
{
    if (!isOpen)
        return;

    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);

    ImGui::Begin(("Prefab Editor - " + currentPrefabName).c_str(), &isOpen, ImGuiWindowFlags_NoCollapse);

    if (currentPrefab)
    {
        ImGui::Columns(2, nullptr, true);

        ImGui::BeginChild("PrefabPreview", ImVec2(0, 0), true, ImGuiWindowFlags_NoCollapse);
        RenderPrefabPreview();
        ImGui::EndChild();

        // Move to the next column
        ImGui::NextColumn();

        ImGui::BeginChild("PrefabProperties", ImVec2(0, 0), true, ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Editing Prefab: %s", currentPrefabName.c_str());
        RenderPrefabProperties();

        if (ImGui::Button("Save Prefab"))
        {
            PrefabManager::SavePrefab(currentPrefabName);
            LevelManager::SaveSceneChanges(GameManager::ActiveSceneName);
            DuckEngine::DUCKENGINE_SceneManager.ReloadScene();
            currentPrefab = PrefabManager::GetPrefab(currentPrefabName);
        }

        ImGui::SameLine();
        if (ImGui::Button("Close"))
        {
            isOpen = false;
        }
        ImGui::EndChild();

        ImGui::Columns(1);
    }
    else
    {
        ImGui::Text("Prefab not found!");
        if (ImGui::Button("Close"))
        {
            isOpen = false;
        }
    }

    ImGui::End();
}


void PrefabEditor::RenderPrefabProperties()
{
    // Ensure componentsData exists and is an array
    if (!currentPrefab || !currentPrefab->componentsData.is_array())
    {
        ImGui::Text("Invalid prefab data or components are missing!");
        return;
    }

    for (auto& componentJson : currentPrefab->componentsData)
    {
        // Check for valid type and properties fields
        if (!componentJson.contains("type") || !componentJson.contains("properties"))
        {
            ImGui::Text("Invalid component format!");
            continue;
        }

        std::string type = componentJson["type"];
        auto& properties = componentJson["properties"];

        // Handle Transform Component
        if (type == "TransformComponent")
        {
            if (properties.contains("position") && properties.contains("scale"))
            {
                auto& position = properties["position"];
                auto& scale = properties["scale"];

                float pos[2] = {
                    position.value("x", 0.0f),
                    position.value("y", 0.0f)
                };

                float scl[2] = {
                    scale.value("x", 1.0f),
                    scale.value("y", 1.0f)
                };

                if (ImGui::CollapsingHeader("Transform Component"))
                {
                    ImGui::Text("Position");
                    ImGui::SameLine(100);
                    if (ImGui::DragFloat2("##Position", pos, 0.1f, -10000.0f, 10000.0f))
                    {
                        position["x"] = pos[0];
                        position["y"] = pos[1];
                    }

                    ImGui::Text("Scale");
                    ImGui::SameLine(100);
                    if (ImGui::DragFloat2("##Scale", scl, 0.1f, 0.1f, 10000.0f))
                    {
                        scale["x"] = scl[0];
                        scale["y"] = scl[1];
                    }

                    float rotation = properties.value("rotation", 0.0f);
                    ImGui::Text("Rotation");
                    ImGui::SameLine(100);
                    if (ImGui::DragFloat("##Rotation", &rotation, 1.0f, -360.0f, 360.0f))
                    {
                        properties["rotation"] = rotation;
                    }
                }
            }
        }
        // Handle Sprite Renderer Component
        else if (type == "SpriteRendererComponent")
        {
            bool useColor = properties.value("useColor", false);
            ImVec4 color = {
                properties["color"].value("r", 255.0f) / 255.0f,
                properties["color"].value("g", 255.0f) / 255.0f,
                properties["color"].value("b", 255.0f) / 255.0f,
                properties["color"].value("a", 255.0f) / 255.0f
            };

            int sortingOrder = properties.value("sortingOrder", 0);
            std::string texturePath = properties.value("texture", "");

            if (ImGui::CollapsingHeader("Sprite Renderer Component"))
            {
                if (ImGui::Checkbox("Use Color", &useColor))
                {
                    properties["useColor"] = useColor;
                }

                if (ImGui::ColorEdit4("Color", &color.x))
                {
                    properties["color"]["r"] = static_cast<int>(color.x * 255);
                    properties["color"]["g"] = static_cast<int>(color.y * 255);
                    properties["color"]["b"] = static_cast<int>(color.z * 255);
                    properties["color"]["a"] = static_cast<int>(color.w * 255);
                }

                ImGui::Text("Sorting Order");
                ImGui::SameLine(120);
                if (ImGui::InputInt("##SortingOrder", &sortingOrder))
                {
                    properties["sortingOrder"] = sortingOrder;
                }

                if (!texturePath.empty())
                {
                    auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTexture(texturePath);
                    if (texture)
                    {
                        ImGui::Text("Current Texture:");
                        ImGui::Image((void*)(intptr_t)*texture, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
                    }
                }
                else
                {
                    ImGui::Text("Current Texture:");
                    ImGui::Button("Empty", ImVec2(64, 64));
                }

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SPRITE_PAYLOAD"))
                    {
                        const char* newTexturePath = static_cast<const char*>(payload->Data);

                        if (texturePath != newTexturePath)
                        {
                            properties["texture"] = newTexturePath;
                            texturePath = newTexturePath;
                        }
                    }
                    ImGui::EndDragDropTarget();
                }

            }
        }
        // Handle Bounding Circle Component
        else if (type == "BoundingCircle")
        {
            if (properties.contains("offset") || properties.contains("radius"))
            {
                // Default the offset to (0, 0) if it is not present
                auto& offset = properties["offset"];
                if (!offset.is_object())
                {
                    offset = { { "x", 0.0f }, { "y", 0.0f } };
                }

                float radius = properties.value("radius", 1.0f);

                float offsetValues[2] = {
                    offset.value("x", 0.0f),
                    offset.value("y", 0.0f)
                };

                if (ImGui::CollapsingHeader("Bounding Circle"))
                {
                    ImGui::Text("Offset");
                    ImGui::SameLine(100);
                    if (ImGui::DragFloat2("##Offset", offsetValues, 0.1f))
                    {
                        offset["x"] = offsetValues[0];
                        offset["y"] = offsetValues[1];
                    }

                    ImGui::Text("Radius");
                    ImGui::SameLine(100);
                    if (ImGui::DragFloat("##Radius", &radius, 0.1f, 0.1f, 10000.0f))
                    {
                        properties["radius"] = radius;
                    }
                }
            }
            else
            {
                properties["offset"] = { { "x", 0.0f }, { "y", 0.0f } };
                properties["radius"] = 1.0f;
            }
        }


        // Handle Rigidbody Component
        else if (type == "RigidbodyComponent")
        {
            bool isStatic = properties.value("isStatic", false);

            if (ImGui::CollapsingHeader("Rigidbody Component"))
            {
                if (ImGui::Checkbox("Is Static", &isStatic))
                {
                    properties["isStatic"] = isStatic;
                }
            }
        }
    }
}



void PrefabEditor::AddComponent()
{
    static int selectedComponentIndex = 0;
    static const std::vector<std::string> componentTypes = {
        "TransformComponent",
        "SpriteRendererComponent",
        "RigidbodyComponent",
        "BoundingBox",
        "BoundingCircle",
        "AnimatorComponent",
        "SoundComponent"
    };

    ImGui::Separator();
    ImGui::Text("Add Component");

    if (ImGui::BeginCombo("##AddComponent", componentTypes[selectedComponentIndex].c_str()))
    {
        for (int i = 0; i < componentTypes.size(); i++)
        {
            bool isSelected = (selectedComponentIndex == i);
            if (ImGui::Selectable(componentTypes[i].c_str(), isSelected))
            {
                selectedComponentIndex = i;
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::Button("Add Component"))
    {
        const std::string& selectedType = componentTypes[selectedComponentIndex];
        nlohmann::json newComponent = { { "type", selectedType }, { "properties", {} } };

        if (selectedType == "TransformComponent")
        {
            newComponent["properties"] = {
                { "position", { { "x", 0.0f }, { "y", 0.0f }, { "z", 0.0f } } },
                { "scale", { { "x", 1.0f }, { "y", 1.0f }, { "z", 1.0f } } }
            };
        }
        else if (selectedType == "SpriteRendererComponent")
        {
            newComponent["properties"] = {
                { "useColor", false },
                { "color", { { "r", 255 }, { "g", 255 }, { "b", 255 }, { "a", 255 } } }
            };
        }
        else if (selectedType == "RigidbodyComponent")
        {
            newComponent["properties"] = {
                { "isStatic", false }
            };
        }

        currentPrefab->componentsData.push_back(newComponent);
    }
}

void PrefabEditor::RenderPrefabPreview()
{
    if (!currentPrefab)
        return;

    std::string texturePath;
    ImVec2 scale = { 1.0f, 1.0f };
    float rotation = 0.0f;
    ImVec2 position = { 0.0f, 0.0f };

    for (const auto& component : currentPrefab->componentsData)
    {
        if (component["type"] == "SpriteRendererComponent")
        {
            texturePath = component["properties"].value("texture", "");
        }
        else if (component["type"] == "TransformComponent")
        {
            if (component["properties"].contains("scale"))
            {
                const auto& scaleJson = component["properties"]["scale"];
                scale = ImVec2(
                    scaleJson.value("x", 1.0f),
                    scaleJson.value("y", 1.0f)
                );
            }

            rotation = component["properties"].value("rotation", 0.0f);

            if (component["properties"].contains("position"))
            {
                const auto& positionJson = component["properties"]["position"];
                position = ImVec2(
                    positionJson.value("x", 0.0f),
                    positionJson.value("y", 0.0f)
                );
            }
        }
    }

    if (!texturePath.empty())
    {
        auto texture = DuckEngine::DUCKENGINE_AssetManager.GetTexture(texturePath);
        if (texture)
        {
            ImVec2 availableSpace = ImGui::GetContentRegionAvail();

            ImVec2 windowPos = ImGui::GetCursorScreenPos();

            ImVec2 textureSize = ImVec2(128.0f, 128.0f);

            ImVec2 scaledTextureSize = ImVec2(
                textureSize.x * scale.x,
                textureSize.y * scale.y
            );

            ImVec2 centerPos = ImVec2(
                (availableSpace.x * 0.5f) + windowPos.x,
                (availableSpace.y * 0.5f) + windowPos.y
            );

            ImVec2 adjustedPos = ImVec2(
                centerPos.x + position.x,
                centerPos.y - position.y 
            );

            ImVec2 textureOffset = ImVec2(
                scaledTextureSize.x * 0.5f,
                scaledTextureSize.y * 0.5f
            );

            float rotationRadians = rotation * (IM_PI / 180.0f);

            ImVec2 corners[4];
            float sinTheta = sin(rotationRadians);
            float cosTheta = cos(rotationRadians);

            corners[0] = ImVec2(-textureOffset.x, -textureOffset.y); 
            corners[1] = ImVec2(textureOffset.x, -textureOffset.y);
            corners[2] = ImVec2(textureOffset.x, textureOffset.y);  
            corners[3] = ImVec2(-textureOffset.x, textureOffset.y);  

            for (int i = 0; i < 4; i++)
            {
                float x = corners[i].x * cosTheta - corners[i].y * sinTheta;
                float y = corners[i].x * sinTheta + corners[i].y * cosTheta;
                corners[i] = ImVec2(x + adjustedPos.x, y + adjustedPos.y);
            }

            ImDrawList* drawList = ImGui::GetWindowDrawList();
            drawList->AddImageQuad(
                (void*)(intptr_t)*texture,
                corners[0], corners[1], corners[2], corners[3],
                ImVec2(0, 1), ImVec2(1, 1), ImVec2(1, 0), ImVec2(0, 0)
            );
        }
        else
        {
            ImGui::Text("Failed to load texture.");
        }
    }
    else
    {
        ImGui::Text("No texture available.");
    }
}
