#include "Scene_2.h"
#include <sstream>

#include "Scene_1.h"

std::string Scene_2::id_ = "Scene 2";

Scene_2::Scene_2()
    : SceneEntity()
{
	SceneInputManager = nullptr;
    framerates = 0;
    setName(id_);
}

Scene_2::~Scene_2()
{

}

void Scene_2::Init()
{
    GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	SceneInputManager = new InputManager();

    Mtx44 perspective;
    perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
    projectionStack->LoadMatrix(perspective);

	camera.Init(Vector3(0, 0, -5), Vector3(0,0,0), Vector3(0, 1, 0));
	camera.SetKeyList(SceneInputManager);

	// Initiallise Model Specific Meshes Here
	Mesh* newMesh = MeshBuilder::GenerateTerrain("terrain", "Image//heightmap5.raw", m_heightMap);
	newMesh->textureArray[0] = LoadTGA("Image//RockTex.tga");
	newMesh->textureArray[1] = LoadTGA("Image//GrassStoneTex.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	newMesh = MeshBuilder::GenerateSkyPlane("skyplane", Color(1, 1, 1), 34, 500.0f, 2000.0f, 1.0f, 1.0f);
	newMesh->textureArray[0] = LoadTGA("Image//ClearSkyUp2.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	newMesh = MeshBuilder::GenerateQuad("SB_Left", Color(1, 1, 1));
	newMesh->textureArray[0] = LoadTGA("Image//RockTex.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	newMesh = MeshBuilder::GenerateQuad("SB_Right", Color(1, 1, 1));
	newMesh->textureArray[0] = LoadTGA("Image//RockTex.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	newMesh = MeshBuilder::GenerateQuad("SB_Top", Color(1, 1, 1));
	newMesh->textureArray[0] = LoadTGA("Image//RockTex.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	newMesh = MeshBuilder::GenerateQuad("SB_Bottom", Color(1, 1, 1));
	newMesh->textureArray[0] = LoadTGA("Image//RockTex.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	newMesh = MeshBuilder::GenerateQuad("SB_Front", Color(1, 1, 1));
	newMesh->textureArray[0] = LoadTGA("Image//RockTex.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	newMesh = MeshBuilder::GenerateQuad("SB_Back", Color(1, 1, 1));
	newMesh->textureArray[0] = LoadTGA("Image//RockTex.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	newMesh = MeshBuilder::GenerateQuad("ayylmao", Color(1, 1, 1));
	newMesh->textureArray[0] = LoadTGA("Image//weed.tga");
	SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

}

void Scene_2::Update(float dt)
{
    GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
    SceneGraphics->Update(dt);
	SceneInputManager->HandleUserInput();

	//Update Camera's Minimum Possible & Current Y Pos
	Application::cA_MinimumTerrainY = TerrainYScale * ReadHeightMap(m_heightMap, camera.position.x / TerrainXScale, camera.position.z / TerrainXScale) + camera.PlayerHeight;

	if (!(Application::cA_CurrentTerrainY - Application::cA_MinimumTerrainY <= Math::EPSILON && Application::cA_MinimumTerrainY - Application::cA_CurrentTerrainY <= Math::EPSILON))
	{
		float RateofChangeY = (Application::cA_CurrentTerrainY - Application::cA_MinimumTerrainY) * (float)dt * (camera.CameraCurrentWalkSpeed / 3);
		if (Application::cA_CurrentTerrainY - RateofChangeY >= Application::cA_MinimumTerrainY || Application::cA_CurrentTerrainY - RateofChangeY <= Application::cA_MinimumTerrainY)
		{
			Application::cA_CurrentTerrainY -= RateofChangeY;
		}
	}

    framerates = 1 / dt;

	if (SceneInputManager->GetKeyValue('1'))
    {
		Scene_System::accessing().SwitchToPreviousScene();
    }

	int RandAmount = Math::RandIntMinMax(0, 3);
	for (int i = 0; i < RandAmount; ++i)
	{
		float RandScale = Math::RandFloatMinMax(2, 3);
		BManager.AddParticle("ayylmao", Vector3(Math::RandFloatMinMax(-5, 5), Math::RandFloatMinMax(-5, 5), Math::RandFloatMinMax(-5, 5)), Vector3(RandScale, RandScale, RandScale), Vector3(Math::RandFloatMinMax(-5, 5), Math::RandFloatMinMax(-5, 5), Math::RandFloatMinMax(-5, 5)), camera.position, 2);
	}
	BManager.UpdateContainer(dt, camera.position);

	camera.Update(dt);
}

void Scene_2::RenderTerrain()
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	modelStack->PushMatrix();
	modelStack->Scale(TerrainXScale, TerrainYScale, TerrainXScale);
	SceneGraphics->RenderMesh("terrain", true);
	modelStack->PopMatrix();
}

void Scene_2::RenderSkyplane()
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	modelStack->PushMatrix();
	modelStack->Translate(500, 1800, -500);
	SceneGraphics->RenderMesh("skyplane", false);
	modelStack->PopMatrix();
}

void Scene_2::RenderShadowCasters()
{
	RenderTerrain();
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	for (std::vector<Billboard*>::iterator it = BManager.BillboardContainer.begin(); it != BManager.BillboardContainer.end(); ++it)
	{
		if ((*it)->Active)
		{
			modelStack->PushMatrix();
			float TimeRatio = 1.1f - (*it)->CurrentTime / (*it)->LifeTime;
			modelStack->Translate((*it)->Position.x, camera.position.y + (*it)->Position.y, (*it)->Position.z);
			modelStack->Rotate(Math::RadianToDegree(atan2(camera.position.x - (*it)->Position.x, camera.position.z - (*it)->Position.z)), 0, 1, 0);
			modelStack->Scale(TimeRatio * (*it)->Dimensions.x, TimeRatio *(*it)->Dimensions.y, TimeRatio *(*it)->Dimensions.z);
			SceneGraphics->RenderMesh((*it)->MeshName, false);
			modelStack->PopMatrix();
		}
	}
}

void Scene_2::RenderSkybox()
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

void Scene_2::RenderPassGPass()
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

void Scene_2::RenderPassMain()
{
	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
	SceneGraphics->m_renderPass = GraphicsEntity::RENDER_PASS::RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::cA_WindowWidth, Application::cA_WindowHeight);
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

	//RenderTerrain();
	RenderSkyplane();

	RenderShadowCasters();

	SceneGraphics->RenderMesh("reference", false);

	SceneGraphics->SetHUD(true);
	std::ostringstream ss;
	ss.str("");
	ss << "Scene 2 - FPS:" << framerates;
	ss.precision(3);
	SceneGraphics->RenderTextOnScreen("text", ss.str(), Color(0, 1, 0), 25, 25, 25);
	SceneGraphics->SetHUD(false);

}

void Scene_2::Render()
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

void Scene_2::Exit()
{
	if (SceneInputManager)
		delete SceneInputManager;
}