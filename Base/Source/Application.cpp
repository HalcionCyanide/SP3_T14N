#include "Application.h"

#include "Scene_System.h"
#include "GraphicsEntity.h"

#include "Scene_1.h"
#include "Scene_2.h"

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

float Application::cA_CameraYaw = 0.0, Application::cA_CameraPitch = 0.0;
int Application::cA_WindowWidth = 800, Application::cA_WindowHeight = 600;
float Application::cA_MinimumTerrainY = 0, Application::cA_CurrentTerrainY = 0;

ISoundEngine* Application::theSoundEngine = NULL;
ISound* Application::Sound_Footstep = NULL;

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

bool Application::GetMouseUpdate()
{
	//New Version for use with my camera version
	//Lock the cursor's position to the center of the screen.
	int moveX = cA_WindowWidth / 2;
	int moveY = cA_WindowHeight / 2;
	POINT mousePosition;
	GetCursorPos(&mousePosition);
	SetCursorPos(moveX, moveY);
	
	//Calculate the difference between the cursor coordinates between frames
	cA_CameraYaw = static_cast<float>(mousePosition.x - moveX);
	cA_CameraPitch = static_cast<float>(mousePosition.y - moveY);

	return false;
}

Application::Application()
{
	Application::theSoundEngine = NULL;
	Application::Sound_Footstep = NULL;
}

Application::~Application()
{
	if (theSoundEngine != NULL)
	{
		theSoundEngine->drop();
	}
}

void Application::Init()
{
	//Start Sound Engine with Default params
	theSoundEngine = createIrrKlangDevice();
	theSoundEngine->setSoundVolume(0.25f);
	if (!theSoundEngine)
		return ;

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

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);	// Hides the cursor within the window
}

void Application::Run()
{
    m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame

	// Initialize the default values of the Scene_System
	Scene_System::accessing().Init();
	GraphicsEntity* SceneGraphics = new GraphicsEntity();
	SceneGraphics->Init();
	Scene_System::accessing().setGraphics_Scene(*SceneGraphics);

	Scene_1 *FirstScene = new Scene_1();
	FirstScene->Init();
	Scene_System::accessing().AddScene(*FirstScene);

	Scene_2* SecondScene = new Scene_2();
	SecondScene->Init();
	Scene_System::accessing().AddScene(*SecondScene);

	//Main Loop
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		Update(); 
		Scene_System::accessing().getCurrScene().Render();
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
	// Active Window Detection
	HWND hwnd = GetActiveWindow();

	// Get the elapsed time
	m_dElaspedTime = m_timer.getElapsedTime();

	// Update threads
	m_dAccumulatedTime_ThreadOne += m_dElaspedTime;
	if (m_dAccumulatedTime_ThreadOne > 1 / frameTime && (hwnd == GetActiveWindow()))
	{
		GetMouseUpdate();
		//GetKeyboardUpdate();
		Scene_System::accessing().getCurrScene().Update((float)m_dElaspedTime);
		m_dAccumulatedTime_ThreadOne = 0.0;
	}

	//m_dAccumulatedTime_ThreadTwo += m_dElaspedTime;
	//if (m_dAccumulatedTime_ThreadTwo > 0.5)
	//{
	//	// Segment Update Calls If Need Be
	//	m_dAccumulatedTime_ThreadTwo = 0.0;
	//}
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}
