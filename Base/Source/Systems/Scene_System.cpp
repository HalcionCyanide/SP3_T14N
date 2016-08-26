#include "Scene_System.h"
#ifdef _DEBUG
    #include <assert.h>
#endif

void Scene_System::Init()
{
    //currScene = nullptr;
    graphics_scene = nullptr;
	cSS_InputManager = new InputManager();
	gPlayer = new GlobalPlayer(1, 420, 1337, false);

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
    delayingLoadingTime = 0;
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
    theLayer->AddUIElement(UI_Element::UI_UNASSIGNED, "SmallWhiteSquare", Vector3(400, 300, 0), Vector3(400, 300, 0), Vector3(5, 5, 5), Vector3(400, 300, 0));
    theLoadingEffect->cUIS_LayerContainer.push_back(theLayer);
}

void Scene_System::UpdateLoadingStuff(double dt)
{
    bool noLongerNeedToUpdate = true;
    for (std::vector<UI_Layer*>::iterator it = theLoadingEffect->cUIS_LayerContainer.begin(), end = theLoadingEffect->cUIS_LayerContainer.end(); it != end; ++it)
    {
        UI_Layer *theLayer = (*it);
        for (std::vector<UI_Element*>::iterator it2 = theLayer->cUI_Layer.begin(), end2 = theLayer->cUI_Layer.end(); it2 != end2; ++it2)
        {
            UI_Element *theElement = (*it2);
            //theElement->
        }
    }
}