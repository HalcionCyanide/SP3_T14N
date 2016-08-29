#include "Scene_System.h"
#ifdef _DEBUG
    #include <assert.h>
#endif

void Scene_System::Init()
{
    //currScene = nullptr;
    graphics_scene = nullptr;
	cSS_InputManager = new InputManager();
	gPlayer = new GlobalPlayer(1, 100, 100, false);

	QM.readFile("DrivenFiles//questData.csv");

	std::string path = "DrivenFiles//quests//q_";
	std::string fileFormat = ".csv";
	for (auto it : QM.allQuests)
	{
		std::string temp = path + it->getName() + fileFormat;
		it->LoadFile(temp);
	}
	NM.LoadFile("DrivenFiles//NPC_FILES.csv");
	BSys = nullptr;

	for (auto it : QM.allQuests)
	{
		gPlayer->playerCurrQState.insert(std::pair<std::string, int>(it->getName(), 0));
	}
	std::vector<int> tempStates;
	for (auto it : NM.allNPCs) //iterate through all NPCS
	{
		for (auto it2 : QM.allQuests) //iterate through all Quests
		{
			for (auto it3 : it2->qStages) //iterate through the Individual Quests' stages.
			{
				if (it3->getGiver() == it->getName()) //if the Stages' giver is the NPC...
				{
					tempStates.push_back(it3->getStageNO());
				}
			}
			it->NPCcurrQstate.insert(std::pair<std::string, std::vector<int>>(it2->getName(), tempStates));
			tempStates.clear();
		}
	}
    theLoadingEffect = nullptr;
    delayingLoadingTime = m_accumulatedLoadingTime = 0;
    prevLoadingState = BEGIN_LOADING;  
    whatLoadingState = FINISHED_LOADING;
    hasLoadingEnded = true;
}

void Scene_System::Update(double dt)
{
#ifdef _DEBUG
    assert(dt > 0 || dt < 0 || dt == 0);    //Make there is no joker
#endif
}

void Scene_System::AddScene(SceneEntity &sceneObject)
{
#ifdef _DEBUG
    assert(graphics_scene != nullptr);  //Ensuring the Graphics are being added before the logics
#endif
	if (SceneHistory.empty() == true)
    {
		SceneHistory.push(&sceneObject);
        graphics_scene->modelStack = sceneObject.modelStack;
        graphics_scene->viewStack = sceneObject.viewStack;
        graphics_scene->projectionStack = sceneObject.projectionStack;
    }
    theSceneList.insert(std::pair<std::string, SceneEntity*>(sceneObject.getName(), &sceneObject));
}

void Scene_System::RemoveScene(SceneEntity &sceneObject)
{
    // Don't know what to do with this function
}

void Scene_System::SwitchScene(const std::string &id_)
{
#ifdef _DEBUG
    assert(graphics_scene != nullptr);  //Ensuring the Graphics are being added before the logics
#endif
    std::map<std::string, SceneEntity*>::iterator it = theSceneList.find(id_);
	if (it != theSceneList.end())
	{
		SceneHistory.push(it->second);
		graphics_scene->modelStack = SceneHistory.top()->modelStack;
		graphics_scene->viewStack = SceneHistory.top()->viewStack;
		graphics_scene->projectionStack = SceneHistory.top()->projectionStack;
	}
}

bool Scene_System::SwitchToPreviousScene()
{
	if (SceneHistory.size() > 1)
	{
		SceneHistory.pop();
		graphics_scene->modelStack = SceneHistory.top()->modelStack;
		graphics_scene->viewStack = SceneHistory.top()->viewStack;
		graphics_scene->projectionStack = SceneHistory.top()->projectionStack;
		return true;
	}
	else return false;
}

SceneEntity &Scene_System::getCurrScene()
{
	return *SceneHistory.top();
}

SceneEntity &Scene_System::getGraphicsScene()
{
    return *graphics_scene;
}

void Scene_System::setGraphics_Scene(SceneEntity &sceneObject)
{
#ifdef _DEBUG
    assert(graphics_scene == nullptr);  //Ensuring the Graphics are being added before the logics
#endif
    graphics_scene = &sceneObject;
}

void Scene_System::clearEverything()
{
    if (gPlayer)
        delete gPlayer;
    gPlayer = nullptr;
    for (auto it : theSceneList)
    {
        it.second->Exit();
        delete it.second;
    }
    theSceneList.clear();
    graphics_scene->Exit();
    delete graphics_scene;
    graphics_scene = nullptr;

	if (cSS_InputManager)
		delete cSS_InputManager;
	
	while (!SceneHistory.empty())
	{
		SceneHistory.pop();
	}
    for (auto it : EnemyData)
    {
        delete it.second;
    }
    EnemyData.clear();

    for (auto it : QM.allQuests)
    {
        delete it;
    }
    QM.allQuests.clear();

	for (auto it : NM.allNPCs)
	{
		delete it;
	}
	NM.allNPCs.clear();

	if (BSys)
		delete BSys;
    if (theLoadingEffect)
        delete theLoadingEffect;
}

void Scene_System::doingLoadingEffect()
{
    theLoadingEffect = new UI_System(); 
    UI_Layer *theLayer = new UI_Layer();
    Vector3 sizeofSquare(cSS_InputManager->cIM_ScreenWidth * 0.1f, cSS_InputManager->cIM_ScreenWidth * 0.1f, 1);
    //theLayer->AddUIElement(UI_Element::UI_BUTTON_T_TO_SCRN, "SmallWhiteSquare", Vector3(400, 300, 0), Vector3(400, 300, 0), sizeofSquare, Vector3(-400, 300, 0));
    int rowsOfSquares = ((int)ceil((int)ceil(cSS_InputManager->cIM_ScreenHeight) / (int)ceil(sizeofSquare.y)) * 2) + 1;
    int colsOfSquares = ((int)ceil((int)ceil(cSS_InputManager->cIM_ScreenWidth) / (int)ceil(sizeofSquare.x)) * 2) + 1;
    Vector3 leftSide(-sizeofSquare.x, cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);
    Vector3 botSide(cSS_InputManager->cIM_ScreenWidth * 0.5f, -sizeofSquare.y, 0);
    Vector3 rightSide(cSS_InputManager->cIM_ScreenWidth + sizeofSquare.x, cSS_InputManager->cIM_ScreenHeight * 0.5f);
    Vector3 topSide(cSS_InputManager->cIM_ScreenWidth * 0.5f, cSS_InputManager->cIM_ScreenHeight + sizeofSquare.y);
    Vector3 *whichTarget = nullptr;
    for (int numOFRows = 0; numOFRows < rowsOfSquares; ++numOFRows)
    {
        for (int numOfCols = 0; numOfCols < colsOfSquares; ++numOfCols)
        {
            if (numOfCols % 4 == 1)
                whichTarget = &botSide;
            else if (numOfCols % 4 == 2)
                whichTarget = &rightSide;
            else if (numOfCols % 4 == 3)
                whichTarget = &topSide;
            else
                whichTarget = &leftSide;
            Vector3 thePos((sizeofSquare.x * 0.5f) + (numOfCols * sizeofSquare.x * 0.5f), (sizeofSquare.y * 0.5f) + (numOFRows * sizeofSquare.y * 0.5f), 0);
            theLayer->AddUIElement(UI_Element::UI_BUTTON_T_TO_SCRN, "SmallBlackSquare",*whichTarget , thePos, sizeofSquare, thePos);
        }
    }
    theLayer->AddUIElement(UI_Element::UI_BUTTON_T_TO_SCRN, "UI_ChatBox", topSide, Vector3(cSS_InputManager->cIM_ScreenWidth * 0.5f, cSS_InputManager->cIM_ScreenHeight * 0.5f, 5.f), Vector3(600, 100, 1), Vector3(cSS_InputManager->cIM_ScreenWidth * 0.5f, cSS_InputManager->cIM_ScreenHeight * 0.5f, 5.f), "Loading Game");
    theLoadingEffect->cUIS_LayerContainer.push_back(theLayer);
}

void Scene_System::UpdateLoadingStuff(double dt)
{
    m_accumulatedLoadingTime += dt;
    if (m_accumulatedLoadingTime > delayingLoadingTime)
    {
            whatLoadingState = FINISHED_LOADING;
    }
    for (std::vector<UI_Layer*>::iterator it = theLoadingEffect->cUIS_LayerContainer.begin(), end = theLoadingEffect->cUIS_LayerContainer.end(); it != end; ++it)
    {
        UI_Layer *theLayer = (*it);
        for (std::vector<UI_Element*>::iterator it2 = theLayer->cUI_Layer.begin(), end2 = theLayer->cUI_Layer.end(); it2 != end2; ++it2)
        {
            UI_Element *theElement = (*it2);
            theElement->Update((float)dt);
            if (whatLoadingState != prevLoadingState)
                theElement->SwapOriginalWithTarget();
        }
    }
    if (whatLoadingState != prevLoadingState)
        prevLoadingState = whatLoadingState;
    //if (m_accumulatedLoadingTime > 5)
    //{
    //    SetLoadingTime(2);
    //}
}

void Scene_System::SetLoadingTime(const double &dt)
{
    delayingLoadingTime = dt;
    m_accumulatedLoadingTime = 0;
    whatLoadingState = BEGIN_LOADING;
}