#ifndef _SCENE_MAINMENU_H
#define _SCENE_MAINMENU_H

#include "SceneEntity.h"
#include "Scene_System.h"
#include "GraphicsEntity.h"
#include "InputManager.h"
#include "Camera3.h"
#include "Application.h"
#include "Utility.h"
#include <vector>
#include "BillboardManager.h"
#include "UI_System.h"

class Scene_MainMenu : public SceneEntity
{
public:
	static std::string id_;
	Scene_MainMenu();
	virtual ~Scene_MainMenu();

	virtual void Init();
	virtual void Update(float dt);
	virtual void Render();
	virtual void Exit();

	Camera3 camera;


private:
	enum STATE_MAIN_MENU
	{
		S_FIRSTLEVEL,
		S_SECONDLEVEL,
	};

	void InitSceneUIElems();
	void UpdateUILogic(float dt, Scene_MainMenu::STATE_MAIN_MENU);

	// Shadow GPass
	void RenderPassGPass();
	void RenderPassMain();

	//Heightmap stuff
	std::vector<unsigned char> m_heightMap;
	Vector3 TerrainScale;

	// Additional Calls
	void RenderShadowCasters();
	void RenderTerrain();
	void RenderSkybox();

	const float SkyboxSize = 1000;

	BillboardManager BManager;
	UI_System UI_Sys;

	double framerates;

	GenericEntity *theInteractiveMap;
	//<!> will remove soon <!>
	std::vector<GenericEntity*> testingRenderingStuff;
	//<!> will remove soon <!>
};

#endif //_SCENE_MAINMENU_H