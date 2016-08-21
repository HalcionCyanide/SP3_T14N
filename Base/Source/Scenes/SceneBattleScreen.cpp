#include "SceneBattleScreen.h"
#include <sstream>

#include "Scene_2.h"
#include "..\\Classes\\GameMap.h"
#include "..\\Classes\\GameObject.h"

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

	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight, 0.1f, 10000.0f);
	projectionStack->LoadMatrix(perspective);

	Vector3 CenterPosition(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.5f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);
	PlayerScale = Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.03f;

	Mesh* newMesh = MeshBuilder::GenerateQuad("Player", Color(1, 1, 1));
	newMesh->textureArray[0] = LoadTGA("Image//TFB_GEM.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	newMesh = MeshBuilder::GenerateQuad("GBox", Color(1, 1, 1));
	newMesh->textureArray[0] = LoadTGA("Image//GBox.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	Player = new BattleScreenObject("Player", 3, CenterPosition, Vector3(PlayerScale, PlayerScale, 1), Vector3(0, 0, 0), 0, Vector3(0, 0, 1));

	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));
	//camera.CameraIsLocked = true;

	CurrentMousePosition = Scene_System::accessing().cSS_InputManager->GetMousePosition();

	BaseExterior = new UI_Element(UI_Element::UI_UNASSIGNED, "GBox", CenterPosition, CenterPosition, Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.65f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.85f, 1), CenterPosition);
	BaseInterior = new UI_Element(UI_Element::UI_UNASSIGNED, "GBox", CenterPosition, CenterPosition, Vector3(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.55f, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight * 0.75f, 1), CenterPosition);
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

	for (std::vector<BattleScreenObject*>::iterator it = ProjectileContainer.begin(); it != ProjectileContainer.end(); ++it)
	{
		BattleScreenObject* go = (BattleScreenObject*)*it;
		go->Update((float)dt);
	}

	BManager.UpdateContainer(dt, camera.position);

	camera.Update(dt);

	PlayerUpdate(dt);
}


void SceneBattleScreen::PlayerUpdate(float dt)
{
	if (BaseInterior->UI_Bounds->CheckCollision(Scene_System::accessing().cSS_InputManager->GetMousePosition(), Vector3()))
	{
		CurrentMousePosition = Scene_System::accessing().cSS_InputManager->GetMousePosition();
	}
	float ForceIncrement = Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.5f;
	bool MouseModeSelected = true;
	switch (MouseModeSelected)
	{
		case (true) :
		{
			Vector3 PlayerDirection = (CurrentMousePosition - Player->GetPosition());
			if (!PlayerDirection.IsZero())
				if (PlayerDirection.LengthSquared() < (Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth * 0.15f))
					PlayerDirection.SetZero();
				else PlayerDirection.Normalize();
				ActingForce.Set(ForceIncrement * PlayerDirection.x, ForceIncrement * PlayerDirection.y);
			break;
		}
		case (false) :
		{
			if (Application::IsKeyPressed('W'))
			{
				ActingForce.y = ForceIncrement;
			}
			if (Application::IsKeyPressed('S'))
			{
				ActingForce.y = -ForceIncrement;
			}
			if (!Application::IsKeyPressed('W') && !Application::IsKeyPressed('S'))
			{
				ActingForce.y = 0;
			}
			if (Application::IsKeyPressed('A'))
			{
				ActingForce.x = -ForceIncrement;
			}
			if (Application::IsKeyPressed('D'))
			{
				ActingForce.x = ForceIncrement;
			}
			if (!Application::IsKeyPressed('A') && !Application::IsKeyPressed('D'))
			{
				ActingForce.x = 0;
			}
			break;
		}
	}
		if (Player->GetMass() > Math::EPSILON)
		{
			// Velocity Due To Acceleration If Mass Exists
			Player->SetVelocity(Player->GetVelocity() + ActingForce * (1.f / Player->GetMass()) * dt);
		}
		Player->SetVelocity(Player->GetVelocity() - Player->GetVelocity()*FrictionDecrementMultiplier * dt);
		Vector3 FwdPos = Player->GetPosition() + Player->GetVelocity() * dt;
		// <!> Needs work
		if (abs(FwdPos.x - BaseInterior->UI_Bounds->GetPosition().x) > BaseInterior->UI_Bounds->GetScale().x * 0.5f - Player->GetDimensions().x * 0.5f)
			Player->SetVelocity(Vector3(/*-Player->GetVelocity().x * 0.5f*/ 0.f, Player->GetVelocity().y, 0.f));
		if (abs(FwdPos.y - BaseInterior->UI_Bounds->GetPosition().y) > BaseInterior->UI_Bounds->GetScale().y * 0.5f - Player->GetDimensions().y * 0.5f)
			Player->SetVelocity(Vector3(Player->GetVelocity().x,/* -Player->GetVelocity().y * 0.5f*/ 0.f, 0.f));
		Player->Update((double)dt);
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

	Mtx44 perspective;
	perspective.SetToOrtho(0, Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth, 0, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight, -100, 100);
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

	SceneGraphics->RenderMesh("reference", false);

	for (std::vector<BattleScreenObject*>::iterator it = ProjectileContainer.begin(); it != ProjectileContainer.end(); ++it)
	{
		BattleScreenObject* go = (BattleScreenObject*)*it;
		go->Render();
	}

	// Player Calls
	Player->Render();

	for (std::vector<Billboard*>::iterator it = BManager.BillboardContainer.begin(); it != BManager.BillboardContainer.end(); ++it)
	{
		if ((*it)->Active)
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

	SceneGraphics->SetHUD(true);
	BaseExterior->Render();
	BaseInterior->Render();
	// Player Calls
	Player->Render();

	if (Scene_System::accessing().cSS_InputManager->cIM_inMouseMode)
	{
		modelStack->PushMatrix();
		modelStack->Translate(Scene_System::accessing().cSS_InputManager->GetMousePosition().x, Scene_System::accessing().cSS_InputManager->GetMousePosition().y, 0);
		modelStack->Rotate(0, 0, 1, 0);
		modelStack->Scale(PlayerScale / 2, PlayerScale / 2, 1);
		SceneGraphics->RenderMesh("TFB_Gem", false);
		modelStack->PopMatrix();
	}

	std::ostringstream ss;
	ss.str("");
	ss << "Scene BS - FPS:" << framerates;
	ss.precision(3);
	SceneGraphics->RenderTextOnScreen("text", ss.str(), Color(0, 1, 0), 25, 25, 25);

	ss.str("");
	ss << "Player Pos:" << Player->GetPosition();
	ss.precision(3);
	SceneGraphics->RenderTextOnScreen("text", ss.str(), Color(0, 1, 0), 25, 25, 50);

	ss.str("");
	ss << "Player Acc:" << ActingForce*(1.f / Player->GetMass());
	ss.precision(3);
	SceneGraphics->RenderTextOnScreen("text", ss.str(), Color(0, 1, 0), 25, 25, 75);

	ss.str("");
	ss << "Player Vel:" << Player->GetVelocity();
	ss.precision(3);
	SceneGraphics->RenderTextOnScreen("text", ss.str(), Color(0, 1, 0), 25, 25, 100);

	ss.str("");
	ss << "Mouse Position:" << Scene_System::accessing().cSS_InputManager->GetMousePosition();
	ss.precision(3);
	SceneGraphics->RenderTextOnScreen("text", ss.str(), Color(0, 1, 0), 25, 25, 125);

}

void SceneBattleScreen::Render()
{
	//*********************************
	//		PRE RENDER PASS
	//*********************************
	//RenderPassGPass();
	//*********************************
	//		MAIN RENDER PASS
	//*********************************
	RenderPassMain();
}

void SceneBattleScreen::Exit()
{
	if (theInteractiveMap)
		delete theInteractiveMap;
	for (std::vector<BattleScreenObject*>::iterator it = ProjectileContainer.begin(); it != ProjectileContainer.end(); ++it)
	{
		if (*it)
		{
			delete *it;
			*it = nullptr;
		}
	}
	if (Player)
		delete Player;
}