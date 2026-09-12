/******************************************************************************/
/*!
\file       main.cpp
\author     Lucas Yee JunJie, l.yee, 2301212
\par        l.yee@digipen.edu
\date       April 5 2024
\brief      entry point of the program

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include <thread>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#ifdef _MSC_VER
#include <crtdbg.h>
#endif
#endif

static void ConfigureSmokeTestEnvironment(bool smokeTest)
{
	if (!smokeTest)
	{
		return;
	}
#ifdef _WIN32
	_putenv_s("QUACK_KITCHEN_SMOKE_TEST", "1");
#else
	setenv("QUACK_KITCHEN_SMOKE_TEST", "1", 1);
#endif
}

#include "GameManager.h"
#include "DuckEngine.h"
#include "DuckEngine_Input.h"
#include "ProjectSettings.h"
#include "PlatformPaths.h"
#include "SaveLoadManager.h"
#include "WindowManager.h"

static GameManager gManager;

#ifdef _WIN32
static bool IsSmokeTest(const char* commandLine)
{
	return commandLine &&
		std::string(commandLine).find("--smoke-test") != std::string::npos;
}

static bool IsHeadlessSmokeTest(const char* commandLine)
{
	return commandLine &&
		std::string(commandLine).find("--headless-smoke-test") != std::string::npos;
}
#else
static bool IsSmokeTest(int argumentCount, char* arguments[])
{
	for (int index = 1; index < argumentCount; ++index)
	{
		if (std::string(arguments[index]) == "--smoke-test")
		{
			return true;
		}
	}
	return false;
}


static bool IsHeadlessSmokeTest(int argumentCount, char* arguments[])
{
	for (int index = 1; index < argumentCount; ++index)
	{
		if (std::string(arguments[index]) == "--headless-smoke-test")
		{
			return true;
		}
	}
	return false;
}
#endif

#ifdef _WIN32
void EnableConsole()
{
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
	freopen_s(&fp, "CONIN$", "r", stdin);

}
#endif

#ifdef _WIN32
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
#else
int main(int argumentCount, char* arguments[])
#endif
{
	const auto launchStarted = std::chrono::steady_clock::now();
#if defined(_WIN32) && defined(_DEBUG)
	EnableConsole();
#endif

#ifdef _WIN32
	(void)hInstance;
	(void)hPrevInstance;
	(void)nCmdShow;
	const bool smokeTest = IsSmokeTest(lpCmdLine);
	const bool headlessSmokeTest = IsHeadlessSmokeTest(lpCmdLine);
#else
	const bool smokeTest = IsSmokeTest(argumentCount, arguments);
	const bool headlessSmokeTest =
		IsHeadlessSmokeTest(argumentCount, arguments);
#endif

#if defined(_MSC_VER) && defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	ConfigureSmokeTestEnvironment(smokeTest);

	const bool runtimeDirectoryReady = PlatformPaths::UseRuntimeDirectory();
	if (headlessSmokeTest)
	{
		return runtimeDirectoryReady &&
			std::filesystem::is_regular_file("Resources/settings.json") &&
			std::filesystem::is_regular_file("Resources/Scenes/Intro.json")
			? 0 : 2;
	}
	const std::filesystem::path userDataDirectory =
		PlatformPaths::GetUserDataDirectory();
	if (!userDataDirectory.empty())
	{
		const std::filesystem::path userSave = userDataDirectory / "save.json";
		const std::filesystem::path legacySave = "Resources/save.json";
		std::error_code migrationError;
		if (!std::filesystem::exists(userSave) &&
			std::filesystem::exists(legacySave))
		{
			std::filesystem::copy_file(
				legacySave, userSave,
				std::filesystem::copy_options::skip_existing,
				migrationError);
		}
		SaveLoadManager::SetSavePath(userSave.string());
	}
	gManager.Engine.Initialize();

	// ProjectSettings supplies release defaults while SaveLoadManager stores the
	// player's choices. Apply the saved values before systems and the intro start
	// so audio and frame pacing are correct from the first rendered frame.
	SaveLoadManager::InitializeSaveFile();
	ProjectSettings::SetMasterVolume(SaveLoadManager::masterVolume);
	ProjectSettings::SetVolumeCategory("BGM", SaveLoadManager::musicVolume);
	ProjectSettings::SetVolumeCategory("SFX", SaveLoadManager::sfxVolume);
	ProjectSettings::SetTargetFPS(SaveLoadManager::targetFPS);
	ProjectSettings::SetUseVSync(SaveLoadManager::useVSync);
	WindowManager::SetVSync(SaveLoadManager::useVSync);

	gManager.InitScenes();
	gManager.SetActiveScene(ProjectSettings::GetStartLevel());
	gManager.Engine.SetupSystems();

	if (!smokeTest)
	{
		DuckEngine::ToggleFullScreen();
	}
	int smokeTestFrames = 0;
	int smokeTestMainMenuFrames = 0;
	int smokeTestExitCode = 0;
	while (gManager.Engine.Running())
	{
		double frameStartTime = DuckEngine::GetGLFWTime();

		// if alt + enter is pressed, toggle full screen
		if (DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_LEFT_ALT) && DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_ENTER)
			|| DuckEngine_Input::IsKeyDown(DuckEngine_Input::KEY_RIGHT_ALT) && DuckEngine_Input::IsKeyPressed(DuckEngine_Input::KEY_ENTER)
			) {
			DuckEngine::ToggleFullScreen();
		}

		gManager.Update();
		gManager.Engine.Update();
#ifdef _DEBUG
		DuckEngine::SetWindowTitle("Quack Kitchen | FPS: " + std::to_string(DuckEngine::FPS()));
#else
		DuckEngine::SetWindowTitle("Quack Kitchen");
#endif
		
		gManager.Engine.StartDraw();

		gManager.Engine.Draw();

		gManager.Engine.EndDraw();

		if (smokeTest)
		{
			++smokeTestFrames;
			if (smokeTestFrames == 1)
			{
				const double firstFrameSeconds =
					std::chrono::duration<double>(
						std::chrono::steady_clock::now() - launchStarted).count();
				std::cout << "Smoke test first frame: "
					<< firstFrameSeconds << " seconds\n";
			}

			if (gManager.Engine.DUCKENGINE_SceneManager.GetActiveSceneName() ==
				"MainMenu")
			{
				++smokeTestMainMenuFrames;
				if (smokeTestMainMenuFrames >= 30)
				{
					std::cout << "Smoke test reached and rendered MainMenu.\n";
					gManager.Engine.CloseWindow();
				}
			}
			else if (std::chrono::duration<double>(
				std::chrono::steady_clock::now() - launchStarted).count() >= 45.0)
			{
				std::cerr << "Smoke test failed: Intro did not reach MainMenu.\n";
				smokeTestExitCode = 4;
				gManager.Engine.CloseWindow();
			}
		}


		static double accumulatedError = 0.0;
		if (!ProjectSettings::GetUseVSync() && ProjectSettings::GetTargetFPS() > 0)
		{
			double targetFrameTime = 1.0 / ProjectSettings::GetTargetFPS();
			double compensationFactor = 0.97;

			if (accumulatedError < -0.005)
			{
				accumulatedError = -0.005;
			}
			else if (accumulatedError > 0.005)
			{
				accumulatedError = 0.005;
			}
			double adjustedFrameTime = (targetFrameTime * compensationFactor) - accumulatedError;

			double frameEndTarget = frameStartTime + adjustedFrameTime;
			double currentTime = DuckEngine::GetGLFWTime();

			if (currentTime < frameEndTarget)
			{
				double timeToWait = frameEndTarget - currentTime;

				if (timeToWait > 0.001)
				{
					std::this_thread::sleep_for(std::chrono::duration<double>(timeToWait - 0.001));
				}

				while (DuckEngine::GetGLFWTime() < frameEndTarget)
				{
					// Just spin
				}
			}

			double frameEndActual = DuckEngine::GetGLFWTime();
			double actualFrameTime = frameEndActual - frameStartTime;
			accumulatedError += (actualFrameTime - targetFrameTime) * 0.1; // Smooth correction
		}


		//THROW_EXCEPTION("Test Error");
	}

	gManager.Engine.Exit();

	return smokeTestExitCode;
}
