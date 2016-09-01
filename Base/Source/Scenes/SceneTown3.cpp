#include "SceneTown3.h"
#include <sstream>

#include "SceneTown1.h"
#include "SceneTown2.h"
#include "SceneFreeField.h"
#include "SceneFreeField2.h"

#include "..\Misc\SimpleCommand.h"
#include "..\\Classes\\GameMap.h"
#include "..\\Classes\\PlayerObject.h"
#include "..\\Systems\\UI_System.h"
#include "../Misc/LoadEnemyData.h"
#include "../Systems/MusicSystem.h"

std::string SceneTown3::id_ = "3_Scene";

SceneTown3::SceneTown3()
	: SceneEntity()
{
	framerates = 0;
	setName(id_);
	theInteractiveMap = nullptr;
}

SceneTown3::~SceneTown3()
{

}

void SceneTown3::Init()
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());

	// Set Terrain Size
	TerrainScale.Set(350.f, 50.f, 350.f);

	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight, 0.1f, 10000.0f);
	projectionStack->LoadMatrix(perspective);

    camera = new Camera3();
	camera->Init(Vector3(0, 5, -5), Vector3(0, 5, 0), Vector3(0, 1, 0));

	// Initiallise Model Specific Meshes Here
	Mesh* newMesh = MeshBuilder::GenerateTerrain("Town 3", "HeightMapFiles//heightmap_Town3.raw", m_heightMap);
	newMesh->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	newMesh->textureArray[0] = LoadTGA("Image//RockTex.tga");
	newMesh->textureArray[1] = LoadTGA("Image//GrassStoneTex.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	Application::cA_MinimumTerrainY = TerrainScale.y * ReadHeightMap(m_heightMap, camera->position.x / TerrainScale.x, camera->position.z / TerrainScale.z) + camera->PlayerHeight;
	Application::cA_CurrentTerrainY = Application::cA_MinimumTerrainY;

	theInteractiveMap = new GameMap();
	GameMap *theMap = dynamic_cast<GameMap*>(theInteractiveMap);
	theMap->setName("scene town 3 logic map");
	theMap->LoadMap("DrivenFiles//Town3Layout.csv", m_heightMap, TerrainScale, objVec, BManager);

	// There can only be 1 Player
	Player = new PlayerObject();
	Player->Init("Player", 1, camera->position - Vector3(0, camera->PlayerHeight, 0), Vector3(2, 1, 2), Vector3(), camera->CurrentCameraRotation.y, Vector3(0, 1));
	std::map<std::string, Mesh*>::iterator it = SceneGraphics->meshList.find("cube");
	Player->setName("PLayer 1");
	Player->SetMesh(it->second);

	PlayerObject* PlayerPTR = dynamic_cast<PlayerObject*>(Player);
	//PlayerPTR->cameraObject = &camera;
	PlayerPTR->SetVelocity(Vector3(10.f, 0.f, 0.f));
	PlayerPTR->SetPosition(Vector3(Player->GetPosition().x, camera->PlayerHeight + TerrainScale.y * ReadHeightMap(m_heightMap, (Player->GetPosition().x / TerrainScale.x), (Player->GetPosition().z / TerrainScale.z)), Player->GetPosition().z));
	PlayerPTR->setPlayerBoundaries(objVec);
	camera->position = PlayerPTR->GetPosition();
	// There can only be 1 Player
    transitingSceneName = "";
}

void SceneTown3::InitChatUI()
{
	Vector3 ButtonScale(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.20f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.1f, 1);
	ChatLayer = new UI_Layer();
	// Name
	Vector3 DefaultPos(CenterPosition.x  * 0.3f, CenterPosition.y * 0.65f, 0);
	NPC_Name = new UI_Element("TFB_Button", DefaultPos, DefaultPos, Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.25f, Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / 20, 1), DefaultPos, "Sek Heng");
	ChatLayer->cUI_Layer.push_back(NPC_Name);

	// Text Box
	DefaultPos.Set(CenterPosition.x, CenterPosition.y * 0.3f, 0);
	NPC_TextBox = new UI_Element("UI_ChatBox", DefaultPos, DefaultPos, Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.95f, Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / 7, 1), DefaultPos, "Text Wrapping Test Text Wrapping Test Text Wrapping Test");
	ChatLayer->cUI_Layer.push_back(NPC_TextBox);

	// Quest Buttons
	DefaultPos.Set(CenterPosition.x * 1.75f, CenterPosition.y * 0.7f, 0);
	NPC_QuestButtons.push_back(new UI_Element("TFB_Button", DefaultPos, DefaultPos, Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / 5, Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / 20, 1), DefaultPos, "Exit"));
	ChatLayer->cUI_Layer.push_back(NPC_QuestButtons.back());

	DefaultPos.Set(CenterPosition.x * 1.75f, CenterPosition.y * 1.3f, 0);
	NPC_QuestButtons.push_back(new UI_Element("TFB_Button", DefaultPos, DefaultPos, Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / 5, Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / 20, 1), DefaultPos, "Q1"));
	ChatLayer->cUI_Layer.push_back(NPC_QuestButtons.back());

	DefaultPos.Set(CenterPosition.x * 1.75f, CenterPosition.y * 1.1f, 0);
	NPC_QuestButtons.push_back(new UI_Element("TFB_Button", DefaultPos, DefaultPos, Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / 5, Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / 20, 1), DefaultPos, "Q2"));
	ChatLayer->cUI_Layer.push_back(NPC_QuestButtons.back());

	DefaultPos.Set(CenterPosition.x * 1.75f, CenterPosition.y * 0.9f, 0);
	NPC_QuestButtons.push_back(new UI_Element("TFB_Button", DefaultPos, DefaultPos, Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / 5, Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / 20, 1), DefaultPos, "Q3"));
	ChatLayer->cUI_Layer.push_back(NPC_QuestButtons.back());

	// Chat Layer Settings
	ChatLayer->LayerCenterPosition.y = -Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight;
	ChatLayer->LayerOriginalPosition = 0;
	ChatLayer->LayerTargetPosition.y = -Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight;

	UI_Sys->cUIS_LayerContainer.push_back(ChatLayer);
}

std::string SceneTown3::HandleChatUIInput(float dt)
{
	for (std::vector<UI_Element*>::iterator it = NPC_QuestButtons.begin(); it != NPC_QuestButtons.end(); ++it)
	{
		if ((*it)->Active)
		{
			(*it)->BoundsActive = true;
			bool ClickDetection = false;
			if ((*it)->BoundsActive)
			{
				(*it)->CheckInput(Scene_System::accessing().cSS_InputManager->GetMousePosition(), ClickDetection);
				if (ClickDetection)
				{
					(*it)->BoundsActive = false;
					return (*it)->UI_Text;
				}
			}
		}
	}
	return "";
}

void SceneTown3::NPC_chat(float dt)
{
	for (std::vector<NPC*>::iterator it = Scene_System::accessing().NM.allNPCs.begin(); it != Scene_System::accessing().NM.allNPCs.end(); ++it)
	{
		NPC* CurrentNPC = *it;

		// Update and rotate the NPC in accordance to the player[camera]'s position.
		CurrentNPC->setTarget(camera->position);

		CurrentNPC->Update((float)dt);
		float DistanceCheck = (camera->position - CurrentNPC->GetPosition()).LengthSquared();
		if (DistanceCheck < CurrentNPC->GetDetectionRadiusSquared() && Scene_System::accessing().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::INTERACT_COMMAND]) && !CurrentNPC->getInteracting())
		{
			CurrentNPC->setInteracting(true);
			Scene_System::accessing().cSS_PlayerUIManager->CurrentState = PlayerUIManager::UIS_NO_UI;

			if (ChatLayer->LayerTargetPosition.y < 0)
				ChatLayer->SwapOriginalWithTarget();
			break;
		}
		// The NPC has interacted with the player successfully.
		if (CurrentNPC->getInteracting())
		{
			NPC_Name->UI_Text = CurrentNPC->getName();
			NPC_TextBox->UI_Text = CurrentNPC->getFText();
			NPC_TextBox->WrapText();

			// Enable the mouse.
			Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = true;
			camera->CameraIsLocked = true;

			// Set the player's target to face the NPC
			camera->target = Vector3(CurrentNPC->GetPosition().x, Application::cA_CurrentTerrainY + (CurrentNPC->GetPosition().y - Application::cA_CurrentTerrainY) + (CurrentNPC->GetDimensions().y * 0.5f), CurrentNPC->GetPosition().z);
			camera->CurrentCameraRotation.x = 0;
			int buttonCount = 1;
			bool imdone = false;
			for (std::map<std::string, std::vector<int>>::iterator it = CurrentNPC->NPCcurrQstate.begin(); it != CurrentNPC->NPCcurrQstate.end(); ++it) // go thru the NPC's states
			{
				for (std::map<std::string, int>::iterator it2 = Scene_System::accessing().gPlayer->playerCurrQState.begin(); it2 != Scene_System::accessing().gPlayer->playerCurrQState.end(); ++it2) // go thru the player's states
				{
					if (it->first == it2->first) // compare the same quests
					{
						//loop thru the NPC's vector of quest stages to offer
						for (std::vector<int>::iterator it3 = it->second.begin(); it3 != it->second.end(); ++it3)
						{
							if (*it3 == it2->second)
							{
								for (std::vector<Quest*>::iterator it4 = Scene_System::accessing().QM.allQuests.begin(); it4 != Scene_System::accessing().QM.allQuests.end(); ++it4)
								{
									Quest* test = *it4;
									if (test->getName() == it->first)
									{
										for (std::map<std::string, int>::iterator it5 = Scene_System::accessing().gPlayer->playerCurrQState.begin(); it5 != Scene_System::accessing().gPlayer->playerCurrQState.end(); ++it5)
										{
											if (it5->first == test->getName())
											{
												if (test->getActive())
												{
													if (it5->second > 0)
													{
														if (test->qStages.at(it5->second - 1)->getComplete())
														{
															imdone = true;
														}
													}
												}
											}
											if (it5->first == test->preReq && it5->second >= test->preReqVal)
											{
												for (std::vector<QuestStage*>::iterator it6 = test->qStages.begin(); it6 != test->qStages.end(); ++it6)
												{
													QuestStage* temp2 = *it6;
													if (((temp2->getGiver() == CurrentNPC->getName()) && (buttonCount <= 3)))
													{
														if (imdone || it2->second == 0)
														{
															NPC_QuestButtons.at(buttonCount)->UI_Text = it->first;
															buttonCount++;
															break;
														}
													}
													else
														break;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			while (buttonCount <= 3)
			{
				NPC_QuestButtons.at(buttonCount)->UI_Text = "";
				buttonCount++;
			}

			//Interacting with NPC: Check UI Key Press
			std::string temp = HandleChatUIInput((float)dt);
			if (temp == "Exit")
			{
				camera->CameraIsLocked = false;
				if (ChatLayer->LayerTargetPosition.y > -1)
					ChatLayer->SwapOriginalWithTarget();
				CurrentNPC->setInteracting(false);
				Scene_System::accessing().cSS_InputManager->SetMouseToScreenCenter();
				Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = false;
				Scene_System::accessing().cSS_PlayerUIManager->CurrentState = PlayerUIManager::UIS_HUD;
				temp.clear();
			}
			bool changedFText = false;
			for (std::vector<UI_Element*>::iterator it = NPC_QuestButtons.begin(); it != NPC_QuestButtons.end(); ++it)
			{
				UI_Element* dat = *it;
				dat->UI_Bounds->ResetValues();
				if (dat->UI_Bounds->CheckCollision(Scene_System::accessing().cSS_InputManager->GetMousePosition()))
				{
					for (std::vector<Quest*>::iterator it2 = Scene_System::accessing().QM.allQuests.begin(); it2 != Scene_System::accessing().QM.allQuests.end(); ++it2)
					{
						Quest* dis = *it2;
						for (std::map<std::string, int>::iterator it3 = Scene_System::accessing().gPlayer->playerCurrQState.begin(); it3 != Scene_System::accessing().gPlayer->playerCurrQState.end(); ++it3)
						{
							if (it3->first == dat->UI_Text && dis->getName() == it3->first)
							{
								NPC_TextBox->UI_Text = dis->qStages.at(it3->second)->getDesc();
								NPC_TextBox->WrapText();
								changedFText = true;
								break;
							}
						}
					}
				}
				else if (changedFText == false)
				{
					NPC_TextBox->UI_Text = CurrentNPC->getFText();
					NPC_TextBox->WrapText();
				}
				else
					break;
			}
			for (std::vector<Quest*>::iterator it = Scene_System::accessing().QM.allQuests.begin(); it != Scene_System::accessing().QM.allQuests.end(); ++it)
			{
				Quest* dis = *it;
				if (temp == dis->getName())
				{
					if (!dis->getActive())
					{
						dis->setActive(true);
					}
					int temp2 = dis->getCurrentStage();
					dis->setCurrStage(temp2 + 1);
					for (std::map<std::string, int>::iterator it2 = Scene_System::accessing().gPlayer->playerCurrQState.begin(); it2 != Scene_System::accessing().gPlayer->playerCurrQState.end(); ++it2)
					{
						if (it2->first == dis->getName())
						{
							it2->second++;
						}
					}
					camera->CameraIsLocked = false;
					if (ChatLayer->LayerTargetPosition.y > -1)
						ChatLayer->SwapOriginalWithTarget();
					CurrentNPC->setInteracting(false);
					Scene_System::accessing().cSS_InputManager->SetMouseToScreenCenter();
					Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = false;
					Scene_System::accessing().cSS_PlayerUIManager->CurrentState = PlayerUIManager::UIS_HUD;
					temp.clear();
				}
			}
		}
	}
}

void SceneTown3::Update(float dt)
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	SceneGraphics->Update(dt);
    MusicSystem::accessing().playBackgroundMusic("town3");

	//Update Camera's Minimum Possible & Current Y Pos
	Application::cA_MinimumTerrainY = TerrainScale.y * ReadHeightMap(m_heightMap, camera->position.x / TerrainScale.x, camera->position.z / TerrainScale.z) + camera->PlayerHeight;

	if (!(Application::cA_CurrentTerrainY - Application::cA_MinimumTerrainY <= Math::EPSILON && Application::cA_MinimumTerrainY - Application::cA_CurrentTerrainY <= Math::EPSILON))
	{
		float RateofChangeY = (Application::cA_CurrentTerrainY - Application::cA_MinimumTerrainY) * (float)dt * (camera->CameraCurrentWalkSpeed / 3);
		if (Application::cA_CurrentTerrainY - RateofChangeY >= Application::cA_MinimumTerrainY || Application::cA_CurrentTerrainY - RateofChangeY <= Application::cA_MinimumTerrainY)
		{
			Application::cA_CurrentTerrainY -= RateofChangeY;
		}
	}
	Vector3 Center(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / 2, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight / 2, 0);

	framerates = 1 / dt;
    PlayerObject* PlayerPTR = dynamic_cast<PlayerObject*>(Player);
    if (Scene_System::accessing().whatLoadingState == Scene_System::FINISHED_LOADING || Scene_System::accessing().whatLoadingState == Scene_System::NOT_LOADING)
    {
        if (Scene_System::accessing().cSS_InputManager->GetKeyValue('1'))
        {
            Scene_System::accessing().SwitchScene(SceneTown1::id_);
        }
        if (Scene_System::accessing().cSS_InputManager->GetKeyValue('2'))
        {
            Scene_System::accessing().SwitchScene(SceneTown2::id_);
        }
        if (Scene_System::accessing().cSS_InputManager->GetKeyValue('4'))
        {
            Scene_System::accessing().SwitchScene(SceneFreeField::id_);
        }
        if (Scene_System::accessing().cSS_InputManager->GetKeyValue('5'))
        {
			Scene_System::accessing().SwitchScene(SceneFreeField2::id_);
        }
        if (Scene_System::accessing().cSS_InputManager->GetKeyValue('9'))
        {
            Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = false;
            Scene_System::accessing().cSS_InputManager->cIM_CameraPitch = 0;
            Scene_System::accessing().cSS_InputManager->cIM_CameraYaw = 0;
        }
        if (Scene_System::accessing().cSS_InputManager->GetKeyValue('0'))
        {
            Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = true;
        }

        BManager.UpdateContainer(dt, camera->position);
    }
    else {
        camera->CameraIsLocked = true;
        PlayerPTR->SetVelocity(Vector3(0, 0, 0));
    }
    PlayerPTR->Update(dt);
	PlayerPTR->SetRotationAngle(camera->CurrentCameraRotation.y);

	camera->position = PlayerPTR->GetPosition();
	camera->Update(dt);
    Scene_System::accessing().UpdateLoadingStuff(dt);
    if (transitingSceneName != "" && Scene_System::accessing().whatLoadingState == Scene_System::FINISHED_LOADING)
    {
        camera->CameraIsLocked = false;
        onNotify("TRANSITIONING");
    }
}

void SceneTown3::RenderTerrain()
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	modelStack->PushMatrix();
	modelStack->Scale(TerrainScale.x, TerrainScale.y, TerrainScale.z);
	SceneGraphics->RenderMesh("Town 3", true);
	modelStack->PopMatrix();
}

void SceneTown3::RenderShadowCasters()
{
	RenderTerrain();
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());

	// will remove soon 
	for (auto it : objVec)
	{
		GameObject *the3DObject = dynamic_cast<GameObject*>(it);
		if (the3DObject && (camera->position - camera->target).Normalize().Dot(the3DObject->GetPosition().Normalized()) < 1.f)
			the3DObject->Render();
	}
	// will remove soon 

	for (std::vector<Billboard*>::iterator it = BManager.BillboardContainer.begin(); it != BManager.BillboardContainer.end(); ++it)
	{
		if ((*it)->Active)
		{
			float TimeRatio = 1;
			if ((*it)->GetLifeTime() != -1)
				TimeRatio = 1.1f - (*it)->GetCurrTime() / (*it)->GetLifeTime();
			modelStack->PushMatrix();
			modelStack->Translate((*it)->GetPosition().x, (*it)->GetPosition().y, (*it)->GetPosition().z);
			modelStack->Rotate(Math::RadianToDegree(atan2(camera->position.x - (*it)->GetPosition().x, camera->position.z - (*it)->GetPosition().z)), 0, 1, 0);
			modelStack->Scale(TimeRatio * (*it)->GetDimensions().x, TimeRatio *(*it)->GetDimensions().y, TimeRatio *(*it)->GetDimensions().z);
			if ((*it)->GetLifeTime() == -1)
				SceneGraphics->RenderMesh((*it)->GetMeshName(), true);
			else SceneGraphics->RenderMesh((*it)->GetMeshName(), false);
			modelStack->PopMatrix();
		}
	}

}

void SceneTown3::RenderSkybox()
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	//left
	modelStack->PushMatrix();
	modelStack->Rotate(90, 0, 1, 0);
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
	modelStack->PopMatrix();

}

void SceneTown3::RenderPassGPass()
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

void SceneTown3::RenderPassMain()
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
	SceneGraphics->SetHUD(false);

	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack->LoadMatrix(perspective);

	// Camera matrix
	viewStack->LoadIdentity();
	viewStack->LookAt(
		camera->position.x, camera->position.y, camera->position.z,
		camera->target.x, camera->target.y, camera->target.z,
		camera->up.x, camera->up.y, camera->up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack->LoadIdentity();

	//RenderTerrain();
	//RenderSkyplane();
	RenderSkybox();
	RenderShadowCasters();
	SceneGraphics->SetHUD(true);

	Scene_System::accessing().cSS_PlayerUIManager->Render();
	if (Scene_System::accessing().cSS_InputManager->cIM_inMouseMode)
	{
		SceneGraphics->RenderMeshIn2D("TFB_Gem", false, 100, 100, Scene_System::accessing().cSS_InputManager->GetMousePosition().x, Scene_System::accessing().cSS_InputManager->GetMousePosition().y);
	}
    if (Scene_System::accessing().theLoadingEffect)
        Scene_System::accessing().RenderLoadingStuff();
  
	SceneGraphics->SetHUD(false);
}

void SceneTown3::Render()
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

void SceneTown3::Exit()
{
	if (theInteractiveMap)
		delete theInteractiveMap;
	for (auto it : objVec)
	{
		if (it)
			delete it;
	}
	if (Player)
		delete Player;
    if (camera)
        delete camera;
}

bool SceneTown3::onNotify(const std::string &theEvent)
{
    if (checkWhetherTheWordInThatString("PLAYER_INFO", theEvent))
    {
        if (Scene_System::accessing().gPlayer->CurrCamera)
        {
            delete camera;
            camera = Scene_System::accessing().gPlayer->CurrCamera;
        }
        if (Scene_System::accessing().gPlayer->PlayerObj)
        {
            delete Player;
            Player = Scene_System::accessing().gPlayer->PlayerObj;
            PlayerObject *PlayerPTR = dynamic_cast<PlayerObject*>(Player);
            PlayerPTR->SetPosition(Vector3(Player->GetPosition().x, camera->PlayerHeight + TerrainScale.y * ReadHeightMap(m_heightMap, (Player->GetPosition().x / TerrainScale.x), (Player->GetPosition().z / TerrainScale.z)), Player->GetPosition().z));
            PlayerPTR->setPlayerBoundaries(objVec);
        }
        return true;
    }
    else if (checkWhetherTheWordInThatString("TRANSITIONING", theEvent))
    {
        Scene_System::accessing().SwitchScene(transitingSceneName);
        std::ostringstream ss;
        ss << "SWITCHING_" << id_;
        transitingSceneName = "";
        Scene_System::accessing().getCurrScene().onNotify(ss.str());
        return true;
    }
    else if (checkWhetherTheWordInThatString("LOADING", theEvent))
    {
        Scene_System::accessing().SetLoadingTime(3.0);
        size_t posOfUnderScore = theEvent.find_first_of('_');
        transitingSceneName = theEvent.substr(posOfUnderScore + 1);
        return true;
    }
    else if (checkWhetherTheWordInThatString("SWITCHING", theEvent))
    {
        PlayerObject *PlayerPTR = Scene_System::accessing().gPlayer->PlayerObj = dynamic_cast<PlayerObject*>(Player);
        PlayerPTR->SetVelocity(Vector3(0, 0, 0));
        Scene_System::accessing().gPlayer->CurrCamera = camera;
        size_t posOfUnderScore = theEvent.find_first_of('_');
        std::string preSceneId = theEvent.substr(posOfUnderScore + 1);
        for (std::vector<GameObject*>::iterator it = objVec.begin(), end = objVec.end(); it != end; ++it)
        {
            if (checkWhetherTheWordInThatString(preSceneId, (*it)->getName()))
            {
                Vector3 theGatePos = (*it)->GetPosition();
                Vector3 theDirectionalPosBetweenPlayerGate = (PlayerPTR->GetPosition() - theGatePos).Normalize();
                theDirectionalPosBetweenPlayerGate *= (((*it)->GetDimensions().x + (*it)->GetDimensions().y) * 0.5f);
                PlayerPTR->SetPosition(theGatePos + theDirectionalPosBetweenPlayerGate);
                break;
            }
        }
        Scene_System::accessing().gPlayer->currSceneID = id_;
        return true;
    }
    return false;
}