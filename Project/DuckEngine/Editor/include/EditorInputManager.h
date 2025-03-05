/******************************************************************************
\file       EditorInputManager.h
\author     Lucas Yee 2301212
\par        l.yee@digipen.edu
\date       November 6, 2024
\brief      Declaration of the EditorInputManager class, which manages input
			events for the editor, such as camera manipulation, scroll handling,
			and entity selection.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

#pragma once
#include "DuckEngine.h"

class EditorInputManager
{
public:
	/**************************************************************************
	@brief Initializes the input manager for the editor.
	**************************************************************************/
	static void Initialize();

	/**************************************************************************
	@brief Updates input handling for each frame, including scrolling, mouse
		   dragging, and key presses.
	**************************************************************************/
	static void Update();

	/**************************************************************************
	@brief Handles scroll input events to zoom the camera in or out.
	@param offsetY The scroll offset along the Y-axis.
	**************************************************************************/
	static void HandleScrollInput(double offsetY);

	/**************************************************************************
	@brief Handles mouse dragging events to move the camera.
	@param deltaX The change in the X position of the mouse.
	@param deltaY The change in the Y position of the mouse.
	**************************************************************************/
	static void HandleMouseDrag(double deltaX, double deltaY);

	/**************************************************************************
	@brief Sets the dragging state for entity manipulation.
	@param enable True to enable dragging, false to disable.
	**************************************************************************/
	static void SetIsDragging(bool enable) { isDragging = enable; }
	static bool GetIsDragging() { return isDragging; }



private:
	static bool isDragging;

};