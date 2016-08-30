#include "SceneBattleScreen.h"
#include <sstream>

#include "Scene_2.h"
#include "..\\Classes\\GameMap.h"
#include "..\\Classes\\GameObject.h"
#include "..\\Misc\\LoadEnemyData.h"
#include "../Systems/MusicSystem.h"

std::string SceneBattleScreen::id_ = "Scene Battle Screen";

SceneBattleScreen::SceneBattleScreen()
	: SceneEntity()
{
	framerates = 0;
	setName(id_);
	theInteractiveMap = nullptr;
}

SceneBattleScreen::~SceneBattleScreen()
{

}

void SceneBattleScreen::Init()
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());

	LoadEnemyData("DrivenFiles//EnemyData.csv", Scene_System::accessing().EnemyData);

    Mtx44 perspective;
	perspective.SetToPerspective(45.0f, Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight, 0.1f, 10000.0f);
	projectionStack->LoadMatrix(perspective);

	Mesh* newMesh = MeshBuilder::GenerateQuad("Player", Color(1, 1, 1));
	newMesh->textureArray[0] = LoadTGA("Image//TFB_GEM.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	newMesh = MeshBuilder::GenerateQuad("GBox", Color(1, 1, 1));
	newMesh->textureArray[0] = LoadTGA("Image//GBox.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	camera.Init(Vector3(0, 0, 1), 0, Vector3(0, 1, 0));

	Scene_System::accessing().BSys = new BattleSystem();
	Scene_System::accessing().BSys->Init();
	Scene_System::accessing().BSys->cBillboardManager.Init();

	Scene_System::accessing().cSS_PlayerUIManager->Init();
}

void SceneBattleScreen::Update(float dt)
{
	Scene_System::accessing().cSS_InputManager->cIM_inMouseMode = true;
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	SceneGraphics->Update(dt);

	framerates = 1 / dt;

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

	Scene_System::accessing().BSys->cBillboardManager.UpdateContainer(dt, camera.position);

	camera.Update(dt);
	Scene_System::accessing().BSys->Update((float)dt);
    Scene_System::accessing().UpdateLoadingStuff(dt);
}

void SceneBattleScreen::RenderPassGPass()
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
}

void SceneBattleScreen::RenderPassMain()
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
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
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

	//SceneGraphics->RenderMesh("reference", false);

	SceneGraphics->SetHUD(true);
	Scene_System::accessing().BSys->Render();

	if (Scene_System::accessing().cSS_InputManager->cIM_inMouseMode)
	{
		modelStack->PushMatrix();
		modelStack->Translate(Scene_System::accessing().cSS_InputManager->GetMousePosition().x, Scene_System::accessing().cSS_InputManager->GetMousePosition().y, 0);
		modelStack->Rotate(0, 0, 1, 0);
		modelStack->Scale(Scene_System::accessing().BSys->PlayerScale * 0.75f, Scene_System::accessing().BSys->PlayerScale * 0.75f, 1);
		SceneGraphics->RenderMesh("TFB_Gem", false);
		modelStack->PopMatrix();
	}
    if (Scene_System::accessing().theLoadingEffect)
        Scene_System::accessing().RenderLoadingStuff();

	std::ostringstream ss;
	ss.str("");
	ss << "Scene BS - FPS:" << framerates;
	ss.precision(3);
	SceneGraphics->RenderTextOnScreen("text", ss.str(), Color(0, 1, 0), 25, 25, 25);

	SceneGraphics->SetHUD(false);
}

void SceneBattleScreen::Render()
{
	RenderPassMain();
}

void SceneBattleScreen::Exit()
{
	if (theInteractiveMap)
		delete theInteractiveMap;
	Scene_System::accessing().BSys->cBillboardManager.Exit();
	Scene_System::accessing().BSys->Exit();
}