/******************************************************************************/
/*!
\file       DuckEngine.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       October 3 2024
\brief      Implements the DuckEngine class, which manages the initialization,
			update, rendering, and exit processes of the game engine. It handles
			the coordination of various systems such as input, graphics, and
			camera management.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include <thread>
#include <chrono>
#include <iostream>


#include "DuckEngine.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "GraphicsManager.h"
#include "WindowManager.h"
#include "TimeManager.h"
#include "CameraManager.h"
#include "InputManager.h"
#include "InputManager.h"
#include "ShaderManager.h"
#include "FontManager.h"
#include "CameraManager.h"
#include "ParticleManager.h"
#include "EntityDestroyer.h"

//include systems
#include "SpriteRendererSystem.h"
#include "RigidbodySystem.h"
#include "CircleColliderSystem.h"
#include "BoxColliderSystem.h"
#include "AnimatorSystem.h"
#include "SoundSystem.h"
#include "ButtonSystem.h"
#include "TextSystem.h"
#include "GameLogicSystem.h"
#include "SoundSystem.h"
#include "SpatialGridSystem.h"
#include "ProjectSettings.h"
#include "RadialSliderSystem.h"


//GraphicsManager graphicsManager;
EntityManager DuckEngine::DUCKENGINE_EntityManager;
ComponentManager DuckEngine::DUCKENGINE_ComponentManager;
SystemManager DuckEngine::DUCKENGINE_SystemManager;
SceneManager DuckEngine::DUCKENGINE_SceneManager;
AssetManager DuckEngine::DUCKENGINE_AssetManager;
EntityFactory DuckEngine::DUCKENGINE_EntityFactory;
PrefabManager DuckEngine::DUCKENGINE_PrefabManager;
LevelManager DuckEngine::DUCKENGINE_LevelManager;

//bool DuckEngine::showDebugColliders = false;

bool DuckEngine::isEditor = false;
bool isPlaying = false;
bool DuckEngine::isPaused = false;
bool DuckEngine::isGamePaused = false;
Vector2D DuckEngine::editorMouseWorldPos;
Vector2D DuckEngine::editorMouseScreenPos;
Vector2D DuckEngine::editorContentRegion;
//TextRenderingSystem textRenderingSystem;

double DuckEngine::accumulatedTime = 0.0;
int DuckEngine::currentSteps = 0;
const double DuckEngine::FIXED_TIMESTEP = 1.0 / 60.0;

/************************************************************************
@brief Initializes the DuckEngine by setting up the window, graphics, input,
	   camera, and font managers. It also loads prefabs and adds systems
	   such as sprite rendering and collision systems to the SystemManager.
*************************************************************************/
void DuckEngine::Initialize(bool _isEditor) 
{
	isEditor = _isEditor;
	
	// load project settings
	ProjectSettings::Load("Resources/settings.json");

	WindowInit window = Serialization::GetWindowInit();
	WindowManager::Initialize(ProjectSettings::GetWindowWidth(), ProjectSettings::GetWindowHeight(), "DigiDucks");
	GraphicsManager::Initialize();
	InputManager::Initialize(WindowManager::getWindow());
	CameraManager::Initialize(0.f, 0.f, 10);
	//FontManager::Initialize("../Resources/Roboto-Black.ttf", 48);

	DuckEngine::DUCKENGINE_AssetManager.LoadAll();

	WindowManager::SetVSync(ProjectSettings::GetUseVSync());

	GraphicsManager::Start();
	ParticleManager::Start(2000);
}

void DuckEngine::SetupSystems()
{
	// add the systems
	std::shared_ptr<System> spriteRendererSystem = std::make_shared<SpriteRendererSystem>();
	DUCKENGINE_SystemManager.AddSystem(spriteRendererSystem);

	//// ADDED BY YY
	//std::shared_ptr<System> cameraSystem = std::make_shared<CameraSystem>();
	//DUCKENGINE_SystemManager.AddSystem(cameraSystem);

	// Update Collision System
	// Circle Collider System

	std::shared_ptr<System> circleColliderSystem = std::make_shared<CircleColliderSystem>();
	DUCKENGINE_SystemManager.AddSystem(circleColliderSystem);

	// Box Collider System
	std::shared_ptr<System> boxColliderSystem = std::make_shared<BoxColliderSystem>();
	DUCKENGINE_SystemManager.AddSystem(boxColliderSystem);

	std::shared_ptr<System> rigidbodySystem = std::make_shared<RigidbodySystem>();
	DUCKENGINE_SystemManager.AddSystem(rigidbodySystem);

	std::shared_ptr<System> animatorSystem = std::make_shared<AnimatorSystem>();
	DUCKENGINE_SystemManager.AddSystem(animatorSystem);

	std::shared_ptr<System> soundSystem = std::make_shared<SoundSystem>();
	DUCKENGINE_SystemManager.AddSystem(soundSystem);

	std::shared_ptr<System> gameLogicSystem = std::make_shared<GameLogicSystem>();
	DUCKENGINE_SystemManager.AddSystem(gameLogicSystem);

	//std::shared_ptr<System> textRenderingSystem = std::make_shared<TextRenderingSystem>();
	//DUCKENGINE_SystemManager.AddSystem(textRenderingSystem);

	//std::shared_ptr<System> textRenderingSystem = std::make_shared<TextRenderingSystem>();
	//DUCKENGINE_SystemManager.AddSystem(textRenderingSystem);

	auto textSystem = std::make_shared<TextSystem>();
	DUCKENGINE_SystemManager.AddSystem(textSystem);

	auto buttonSystem = std::make_shared<ButtonSystem>();
	DUCKENGINE_SystemManager.AddSystem(buttonSystem);

	auto radialSliderSystem = std::make_shared<RadialSliderSystem>();
	DUCKENGINE_SystemManager.AddSystem(radialSliderSystem);

	//auto particleSystem = std::make_shared<ParticleSystem>(2000);
	//DUCKENGINE_SystemManager.AddSystem(particleSystem);

	//static ParticleManager g_particleManager(2000);  // 2000 is max pool size


	// start all systems
	DuckEngine::DUCKENGINE_SystemManager.StartAll();

	DuckEngine::DUCKENGINE_AssetManager.LoadAllSounds("Resources/Sounds");
}


void DuckEngine::SetPlaying(bool playing)
{
	isPlaying = playing;
	if (!playing)
	{
		isPaused = false;
		isGamePaused = false;
		GameLogicManager::Clear();
	}
	else
	{
		DuckEngine::DUCKENGINE_SystemManager.StartAll();
	}

}

bool DuckEngine::IsPlaying()
{
	return isPlaying;
}

void DuckEngine::SetPaused(bool paused) 
{
	isPaused = paused;
	if (isPaused)
	{
		SoundSystem::PauseAllSounds();
	}
	else
	{
		SoundSystem::ResumeAllSounds();
		TimeManager::ResetPrevTime();
	}
}

bool DuckEngine::IsPaused() {
	return isPaused;
}

/************************************************************************
@brief Updates the DuckEngine by processing inputs, updating systems,
	   handling scene management, and rendering text. It also updates
	   the window title with the current FPS.
*************************************************************************/
void DuckEngine::Update()
{
	//std::cout << "Height: " << CameraManager::GetHeight() << " Position x: " << CameraManager::GetPosition().x << ", y: " << CameraManager::GetPosition().y << std::endl;


	static bool pausedOrMinimized = false;
	if (IsPaused() || WindowManager::IsWindowMinimized() || !WindowManager::IsWindowFocused())
	{
		if (!pausedOrMinimized)
		{
			std::cout << "Paused" << std::endl;
			DuckEngine::PauseGame(true);
			SoundSystem::PauseAllSounds();
			pausedOrMinimized = true;
		}
		return;
	}
	else if (pausedOrMinimized && !isGamePaused)
	{
		SoundSystem::ResumeAllSounds();
		pausedOrMinimized = false;
		DuckEngine::PauseGame(false);
	}

	static float timer = 0;
	// Look for inputs first   
	TimeManager::StartManagerTimer("Input System");
	TimeManager::EndManagerTimer("Input System");

	// Update viewport
	WindowManager::UpdateViewportDimensions();

	// Get the actual frame time
	TimeManager::UpdateTime(1.0);
	double frameTime = TimeManager::DT();

	// Accumulate time
	accumulatedTime += frameTime;
	currentSteps = 0;


	DuckEngine::DUCKENGINE_SystemManager.EditorUpdateAll();

	if (isEditor && isPlaying || !isEditor)
	{
		DUCKENGINE_SceneManager.Update();
		DUCKENGINE_SystemManager.UpdateAll();
		ParticleManager::Update();
	}
  

	TimeManager::StartManagerTimer("Systems Update");
	// Update in fixed timesteps
	while (accumulatedTime >= FIXED_TIMESTEP)
	{
		// Fixed update step    
		if (isEditor && isPlaying || !isEditor)
		{
			DUCKENGINE_SystemManager.FixedUpdateAll();
		}
		
		accumulatedTime -= FIXED_TIMESTEP;
		currentSteps++;

		if (currentSteps > 5)
		{
			accumulatedTime = 0.0;
			break;
		}
	}
		
	TimeManager::EndManagerTimer("Systems Update");

	EntityDestroyer::ProcessDestructions();

	// Render at whatever FPS we can achieve
	TimeManager::StartManagerTimer("Font System");
	FontManager::Update();
	TimeManager::EndManagerTimer("Font System");

	//DuckEngine::SetWindowTitle("Quack Kitchen | FPS: " + std::to_string(DuckEngine::FPS()));
}

/************************************************************************
@brief Prepares the rendering process for a new frame by clearing the screen
	   and adapting the viewport to any window size changes.
*************************************************************************/
void DuckEngine::StartDraw(GLint width, GLint height)
{
	// Clear the screen
	//Adapt viewport to resized window
	GLint w{ width }, h{ height };
	static GLint old_w{}, old_h{};
	if (w != old_w || h != old_h)
	{
		old_w = w;
		old_h = h;
		glViewport(0, 0, w, h);
		//std::cout << "caalled\n";
	}
}

/************************************************************************
@brief Executes the rendering of the game, including rendering the graphics,
	   debug elements, and the UI if the editor mode is enabled.
*************************************************************************/
void DuckEngine::Draw() 
{
	if (WindowManager::IsWindowMinimized() || !WindowManager::IsWindowFocused())
	{
		InputManager::Update();
		glfwWaitEventsTimeout(0.1);
		return;
	}

	CameraManager::Update();
	TimeManager::StartManagerTimer("Graphics System");
	ParticleManager::Render();
	GraphicsManager::Render();
	GraphicsManager::DrawGizmo();
	TimeManager::EndManagerTimer("Graphics System");

	InputManager::Update();
	DuckEngine::DUCKENGINE_SystemManager.RenderAll();
	DuckEngine::DUCKENGINE_SceneManager.Render();
	
	TimeManager::StartManagerTimer("Font System");
	//FontManager::Render();
	TimeManager::EndManagerTimer("Font System");
	
}

/************************************************************************
@brief Finalizes the rendering process by swapping the window buffers to
	   display the rendered frame.
*************************************************************************/
void DuckEngine::EndDraw()
{
	if (!WindowManager::IsWindowMinimized() && WindowManager::IsWindowFocused())
	{
		glfwSwapBuffers(WindowManager::getWindow());
	}
}

/************************************************************************
@brief Exits the DuckEngine by shutting down various systems and cleaning
	   up resources.
*************************************************************************/
void DuckEngine::Exit() 
{ 
	WindowManager::Exit();
	GraphicsManager::Exit();
	//PrefabManager::Exit();
	InputManager::Exit();
	ShaderManager::Exit();
	

	DUCKENGINE_SceneManager.Shutdown();
	
}

/************************************************************************
@brief Checks if the game engine is still running by determining if the
	   window is closed.
@return True if the window is still open, otherwise false.
*************************************************************************/
bool DuckEngine::Running() {
	if (!WindowManager::CloseWindow())
		return true;
	else
		return false;
}

/************************************************************************
@brief Sets the background color of the game using the specified RGBA values.
@param r The red component of the background color (0-255).
@param g The green component of the background color (0-255).
@param b The blue component of the background color (0-255).
@param a The alpha (transparency) component of the background color (0-255).
*************************************************************************/
void DuckEngine::SetBackgroundColor(const float r, const float g, const float b, const float a) 
{
	GraphicsManager::SetBackgroundColor(r, g, b, a);
}

/************************************************************************
@brief Retrieves the width of the game window.
@return The width of the game window.
*************************************************************************/
int DuckEngine::GetWindowWidth() 
{
	return WindowManager::GetWindowWidth();
}

/************************************************************************
@brief Retrieves the height of the game window.
@return The height of the game window.
*************************************************************************/
int DuckEngine::GetWindowHeight() 
{
	return WindowManager::GetWindowHeight();
}

/************************************************************************
@brief Retrieves the delta time between the current and previous frame.
@return The time in seconds between frames.
*************************************************************************/
float DuckEngine::DeltaTime()
{
	return static_cast<float>(TimeManager::DT());
}

/************************************************************************
@brief Sets the camera position in the game world.
@param x The x-coordinate of the camera.
@param y The y-coordinate of the camera.
*************************************************************************/
void DuckEngine::SetCameraPosition(const float x, const float y) {
	CameraManager::SetPosition(x, y);
}

/************************************************************************
@brief Sets the camera height in the game world.
@param height The new height of the camera.
*************************************************************************/
void DuckEngine::SetCameraHeight(const int height) {
	CameraManager::SetHeight(height);
}

/************************************************************************
@brief Renders text on the screen at a specified position with a given scale
	   and color.
@param text The string of text to render.
@param position The position on the screen where the text will appear.
@param scale The scaling factor for the text.
@param color The color of the text.
*************************************************************************/
void DuckEngine::RenderText(const std::string& fontName, const std::string& text, const Vector2D& position, float scale, const Color& color, bool isUI, int sortingOrder, int layer) {
	TextRenderCommand command{
		fontName,
		text,
		position,
		scale,
		color,
		isUI,
	};

	GraphicsManager::AddToDrawQueue({ layer, sortingOrder, RenderCommandType::Text, command });
}

/************************************************************************
@brief Draws a point on the screen at a specified position, with a given
	   size and color.
@param position The position of the point.
@param size The size of the point.
@param color The color of the point (default is red).
@param relativeToCamera Determines if the point should be drawn relative to the camera.
*************************************************************************/
void DuckEngine::DrawPoint(const Vector2D& position, float size, const Color& color, bool relativeToCamera, int sortingOrder, int layer) {

	// Create a DebugRenderCommand for a point and add it to the debug draw queue
	DebugRenderCommand drawCommand(DebugRenderCommand::POINT, position, {}, size, 0.f, color, relativeToCamera); // position2 and rotation are unused

	GraphicsManager::AddToDrawQueue({ layer, sortingOrder, RenderCommandType::Debug, drawCommand });
}

/************************************************************************
@brief Draws a line between two points on the screen with a specified size
	   and color.
@param start The starting point of the line.
@param end The ending point of the line.
@param size The thickness of the line.
@param color The color of the line (default is red).
@param relativeToCamera Determines if the line should be drawn relative to the camera.
*************************************************************************/
void DuckEngine::DrawLine(const Vector2D& start, const Vector2D& end, float size, const Color& color, bool relativeToCamera, int sortingOrder, int layer) {

	// Create a DebugRenderCommand for a line and add it to the debug draw queue
	DebugRenderCommand drawCommand(DebugRenderCommand::LINE, start, end, size, 0.f, color, relativeToCamera); // rotation is unused

	GraphicsManager::AddToDrawQueue({ layer, sortingOrder, RenderCommandType::Debug, drawCommand });
}

/************************************************************************
@brief Draws a rectangle on the screen using the given minimum and maximum
	   corner positions, with optional rotation.
@param minCorner The position of the bottom-left corner of the rectangle.
@param maxCorner The position of the top-right corner of the rectangle.
@param rotation The rotation angle of the rectangle (default is 0 degrees).
@param color The color of the rectangle (default is red).
@param relativeToCamera Determines if the rectangle should be drawn relative to the camera.
*************************************************************************/
void DuckEngine::DrawRectangle(const Vector2D& minCorner, const Vector2D& maxCorner, float rotation, const Color& color, bool relativeToCamera, int sortingOrder, int layer) {

	// Calculate the center of the rectangle
	Vector2D center = (minCorner + maxCorner) * 0.5f;

	// Calculate the size (width and height) of the rectangle
	Vector2D size = maxCorner - minCorner;

	// Create a DebugRenderCommand for a rectangle with rotation and add it to the debug draw queue
	DebugRenderCommand drawCommand(DebugRenderCommand::RECTANGLE, center, size, 0.f, rotation, color, relativeToCamera); // sizeOrRadius is 0 for rectangles

	GraphicsManager::AddToDrawQueue({ layer, sortingOrder, RenderCommandType::Debug, drawCommand });
}

/************************************************************************
@brief Draws a circle at a specified position with a given radius and color.
@param position The position of the circle's center.
@param radius The radius of the circle.
@param color The color of the circle (is red).
@param relativeToCamera Determines if the circle should be drawn relative to the camera.
*************************************************************************/
void DuckEngine::DrawCircle(const Vector2D& position, float radius, const Color& color, bool relativeToCamera, int sortingOrder, int layer) {

	// Create a DebugRenderCommand for a circle and add it to the debug draw queue
	DebugRenderCommand drawCommand(DebugRenderCommand::CIRCLE, position, {}, radius, 0.f, color, relativeToCamera); // position2 and rotation are unused

	GraphicsManager::AddToDrawQueue({ layer, sortingOrder, RenderCommandType::Debug, drawCommand });
}

/************************************************************************
@brief Sets the window title.
@param title The new title for the game window.
*************************************************************************/
void DuckEngine::SetWindowTitle(std::string title) {
	WindowManager::SetWindowTitle(title.c_str());
}

/************************************************************************
@brief Retrieves the current frames per second (FPS) of the game.
@return The current FPS.
*************************************************************************/
float DuckEngine::FPS() {
	return static_cast<float>(TimeManager::FPS());
}

/************************************************************************
@brief Enables or disables logging for the DuckEngine component manager.
@param enable True to enable logging, false to disable.
*************************************************************************/
void DuckEngine::EnableLogging(bool enable) {
	DUCKENGINE_ComponentManager.EnableLogging(enable);
}

float DuckEngine::GetViewportWidth()
{
	return static_cast<float>(WindowManager::GetViewportWidth());
}

bool DuckEngine::GetEditorBool()
{
	return DuckEngine::isEditor;
}

float DuckEngine::GetViewportHeight()
{
	return static_cast<float>(WindowManager::GetViewportHeight());
} 

void DuckEngine::ToggleFullScreen() {
	WindowManager::ToggleFullscreen();
}

void DuckEngine::MinimizeWindow() {
	WindowManager::MinimizeWindow();
}

void DuckEngine::ToggleShowDebugColliders() {

	for (const auto& [entityId, circleCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingCircle>())
	{
		BoundingCircle* circle = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingCircle>(entityId);

		if (circle) circle->showDebugCollider = !circle->showDebugCollider;
	}

	for (const auto& [entityId, boxCollider] : DuckEngine::DUCKENGINE_ComponentManager.GetComponents<BoundingBox>())
	{
		BoundingBox* box = DuckEngine::DUCKENGINE_ComponentManager.GetComponent<BoundingBox>(entityId);

		if (box) box->showDebugCollider = !box->showDebugCollider;
	}
}

void DuckEngine::CloseWindow()
{
	WindowManager::SetWindowShouldClose();
}

//void DuckEngine::Emit(const Vector2D& pos, const Vector2D& vel) {
//	//ParticleSystem::Emit(pos, vel);
//	//std::cout << "Emitting\n";
//}

void DuckEngine::Emit(const std::string& type, const Vector2D& pos, const Vector2D& baseVel) {
	ParticleManager::Emit(type, pos, baseVel);
}

int DuckEngine::RandomRange(int minVal, int maxVal)
{
	static std::mt19937 rng{ std::random_device{}() };
	std::uniform_int_distribution<int> dist(minVal, maxVal);
	return dist(rng);
}

void DuckEngine::RegisterEmitter(const std::string& ParticleType, Emitter& emitter) {
	ParticleManager::RegisterEmitter(ParticleType, emitter);
}

double DuckEngine::GetGLFWTime() 
{
	return glfwGetTime();
}

void DuckEngine::PauseGame(bool pause) 
{
	TimeManager::FreezeTime(pause);

	if (pause) 
	{
		SoundSystem::PauseAllSounds();
	}
	else {
		SoundSystem::ResumeAllSounds();
	}
}

void DuckEngine::DestroyEntity(int entityID)
{
	EntityDestroyer::MarkForDestruction(entityID);
}

float DuckEngine::PauseDeltaTime()
{
	return static_cast<float>(TimeManager::PauseDT());
}
