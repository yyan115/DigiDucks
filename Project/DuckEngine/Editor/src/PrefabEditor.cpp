#include "PrefabEditor.h"
#include "DuckEngine.h"
#include "GameManager.h"

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

    ImGui::Begin(("Prefab Editor - " + currentPrefabName).c_str(), &isOpen, ImGuiWindowFlags_NoCollapse);

    if (currentPrefab)
    {
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
            if (properties.contains("center") && properties.contains("radius"))
            {
                auto& center = properties["center"];
                float radius = properties.value("radius", 1.0f);

                float centerValues[2] = {
                    center.value("x", 0.0f),
                    center.value("y", 0.0f)
                };

                if (ImGui::CollapsingHeader("Bounding Circle"))
                {
                    ImGui::Text("Center");
                    ImGui::SameLine(100);
                    if (ImGui::DragFloat2("##Center", centerValues, 0.1f))
                    {
                        center["x"] = centerValues[0];
                        center["y"] = centerValues[1];
                    }

                    ImGui::Text("Radius");
                    ImGui::SameLine(100);
                    if (ImGui::DragFloat("##Radius", &radius, 0.1f, 0.1f, 10000.0f))
                    {
                        properties["radius"] = radius;
                    }
                }
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
