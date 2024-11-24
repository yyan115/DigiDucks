/******************************************************************************/
/*!
\file     FILEXTENSIONS.h
\author   Muhammad Zikry Bin Zakaria , muhammadzikry.b, 2201751 (100%)
\par      muhammadzikry.b@digipen.edu
\brief    This file contains the declaration of the Gizmos class
		  which is responsible for rendering gizmos in the editor.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#ifndef FILEXTENSIONS_H
#define FILEXTENSIONS_H

#include <vector>
#include <string>
#include <unordered_map>

std::unordered_map<std::string, std::vector<std::string>> folderAllowedExtensions = {
        {"Fonts", {".ttf"}},
        {"Prefabs", {".json"}},
        {"Scenes", {".json"}},
        {"Shaders", {".vert", ".frag"}},
        {"Sounds", {".ogg", ".mp3", ".wav"}},
        {"Sprites", {".png", ".jpg", ".jpeg"}},        
        {"Scripts", {".txt", ".json", ".lua"}},
        // Add other folders and their extensions as needed
};

#endif // FILEXTENSIONS_H
