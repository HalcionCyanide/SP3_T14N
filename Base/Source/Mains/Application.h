#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "..\\Misc\\DetectMemoryLeak.h"
#include <string>
#include "Vector3.h"
#include "irrklang.h"
using namespace irrklang;
//#pragma comment(lib, "../irrKlang-1.5.0/lib/Win32-visualStudio/irrKlang.lib")
//
//const std::string SoundName[] = {
//	"Sounds//.ogg",
//	"Sounds//.ogg",
//	"Sounds//.ogg",
//	"Sounds//.ogg",
//	"Sounds//.ogg",
//	"Sounds//Step_Grass.mp3",
//};

// Notation of c[ClassInitials]_Variable, cA_

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Update();
	void Exit();

	static bool IsKeyPressed(unsigned short key);

	// Stored Window Values
	int cA_WindowHeight, cA_WindowWidth;

	// Stored Camera Values
	static float cA_MinimumTerrainY, cA_CurrentTerrainY;

	//static ISoundEngine* theSoundEngine;
	//static ISound* Sound_Footstep;

	static bool ExitGame;

private:
	Application();
	~Application();

	//Declare a window object
	StopWatch m_timer;
	double m_dElaspedTime;
	double m_dAccumulatedTime_ThreadOne;
	double m_dAccumulatedTime_ThreadTwo;

    bool loadGlobalStuff();
    void convertStringToUpper(std::string &thestring);
};

#endif