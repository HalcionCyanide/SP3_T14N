#include "Scene_MainMenu.h"
#include <sstream>

#include "Scene_2.h"
#include "SceneTown1.h"
#include "SceneTown2.h"
#include "SceneTown3.h"
#include "SceneFreeField.h"
#include "SceneBattleScreen.h"

#include "..\\Classes\\GameMap.h"
#include "..\\Classes\\GameObject.h"
#include "../Misc/SimpleCommand.h"
#include "../Misc/LoadEnemyData.h"
#include <climits>
#include "../Systems/MusicSystem.h"

std::string Scene_MainMenu::id_ = "M_Scene";

const std::string Scene_MainMenu::UI_Text[17] = { "", "Start", "Settings", "Exit", "New Game", "Load Game", "Return", "Forward_Button", "Backward_Button", "Right_Button", "Left_Button", "Jump_Button", "Press Any Keys to Change the command", "Load Save 1", "Load Save 2", "Load Save 3", "Back" };

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
	perspective.SetToPerspective(45.0f, Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight, 0.1f, 10000.0f);
	projectionStack->LoadMatrix(perspective);

	// Initiallise Model Specific Meshes Here
	Mesh* newMesh = MeshBuilder::GenerateTerrain("terrain", "HeightMapFiles//heightmap_MainMenu.raw", m_heightMap);
	newMesh->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	newMesh->textureArray[0] = LoadTGA("Image//RockTex.tga");
	newMesh->textureArray[1] = LoadTGA("Image//GrassStoneTex.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	Application::cA_MinimumTerrainY = TerrainScale.y * ReadHeightMap(m_heightMap, camera.position.x / TerrainScale.x, camera.position.z / TerrainScale.z) + camera.PlayerHeight;
	Application::cA_CurrentTerrainY = Application::cA_MinimumTerrainY;

	camera.Init(Vector3(-0.5f, Application::cA_MinimumTerrainY + 5, -40.f), Vector3(0, Application::cA_MinimumTerrainY + 15, -35), Vector3(0, 1, 0));
	camera.CameraIsLocked = true;

	for (int i = 0; i < 15; i++)
	{
		BManager.AddHMapBillboard("Tree", m_heightMap, TerrainScale, Vector3((float)i * Math::RandFloatMinMax(-10.f, 10.f) + 40.f, 0.f, 25.f + Math::RandFloatMinMax(-5.f, 10.f)), Vector3(10.f, 20.f, 10.f), Vector3(), camera.position);
		BManager.AddHMapBillboard("Tree", m_heightMap, TerrainScale, Vector3((float)i * Math::RandFloatMinMax(-10.f, 10.f) + 50.f, 0.f, 47.f + Math::RandFloatMinMax(-5.f, 10.f)), Vector3(10.f, 20.f, 10.f), Vector3(), camera.position);
	}

	CurrentMenuState = S_FIRSTLEVEL;
	Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = true;
	InitSceneUIElems();

    transitingSceneName = "";
}

void Scene_MainMenu::InitSceneUIElems()
{
	UI_Layer* NewL = new UI_Layer();
	Vector3 ButtonScale(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.20f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.1f, 1); Vector3 CenterPosition(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.5f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);
	NewL->AddUIElement("TFB_Logo", CenterPosition * 3, CenterPosition * 3, Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.55f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight* 0.9f, 1), CenterPosition * 1.35f);

	NewL->AddUIElement("TFB_Button", CenterPosition * -2.f, CenterPosition * -2.f, ButtonScale, Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 1.2f, 0), UI_Text[1]);
	NewL->AddUIElement("TFB_Button", CenterPosition * -2.5f, CenterPosition * -2.5f, ButtonScale, Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.9f, 0), UI_Text[2]);
	NewL->AddUIElement("TFB_Button", CenterPosition * -3.f, CenterPosition * -3.f, ButtonScale, Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.6f, 0), UI_Text[3]);

	NewL->AddUIElement("TFB_Button", Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 1.2f, 0), Vector3(0, CenterPosition.y * 3.f, 0), ButtonScale, Vector3(0, CenterPosition.y * 3.f, 0), UI_Text[4]);
	NewL->AddUIElement("TFB_Button", Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.9f, 0), Vector3(0, CenterPosition.y * 3.f, 0), ButtonScale, Vector3(0, CenterPosition.y * 3.f, 0), UI_Text[5]);
	NewL->AddUIElement("TFB_Button", Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.6f, 0), Vector3(0, CenterPosition.y * 3.f, 0), ButtonScale, Vector3(0, CenterPosition.y * 3.f, 0), UI_Text[6]);

    //For Setting Stuff
    std::ostringstream ss;
    ss << UI_Text[7] << ": " << SimpleCommand::m_allTheKeys[SimpleCommand::FORWARD_COMMAND];
    NewL->AddUIElement("TFB_Button", Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 1.8f, 0), Vector3(0, CenterPosition.x * 3.f, 0), ButtonScale, Vector3(0, CenterPosition.y * 3.f, 0), ss.str());

    ss.str("");
    ss << UI_Text[8] << ": " << SimpleCommand::m_allTheKeys[SimpleCommand::BACK_COMMAND];
	NewL->AddUIElement("TFB_Button", Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 1.5f, 0), Vector3(0, CenterPosition.x * 3.f, 0), ButtonScale, Vector3(0, CenterPosition.y * 3.f, 0), ss.str());

    ss.str("");
    ss << UI_Text[9] << ": " << SimpleCommand::m_allTheKeys[SimpleCommand::RIGHT_COMMAND];
	NewL->AddUIElement("TFB_Button", Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 1.2f, 0), Vector3(0, CenterPosition.x * 3.f, 0), ButtonScale, Vector3(0, CenterPosition.y * 3.f, 0), ss.str());

    ss.str("");
    ss << UI_Text[10] << ": " << SimpleCommand::m_allTheKeys[SimpleCommand::LEFT_COMMAND];
	NewL->AddUIElement("TFB_Button", Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.9f, 0), Vector3(0, CenterPosition.x * 3.f, 0), ButtonScale, Vector3(0, CenterPosition.y * 3.f, 0), ss.str());

    ss.str("");
    ss << UI_Text[11] << ": " << SimpleCommand::m_allTheKeys[SimpleCommand::JUMP_COMMAND];
	NewL->AddUIElement("TFB_Button", Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.3f, 0), Vector3(0, CenterPosition.y * 3.f, 0), ButtonScale, Vector3(0, CenterPosition.y * 3.f, 0), ss.str());
    whatKeyToChange = "";

	NewL->AddUIElement("TFB_Button", Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.3f, 0), Vector3(0, CenterPosition.y * 3.f, 0), ButtonScale, Vector3(500, CenterPosition.y * 3.f, 0), UI_Text[12]);
    //For Setting Stuff

    //For Loading Save Stuff
    NewL->AddUIElement("TFB_Button", Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 1.5f, 0), Vector3(0, CenterPosition.y * 3.f, 0), ButtonScale, Vector3(500, CenterPosition.y * 3.f, 0), UI_Text[13]);
    NewL->AddUIElement("TFB_Button", Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 1.2f, 0), Vector3(0, CenterPosition.y * 3.f, 0), ButtonScale, Vector3(500, CenterPosition.y * 3.f, 0), UI_Text[14]);
    NewL->AddUIElement("TFB_Button", Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.9f, 0), Vector3(0, CenterPosition.y * 3.f, 0), ButtonScale, Vector3(500, CenterPosition.y * 3.f, 0), UI_Text[15]);
    NewL->AddUIElement("TFB_Button", Vector3(CenterPosition.x * 0.5f, CenterPosition.y * 0.6f, 0), Vector3(0, CenterPosition.y * 3.f, 0), ButtonScale, Vector3(500, CenterPosition.y * 3.f, 0), UI_Text[16]);
    //For Loading Save Stuff

	UI_Sys.cUIS_LayerContainer.push_back(NewL);
}

void Scene_MainMenu::UpdateUILogic(float dt, Scene_MainMenu::STATE_MAIN_MENU cState)
{
    bool ClickSucceeded = false;
    for (std::vector<UI_Layer*>::iterator it = UI_Sys.cUIS_LayerContainer.begin(); it != UI_Sys.cUIS_LayerContainer.end(); ++it)
    {
        for (std::vector<UI_Element*>::iterator it2 = (*it)->cUI_Layer.begin(); it2 != (*it)->cUI_Layer.end(); ++it2)
        {
            if ((*it2)->Active)
            {
                bool ClickSucceeded = false;
                if (CurrentMenuState == S_FIRSTLEVEL)
                {
                    (*it2)->BoundsActive = true;
                    if (((*it2)->UI_Text == UI_Text[1] || (*it2)->UI_Text == UI_Text[2] || (*it2)->UI_Text == UI_Text[3]))
                    {
						(*it2)->CheckInput(Scene_System::accessing().cSS_InputManager->GetMousePosition(), ClickSucceeded);
						if (ClickSucceeded)
                        {
                            if (((*it2)->UI_Text == UI_Text[1]))
                            {
                                // Start
                                CurrentMenuState = S_SECONDLEVEL;
                                for (std::vector<UI_Element*>::iterator it2 = (*it)->cUI_Layer.begin(); it2 != (*it)->cUI_Layer.end(); ++it2)
                                {
                                    if (((*it2)->UI_Text == UI_Text[1] || (*it2)->UI_Text == UI_Text[2] || (*it2)->UI_Text == UI_Text[3]))
                                    {
                                        (*it2)->SwapOriginalWithTarget();
                                    }
                                    else if (((*it2)->UI_Text == UI_Text[4] || (*it2)->UI_Text == UI_Text[5] || (*it2)->UI_Text == UI_Text[6]))
                                    {
                                        (*it2)->SwapOriginalWithTarget();
                                    }
                                }
                            }
                            else if (((*it2)->UI_Text == UI_Text[2]))
                            {
                                // Settings
                                CurrentMenuState = S_SETTING;
                                //There can a potential bug with this!
                                for (std::vector<UI_Element*>::iterator it3 = (*it)->cUI_Layer.begin(); it3 != (*it)->cUI_Layer.end(); ++it3)
                                {
                                    if (((*it3)->UI_Text == UI_Text[1] || (*it3)->UI_Text == UI_Text[2] || (*it3)->UI_Text == UI_Text[3]) ||    // Main Menu 
                                        (*it3)->UI_Text == UI_Text[6] || checkWhetherTheWordInThatString(UI_Text[7], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[8], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[9], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[10], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[11], (*it3)->UI_Text))   //Setting Stuff
                                    {
                                        (*it3)->SwapOriginalWithTarget();
                                    }
                                }
                                //There can a potential bug with this!
                            }
                            else if (((*it2)->UI_Text == UI_Text[3]))
                            {
                                // Exit2
                                Application::ExitGame = true;
                            }
							break;
                        }
                    }
                }
                else if (CurrentMenuState == S_SECONDLEVEL)
                {
                    if (((*it2)->UI_Text == UI_Text[4] || (*it2)->UI_Text == UI_Text[5] || (*it2)->UI_Text == UI_Text[6]))
                    {
                        (*it2)->BoundsActive = true;
						(*it2)->CheckInput(Scene_System::accessing().cSS_InputManager->GetMousePosition(), ClickSucceeded);
                        if (ClickSucceeded)
                        {
                            if (((*it2)->UI_Text == UI_Text[4]))
                            {
                                // Start
                                //<!> the most hardcoding method ever!
                                transitingSceneName = "DrivenFiles//NewPlayerSave.csv";
                                Scene_System::accessing().SetLoadingTime(3.f);
                            }
                            else if (((*it2)->UI_Text == UI_Text[5]))
                            {
                                // Load
                                CurrentMenuState = S_LOADING_SAVE;
                                for (std::vector<UI_Element*>::iterator it3 = (*it)->cUI_Layer.begin(); it3 != (*it)->cUI_Layer.end(); ++it3)
                                {
                                    if (((*it3)->UI_Text == UI_Text[4] || (*it3)->UI_Text == UI_Text[5] || (*it3)->UI_Text == UI_Text[6]) || //Second Layer Stuff
                                        checkWhetherTheWordInThatString(UI_Text[13], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[14], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[15], (*it3)->UI_Text) || (*it3)->UI_Text ==  UI_Text[16])   //Loading Saves Stuff
                                    {
                                        (*it3)->SwapOriginalWithTarget();
                                    }
                                }
                            }
                            else if (((*it2)->UI_Text == UI_Text[6]))
                            {
                                // Return
                                CurrentMenuState = S_FIRSTLEVEL;
                                for (std::vector<UI_Element*>::iterator it2 = (*it)->cUI_Layer.begin(); it2 != (*it)->cUI_Layer.end(); ++it2)
                                {
                                    if (((*it2)->UI_Text == UI_Text[4] || (*it2)->UI_Text == UI_Text[5] || (*it2)->UI_Text == UI_Text[6]))
                                    {
                                        (*it2)->SwapOriginalWithTarget();
                                    }
                                    else if (((*it2)->UI_Text == UI_Text[1] || (*it2)->UI_Text == UI_Text[2] || (*it2)->UI_Text == UI_Text[3]))
                                    {
                                        (*it2)->SwapOriginalWithTarget();
                                    }
                                }
                            }
							break;
                        }
                    }
                }
                //Updating of Settings
                else if (CurrentMenuState == S_SETTING)
                {
                    if ((*it2)->UI_Text == UI_Text[6] || checkWhetherTheWordInThatString(UI_Text[7], (*it2)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[8], (*it2)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[9], (*it2)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[10], (*it2)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[11], (*it2)->UI_Text))   //Setting Stuff
                    {
						(*it2)->CheckInput(Scene_System::accessing().cSS_InputManager->GetMousePosition(), ClickSucceeded);
                        if (ClickSucceeded)
                        {
                            if ((*it2)->UI_Text == UI_Text[6])
                            {
                                // Return
                                CurrentMenuState = S_FIRSTLEVEL;
                                for (std::vector<UI_Element*>::iterator it3 = (*it)->cUI_Layer.begin(); it3 != (*it)->cUI_Layer.end(); ++it3)
                                {
                                    if (((*it3)->UI_Text == UI_Text[1] || (*it3)->UI_Text == UI_Text[2] || (*it3)->UI_Text == UI_Text[3]) ||    // Main Menu 
                                        (*it3)->UI_Text == UI_Text[6] || checkWhetherTheWordInThatString(UI_Text[7], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[8], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[9], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[10], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[11], (*it3)->UI_Text))   //Setting Stuff
                                    {
                                        (*it3)->SwapOriginalWithTarget();
                                    }
                                }
                            }
                            else 
							{
                                for (unsigned num = 7; num <= 11; ++num)
                                {
                                    if (checkWhetherTheWordInThatString(UI_Text[num], (*it2)->UI_Text))
                                    {
                                        whatKeyToChange = UI_Text[num];
                                        CurrentMenuState = S_UPDATING_KEYS;
                                        for (std::vector<UI_Element*>::iterator it3 = (*it)->cUI_Layer.begin(); it3 != (*it)->cUI_Layer.end(); ++it3)
                                        {
                                            if ((*it3)->UI_Text == UI_Text[6] || checkWhetherTheWordInThatString(UI_Text[7], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[8], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[9], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[10], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[11], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[12], (*it3)->UI_Text))   //Setting Stuff
                                            {
                                                (*it3)->SwapOriginalWithTarget();
                                            }
                                        }
                                    }
                                }
                            }

							break;
                        }
                    }
                }
                //Updating of the keys
                else if (CurrentMenuState == S_UPDATING_KEYS)
				{
                    for (unsigned char theChar = 0; theChar < UCHAR_MAX; ++theChar)
                    {
                        if (Application::IsKeyPressed(theChar) && !Application::IsKeyPressed(VK_LBUTTON))
                        {
                            CurrentMenuState = S_SETTING;
                            theChar = toupper(theChar);
                            writeToGlobalDrivenAndChangeCommand(theChar, whatKeyToChange);
                            Application::loadThoseKeyCommandsStuff();
                            for (std::vector<UI_Element*>::iterator it3 = (*it)->cUI_Layer.begin(); it3 != (*it)->cUI_Layer.end(); ++it3)
                            {
                                if ((*it3)->UI_Text == UI_Text[6] || checkWhetherTheWordInThatString(UI_Text[7], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[8], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[9], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[10], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[11], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[12], (*it3)->UI_Text))   //Setting Stuff
                                {
                                    (*it3)->SwapOriginalWithTarget();
                                    if (checkWhetherTheWordInThatString(whatKeyToChange, (*it3)->UI_Text))
                                    {
                                        std::ostringstream ss2;
                                        ss2 << whatKeyToChange << ":" << theChar;
                                        (*it3)->UI_Text = ss2.str();
                                    }
                                }
                            }
                            break;
                        }
                    }
                }
                //Updating of the keys
                //Loading of Save Stuff
                else if (CurrentMenuState == S_LOADING_SAVE)
                {
                    if (checkWhetherTheWordInThatString(UI_Text[13], (*it2)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[14], (*it2)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[15], (*it2)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[16], (*it2)->UI_Text))   //Loading Saves Stuff
                    {
						(*it2)->CheckInput(Scene_System::accessing().cSS_InputManager->GetMousePosition(), ClickSucceeded);
                        if (ClickSucceeded)
                        {
                            if ((*it2)->UI_Text == UI_Text[16])
                            {
                                CurrentMenuState = S_SECONDLEVEL;
                                for (std::vector<UI_Element*>::iterator it3 = (*it)->cUI_Layer.begin(); it3 != (*it)->cUI_Layer.end(); ++it3)
                                {
                                    if (((*it3)->UI_Text == UI_Text[4] || (*it3)->UI_Text == UI_Text[5] || (*it3)->UI_Text == UI_Text[6]) || //Second Layer Stuff
                                        checkWhetherTheWordInThatString(UI_Text[13], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[14], (*it3)->UI_Text) || checkWhetherTheWordInThatString(UI_Text[15], (*it3)->UI_Text) || UI_Text[16] == (*it3)->UI_Text)   //Loading Saves Stuff
                                    {
                                        (*it3)->SwapOriginalWithTarget();
                                    }
                                }
                            }
                            else
                            {
                                for (unsigned num = 13, beginningNum = num - 1; num <= 15; ++num)
                                {
                                    if ((*it2)->UI_Text == UI_Text[num])
                                    {
                                        std::ostringstream ss;
                                        ss << num - beginningNum;
                                        transitingSceneName = ss.str();
                                        Scene_System::accessing().SetLoadingTime(3.f);
                                    }
                                }
                            }
							break;
                        }
                    }
                }
                //Loading of Save Stuff
     }
        }
    }

    //For Loading Screen
    if (transitingSceneName != "" && Scene_System::accessing().whatLoadingState == Scene_System::FINISHED_LOADING)
    {
        if (CurrentMenuState == S_LOADING_SAVE)
        {
            Scene_System::accessing().gPlayer->settingTheFileToSave(stoi(transitingSceneName));
            Scene_System::accessing().gPlayer->automaticallyLoadFile();
        }
        else {
            Scene_System::accessing().whatLoadingState = Scene_System::NOT_LOADING;
            Scene_System::accessing().gPlayer->LoadPlayerSave(transitingSceneName);
        }
        Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = false;
        transitingSceneName = "";
    }
}

void Scene_MainMenu::Update(float dt)
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	SceneGraphics->Update(dt);
    MusicSystem::accessing().playBackgroundMusic("MainMenuAlt");

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
	else if (Scene_System::accessing().cSS_InputManager->GetKeyValue('0'))
	{
		Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = true;
	}

	UpdateUILogic(dt, CurrentMenuState);
	UI_Sys.Update(dt);
	BManager.UpdateContainer(dt, camera.position);
	camera.Update(dt);
    Scene_System::accessing().UpdateLoadingStuff(dt);
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
	SceneGraphics->SetHUD(false);

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
		if (the3DObject && (camera.position - camera.target).Normalize().Dot(the3DObject->GetPosition().Normalized()) < 1.f)
			the3DObject->Render();
	}
	//<!> will remove soon <!>

	SceneGraphics->SetHUD(true);

	UI_Sys.Render();

	if (Scene_System::accessing().cSS_InputManager->cIM_inMouseMode)
	{
		SceneGraphics->RenderMeshIn2D("TFB_Gem", false, 100, 100, Scene_System::accessing().cSS_InputManager->GetMousePosition().x, Scene_System::accessing().cSS_InputManager->GetMousePosition().y);
	}

    if (Scene_System::accessing().theLoadingEffect)
        Scene_System::accessing().RenderLoadingStuff();

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

void Scene_MainMenu::writeToGlobalDrivenAndChangeCommand(const unsigned char &command, const std::string &theKey)
{
    std::ifstream file("DrivenFiles//GlobalDriven.csv");
    if (file.is_open())
    {
        std::vector<std::string> allTheLines;
        std::string data = "";
        while (getline(file, data))
            allTheLines.push_back(data);
        file.close();

        std::ofstream writeFile("DrivenFiles//GlobalDriven.csv");
        for (std::vector<std::string>::iterator it = allTheLines.begin(), end = allTheLines.end(); it != end; ++it)
        {
            if (checkWhetherTheWordInThatString(theKey, (*it)))
            {
                writeFile << theKey << "," << command << std::endl;
            }
            else
                writeFile << (*it) << std::endl;
        }
        writeFile.close();
    }
}