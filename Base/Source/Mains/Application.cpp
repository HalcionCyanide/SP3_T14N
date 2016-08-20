#include "Application.h"

#include "..\\Systems\\Scene_System.h"
#include "..\\Scenes\\GraphicsEntity.h"

#include "..\\Scenes\\Scene_MainMenu.h"
#include "..\\Scenes\\Scene_2.h"
#include "..\\Scenes\\SceneBattleScreen.h"
#include "..\\Scenes\\SceneFreeField.h"
#include "..\\Scenes\\SceneTown1.h"
#include "..\\Scenes\\SceneTown2.h"
#include "..\\Scenes\\SceneTown3.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

float Application::cA_MinimumTerrainY = 0, Application::cA_CurrentTerrainY = 0;

ISoundEngine* Application::theSoundEngine = NULL;
ISound* Application::Sound_Footstep = NULL;

bool Application::ExitGame = false;

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

bool Application::IsKeyPressed(unsigned short key)
{
	return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Application::Application()
{
	Application::theSoundEngine = NULL;
	Application::Sound_Footstep = NULL;
}

Application::~Application()
{
	//if (theSoundEngine != NULL)
	//{
	theSoundEngine->drop();
	//}
}

void Application::Init()
{
	// Initialize the default values of the Scene_System
	Scene_System::accessing().Init();

	//Start Sound Engine with Default params
	theSoundEngine = createIrrKlangDevice();
	theSoundEngine->setSoundVolume(0.25f);
	if (!theSoundEngine)
		return;

	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 8);								//Request 8x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					//Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					//Request a specific OpenGL version
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	//We don't want the old OpenGL 
	glfwWindowHint(GLFW_DECORATED, GL_FALSE);						//Create the window without a border

	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());	//Obtain Width and Height values from the monitor;
	cA_WindowWidth = mode->width;
	cA_WindowHeight = mode->height;
	Scene_System::accessing().cSS_InputManager->SetScreenSize((float)cA_WindowWidth, (float)cA_WindowHeight);
	m_window = glfwCreateWindow(cA_WindowWidth, cA_WindowHeight, "SP3 Framework", NULL, NULL); // Create a window with attained values.

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	m_dElaspedTime = 0.0;
	// Accumulator Threads
	m_dAccumulatedTime_ThreadOne = 0.0;
	m_dAccumulatedTime_ThreadTwo = 0.0;

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	float ScreenHeight = 100; // <!>
	float ScreenWidth = ScreenHeight * ((float)cA_WindowWidth / (float)cA_WindowHeight);
	Scene_System::accessing().SetUIDimensions(ScreenWidth, ScreenHeight);

	GraphicsEntity* SceneGraphics = new GraphicsEntity();
	SceneGraphics->Init();
	Scene_System::accessing().setGraphics_Scene(*SceneGraphics);

	Scene_MainMenu *MainMenuScene = new Scene_MainMenu();
	MainMenuScene->Init();
	Scene_System::accessing().AddScene(*MainMenuScene);

	SceneTown1* town1 = new SceneTown1();
	town1->Init();
	Scene_System::accessing().AddScene(*town1);

	SceneTown2* town2 = new SceneTown2();
	town2->Init();
	Scene_System::accessing().AddScene(*town2);

	SceneTown3* town3 = new SceneTown3();
	town3->Init();
	Scene_System::accessing().AddScene(*town3);

	Scene_2* SecondScene = new Scene_2();
	SecondScene->Init();
	Scene_System::accessing().AddScene(*SecondScene);

	SceneFreeField* freefield = new SceneFreeField();
	freefield->Init();
	Scene_System::accessing().AddScene(*freefield);

	SceneBattleScreen* battlescreen = new SceneBattleScreen();
	battlescreen->Init();
	Scene_System::accessing().AddScene(*battlescreen);
}

void Application::Run()
{
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame

	// Active Window Detection
	HWND hwnd = GetActiveWindow();

	//Main Loop
	while (!ExitGame && !glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		if (hwnd == GetActiveWindow())
		{
			Update();
			Scene_System::accessing().getCurrScene().Render();
		}
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
		m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

	} //Check if the ESC key had been pressed or if the window had been closed
	Scene_System::accessing().clearEverything();
}

void Application::Update()
{
	// Get the elapsed time
	m_dElaspedTime = m_timer.getElapsedTime();

	// Update threads
	m_dAccumulatedTime_ThreadOne += m_dElaspedTime;
	if (m_dAccumulatedTime_ThreadOne > 1 / frameTime)
	{
		Scene_System::accessing().cSS_InputManager->UpdateMouse();
		Scene_System::accessing().getCurrScene().Update((float)m_dElaspedTime);
		m_dAccumulatedTime_ThreadOne = 0.0;
	}

	//<!> This is a huge problem even if you made it to be 2 threaded
	m_dAccumulatedTime_ThreadTwo += m_dElaspedTime;
	if (m_dAccumulatedTime_ThreadTwo > 1 / frameTime)
	{
		Scene_System::accessing().cSS_InputManager->HandleUserInput();
		m_dAccumulatedTime_ThreadTwo = 0.0;
	}
	//<!> This is a huge problem even if you made it to be 2 threaded
}

void Application::Exit()
{
	_CrtDumpMemoryLeaks();
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}