/******************************************************************************/
/*!
\file       Resources.h
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Defines the Resources namespace, which contains file paths for
			various resources used in the game, including textures, sounds,
			fonts, and shaders. These constants are used to access and load
			the respective resources during runtime.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <string>

/************************************************************************
@brief The Resources namespace provides constants for the file paths of
	   textures, sounds, fonts, and shaders used in the game engine. These
	   paths can be referenced to load the necessary resources.
*************************************************************************/
namespace Resources
{
	// Textures
	const std::string TEXTURE_CHARACTERIDLE = "Resources/characteridle.png";
	const std::string TEXTURE_CHARACTERWALK = "Resources/characterwalk.png";
	const std::string TEXTURE_CRATE = "Resources/Crate.png";
	const std::string TEXTURE_OLDMAN = "Resources/oldman.png";
	const std::string TEXTURE_BACKGROUND = "Resources/background.png";
	const std::string TEXTURE_STATIONS_CUTTINGTABLE = "Resources/Sprites/cutting_table.png";
	const std::string TEXTURE_STATIONS_TOMATOSTOCK = "Resources/Sprites/tomato_stock.png";
	const std::string TEXTURE_STATIONS_CABBAGESTOCK = "Resources/Sprites/cabbage_stock.png";

	// Sounds
	const std::string SOUND_DUCKSOUND = "Resources/Sounds/AnimalDuck.wav";
	const std::string SOUND_THEME = "Resources/Sounds/COMEDYACCENT.wav";

	// Fonts
	const std::string FONT_ARIAL = "Resources/Fonts/arial.ttf";

	// Shaders
	const std::string SHADER_VERTEX = "Resources/Shaders/vertex_shader.glsl";
	const std::string SHADER_FRAGMENT = "Resources/Shaders/fragment_shader.glsl";
}


