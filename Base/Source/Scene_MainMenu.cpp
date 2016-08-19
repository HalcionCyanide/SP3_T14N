#include "Scene_MainMenu.h"
#include <sstream>

#include "Scene_2.h"
#include "SceneTown1.h"
#include "SceneTown2.h"
#include "SceneTown3.h"
#include "SceneFreeField.h"

#include "GameMap.h"
#include "GameObject.h"

std::string Scene_MainMenu::id_ = "Scene Main Menu";

const std::string Scene_MainMenu::UI_Text[10] = { "", "Start", "Settings", "Exit", "New Game", "Load Game", "Return" };

Scene_MainMenu::Scene_MainMenu()
	: SceneEntity()
{
	framerates = 0;
	setName(id_);
	theInteractiveMap = nullptr;
}

Scene_MainMenu::~Scene_MainMenu()
{

}

void Scene_MainMenu::Init()
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());

	// Set Terrain Size
	TerrainScale.Set(500.f, 110.f, 210.f);

	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	projectionStack->LoadMatrix(perspective);

	// Initiallise Model Specific Meshes Here
	Mesh* newMesh = MeshBuilder::GenerateTerrain("terrain", "Image//heightmap_MainMenu.raw", m_heightMap);
	newMesh->textureArray[0] = LoadTGA("Image//RockTex.tga");
	newMesh->textureArray[1] = LoadTGA("Image//GrassStoneTex.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	newMesh = MeshBuilder::GenerateQuad("TFB_Logo", Color(1, 1, 1));
	newMesh->textureArray[0] = LoadTGA("Image//TFB_Logo.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	newMesh = MeshBuilder::GenerateQuad("TFB_Gem", Color(1, 1, 1));
	newMesh->textureArray[0] = LoadTGA("Image//TFB_GEM.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	newMesh = MeshBuilder::GenerateQuad("TFB_Button", Color(1, 1, 1));
	newMesh->textureArray[0] = LoadTGA("Image//TFB_Button.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	Application::cA_MinimumTerrainY = TerrainScale.y * ReadHeightMap(m_heightMap, camera.position.x / TerrainScale.x, camera.position.z / TerrainScale.z) + camera.PlayerHeight;
	Application::cA_CurrentTerrainY = Application::cA_MinimumTerrainY;

	camera.Init(Vector3(-0.5f, Application::cA_CurrentTerrainY, -40.f), Vector3(0, Application::cA_CurrentTerrainY + 5, -35), Vector3(0, 1, 0));
	camera.CameraIsLocked = true;

	for (int i = 0; i < 15; i++)
	{
		BManager.AddHMapBillboard("Tree", m_heightMap, TerrainScale, Vector3((float)i * Math::RandFloatMinMax(-10.f, 10.f) + 40.f, 0.f, 25.f + Math::RandFloatMinMax(-5.f, 10.f)), Vector3(10.f, 20.f, 10.f), Vector3(), camera.position);
		BManager.AddHMapBillboard("Tree", m_heightMap, TerrainScale, Vector3((float)i * Math::RandFloatMinMax(-10.f, 10.f) + 40.f, 0.f, 47.f + Math::RandFloatMinMax(-5.f, 10.f)), Vector3(10.f, 20.f, 10.f), Vector3(), camera.position);
	}

	CurrentMenuState = S_FIRSTLEVEL;
	Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = true;
	InitSceneUIElems();
}

void Scene_MainMenu::InitSceneUIElems()
{
	Vector3 CenterPosition(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.5f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);
	UI_Sys.AddUIElement(UI_Element::UI_LOGO, "TFB_Logo", CenterPosition * 3, CenterPosition * 3, Vector3(1300, 1300, 1), CenterPosition * 1.35f);
	
	UI_Sys.AddUIElement(UI_Element::UI_BUTTON_L_TO_SCRN, "TFB_Button", CenterPosition * -2.f, CenterPosition * -2.f, Vector3(400, 100, 1), Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 1.2f, 0), UI_Text[1]);
	UI_Sys.AddUIElement(UI_Element::UI_BUTTON_L_TO_SCRN, "TFB_Button", CenterPosition * -2.5f, CenterPosition * -2.5f, Vector3(400, 100, 1), Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.9f, 0), UI_Text[2]);
	UI_Sys.AddUIElement(UI_Element::UI_BUTTON_L_TO_SCRN, "TFB_Button", CenterPosition * -3.f, CenterPosition * -3.f, Vector3(400, 100, 1), Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.6f, 0), UI_Text[3]);

	UI_Sys.AddUIElement(UI_Element::UI_BUTTON_T_TO_SCRN, "TFB_Button", Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 1.2f, 0), Vector3(0, CenterPosition.y * 3.f, 0), Vector3(400, 100, 1), Vector3(0, CenterPosition.y * 3.f, 0), UI_Text[4]);
	UI_Sys.AddUIElement(UI_Element::UI_BUTTON_T_TO_SCRN, "TFB_Button", Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.9f, 0), Vector3(0, CenterPosition.y * 3.f, 0), Vector3(400, 100, 1), Vector3(0, CenterPosition.y * 3.f, 0), UI_Text[5]);
	UI_Sys.AddUIElement(UI_Element::UI_BUTTON_T_TO_SCRN, "TFB_Button", Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.6f, 0), Vector3(0, CenterPosition.y * 3.f, 0), Vector3(400, 100, 1), Vector3(0, CenterPosition.y * 3.f, 0), UI_Text[6]);


}

void Scene_MainMenu::UpdateUILogic(float dt, Scene_MainMenu::STATE_MAIN_MENU cState)
{
	for (std::vector<UI_Element*>::iterator it = UI_Sys.cUIS_ElementContainer.begin(); it != UI_Sys.cUIS_ElementContainer.end(); ++it)
	{
		if ((*it)->Active)
		{
			bool ClickSucceeded = false;
			if (CurrentMenuState == S_FIRSTLEVEL)
			{
				(*it)->BoundsActive = true;
				if (((*it)->UI_Text == UI_Text[1] || (*it)->UI_Text == UI_Text[2] || (*it)->UI_Text == UI_Text[3]))
				{
					(*it)->Update(dt, Scene_System::accessing().cSS_InputManager->GetMousePosition(), ClickSucceeded);
					if (ClickSucceeded)
					{
						if (((*it)->UI_Text == UI_Text[1]))
						{
							// Start
							CurrentMenuState = S_SECONDLEVEL;
							for (std::vector<UI_Element*>::iterator it = UI_Sys.cUIS_ElementContainer.begin(); it != UI_Sys.cUIS_ElementContainer.end(); ++it)
							{
								if (((*it)->UI_Text == UI_Text[1] || (*it)->UI_Text == UI_Text[2] || (*it)->UI_Text == UI_Text[3]))
								{
									(*it)->SwapOriginalWithTarget();
								}
								else if (((*it)->UI_Text == UI_Text[4] || (*it)->UI_Text == UI_Text[5] || (*it)->UI_Text == UI_Text[6]))
								{
									(*it)->SwapOriginalWithTarget();
								}
							}
						}
						else if (((*it)->UI_Text == UI_Text[2]))
						{
							// Settings
						}
						else if (((*it)->UI_Text == UI_Text[3]))
						{
							// Exit
							Application::ExitGame = true;
						}
					}
				}
				else if ((*it)->Active)
					(*it)->Update((float)dt);
			}
			else if (CurrentMenuState == S_SECONDLEVEL)
			{
				if (((*it)->UI_Text == UI_Text[4] || (*it)->UI_Text == UI_Text[5] || (*it)->UI_Text == UI_Text[6]))
				{
					(*it)->BoundsActive = true;
					(*it)->Update(dt, Scene_System::accessing().cSS_InputManager->GetMousePosition(), ClickSucceeded);
					if (ClickSucceeded)
					{
						if (((*it)->UI_Text == UI_Text[4]))
						{
							// Start
							Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = false;
							Scene_System::accessing().SwitchScene(SceneTown1::id_);
						}
						else if (((*it)->UI_Text == UI_Text[5]))
						{
							// Load
						}
						else if (((*it)->UI_Text == UI_Text[6]))
						{
							// Return
							CurrentMenuState = S_FIRSTLEVEL;
							for (std::vector<UI_Element*>::iterator it = UI_Sys.cUIS_ElementContainer.begin(); it != UI_Sys.cUIS_ElementContainer.end(); ++it)
							{
								if (((*it)->UI_Text == UI_Text[4] || (*it)->UI_Text == UI_Text[5] || (*it)->UI_Text == UI_Text[6]))
								{
									(*it)->SwapOriginalWithTarget();
								}
								else if (((*it)->UI_Text == UI_Text[1] || (*it)->UI_Text == UI_Text[2] || (*it)->UI_Text == UI_Text[3]))
								{
									(*it)->SwapOriginalWithTarget();
								}
							}
						}
					}
				}
				else if ((*it)->Active)
					(*it)->Update((float)dt);
			}
		}
	}
}

void Scene_MainMenu::Update(float dt)
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	SceneGraphics->Update(dt);

	//Update Camera's Minimum Possible & Current Y Pos
	Application::cA_MinimumTerrainY = TerrainScale.y * ReadHeightMap(m_heightMap, camera.position.x / TerrainScale.x, camera.position.z / TerrainScale.z) + camera.PlayerHeight;

	if (!(Application::cA_CurrentTerrainY - Application::cA_MinimumTerrainY <= Math::EPSILON && Application::cA_MinimumTerrainY - Application::cA_CurrentTerrainY <= Math::EPSILON))
	{
		float RateofChangeY = (Application::cA_CurrentTerrainY - Application::cA_MinimumTerrainY) * (float)dt * (camera.CameraCurrentWalkSpeed / 3);
		if (Application::cA_CurrentTerrainY - RateofChangeY >= Application::cA_MinimumTerrainY || Application::cA_CurrentTerrainY - RateofChangeY <= Application::cA_MinimumTerrainY)
		{
			Application::cA_CurrentTerrainY -= RateofChangeY;
		}
	}

	framerates = 1 / dt;

	if (Scene_System::accessing().cSS_InputManager->GetKeyValue('1'))
	{
		Scene_System::accessing().SwitchScene(SceneTown1::id_);
	}
	else if (Scene_System::accessing().cSS_InputManager->GetKeyValue('2'))
	{
		Scene_System::accessing().SwitchScene(SceneTown2::id_);
	}
	else if (Scene_System::accessing().cSS_InputManager->GetKeyValue('3'))
	{
		Scene_System::accessing().SwitchScene(SceneTown3::id_);
	}
	else if (Scene_System::accessing().cSS_InputManager->GetKeyValue('4'))
	{
		Scene_System::accessing().SwitchScene(Scene_2::id_);
	}

	if (Scene_System::accessing().cSS_InputManager->GetKeyValue('9'))
	{
		Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = false;
		Scene_System::accessing().cSS_InputManager->cIM_CameraPitch = 0;
		Scene_System::accessing().cSS_InputManager->cIM_CameraYaw = 0;
	}
	else if(Scene_System::accessing().cSS_InputManager->GetKeyValue('0'))
	{
		Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = true;
	}

	UpdateUILogic(dt, CurrentMenuState);
	BManager.UpdateContainer(dt, camera.position);
	camera.Update(dt);
}

void Scene_MainMenu::RenderTerrain()
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	modelStack->PushMatrix();
	modelStack->Scale(TerrainScale.x, TerrainScale.y, TerrainScale.z);
	SceneGraphics->RenderMesh("terrain", true);
	modelStack->PopMatrix();
}

void Scene_MainMenu::RenderShadowCasters()
{
	RenderTerrain();
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	for (std::vector<Billboard*>::iterator it = BManager.BillboardContainer.begin(); it != BManager.BillboardContainer.end(); ++it)
	{
		if ((*it)->Active)
		{
			if ((camera.position - camera.target).Normalize().Dot((*it)->GetPosition().Normalized()) < 1.f)
			{
				float TimeRatio = 1;
				if ((*it)->GetLifeTime() != -1)
					TimeRatio = 1.1f - (*it)->GetCurrTime() / (*it)->GetLifeTime();
				modelStack->PushMatrix();
				modelStack->Translate((*it)->GetPosition().x, (*it)->GetPosition().y, (*it)->GetPosition().z);
				modelStack->Rotate(Math::RadianToDegree(atan2(camera.position.x - (*it)->GetPosition().x, camera.position.z - (*it)->GetPosition().z)), 0, 1, 0);
				modelStack->Scale(TimeRatio * (*it)->GetDimensions().x, TimeRatio *(*it)->GetDimensions().y, TimeRatio *(*it)->GetDimensions().z);
				SceneGraphics->RenderMesh((*it)->GetMeshName(), false);
				modelStack->PopMatrix();
			}
			
		}
	}
}

void Scene_MainMenu::RenderSkybox()
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	//left
	modelStack->PushMatrix();
	modelStack->Rotate(90, 0, 1, 0);
	modelStack->Translate(0, 0, -SkyboxSize / 2 + 2.f);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	SceneGraphics->RenderMesh("SB_Left", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Rotate(-90, 0, 1, 0);
	modelStack->Translate(0, 0, -SkyboxSize / 2 + 2.f);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	SceneGraphics->RenderMesh("SB_Right", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Translate(0, 0, -SkyboxSize / 2 + 2.f);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	SceneGraphics->RenderMesh("SB_Front", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Rotate(180, 0, 1, 0);
	modelStack->Translate(0, 0, -SkyboxSize / 2 + 2.f);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	SceneGraphics->RenderMesh("SB_Back", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Rotate(90, 1, 0, 0);
	modelStack->Translate(0, 0, -SkyboxSize / 2 + 2.f);
	modelStack->Rotate(90, 0, 0, 1);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	SceneGraphics->RenderMesh("SB_Top", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Rotate(-90, 1, 0, 0);
	modelStack->Translate(0, 0, -SkyboxSize / 2 + 2.f);
	modelStack->Rotate(-90, 0, 0, 1);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	SceneGraphics->RenderMesh("SB_Bottom", false);
	modelStack->PopMatrix();
}

void Scene_MainMenu::RenderPassGPass()
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	SceneGraphics->m_renderPass = GraphicsEntity::RENDER_PASS::RENDER_PASS_PRE;
	SceneGraphics->m_lightDepthFBO.BindForWriting();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(SceneGraphics->m_gPassShaderID);
	//These matrices should change when light position or direction changes
	if (SceneGraphics->lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		// based on scene size [below]
		SceneGraphics->m_lightDepthProj.SetToOrtho(-1200, 1200, -1200, 1200, 0, 4000);
	}
	else
	{
		SceneGraphics->m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 20);
	}
	SceneGraphics->m_lightDepthView.SetToLookAt(SceneGraphics->lights[0].position.x, SceneGraphics->lights[0].position.y, SceneGraphics->lights[0].position.z, 0, 0, 0, 0, 1, 0);
	// Things that the light sees and creates shadows from.
	RenderShadowCasters();
}

void Scene_MainMenu::RenderPassMain()
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	SceneGraphics->m_renderPass = GraphicsEntity::RENDER_PASS::RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, (GLsizei)Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth, (GLsizei)Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(SceneGraphics->m_programID);
	//pass light depth texture
	SceneGraphics->m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(SceneGraphics->m_parameters[SceneGraphics->U_SHADOW_MAP], 8);

	if (SceneGraphics->lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(SceneGraphics->lights[0].position.x, SceneGraphics->lights[0].position.y, SceneGraphics->lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack->Top() * lightDir;
		glUniform3fv(SceneGraphics->m_parameters[SceneGraphics->U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (SceneGraphics->lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack->Top() * SceneGraphics->lights[0].position;
		glUniform3fv(SceneGraphics->m_parameters[SceneGraphics->U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack->Top() * SceneGraphics->lights[0].spotDirection;
		glUniform3fv(SceneGraphics->m_parameters[SceneGraphics->U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack->Top() * SceneGraphics->lights[0].position;
		glUniform3fv(SceneGraphics->m_parameters[SceneGraphics->U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack->LoadMatrix(perspective);

	// Camera matrix
	viewStack->LoadIdentity();
	viewStack->LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack->LoadIdentity();

	RenderSkybox();
	RenderShadowCasters();

	//SceneGraphics->RenderMesh("reference", false);

	//<!> will remove soon <!>
	for (auto it : testingRenderingStuff)
	{
		GameObject *the3DObject = dynamic_cast<GameObject*>(it);
		if (the3DObject && (camera.position - camera.target).Normalize().Dot(the3DObject->GetPos().Normalized()) < 1.f)
			the3DObject->Render();
	}
	//<!> will remove soon <!>

	SceneGraphics->SetHUD(true);

	UI_Sys.Render();

	if (Scene_System::accessing().cSS_InputManager->cIM_inMouseMode)
	{
		SceneGraphics->RenderMeshIn2D("TFB_Gem", false, 100, 100, Scene_System::accessing().cSS_InputManager->GetMousePosition().x, Scene_System::accessing().cSS_InputManager->GetMousePosition().y);
	}

	std::ostringstream ss;
	ss.str("");
	ss << "Scene Main Menu - FPS:" << framerates;
	ss.precision(3);
	SceneGraphics->RenderTextOnScreen("text", ss.str(), Color(0, 1, 0), 25, 25, 25);

	ss.str("");
	ss << "CVel:" << camera.CameraVelocity;
	ss.precision(3);
	SceneGraphics->RenderTextOnScreen("text", ss.str(), Color(0, 1, 0), 25, 25, 50);

	ss.str("");
	ss << "Mouse Position:" << Scene_System::accessing().cSS_InputManager->GetMousePosition();
	ss.precision(3);
	SceneGraphics->RenderTextOnScreen("text", ss.str(), Color(0, 1, 0), 25, 25, 75);
	//<!> Removing soon
	ss.str("");
	ss << "CPos:" << camera.position;
	ss.precision(3);
	SceneGraphics->RenderTextOnScreen("text", ss.str(), Color(0, 1, 0), 25, 25, 125);
	//<!> Removing soon
	SceneGraphics->SetHUD(false);

	ss.str("9, 0 - Toggle Mouse Modes");
	ss.precision(3);
	SceneGraphics->RenderTextOnScreen("text", ss.str(), Color(0, 1, 0), 25, 25, 75);

}

void Scene_MainMenu::Render()
{
	//*********************************
	//		PRE RENDER PASS
	//*********************************
	RenderPassGPass();
	//*********************************
	//		MAIN RENDER PASS
	//*********************************
	RenderPassMain();
}

void Scene_MainMenu::Exit()
{
	if (theInteractiveMap)
		delete theInteractiveMap;
	//<!> will remove soon <!>
	for (auto it : testingRenderingStuff)
	{
		if (it)
			delete it;
	}
	//<!> will remove soon <!>
}