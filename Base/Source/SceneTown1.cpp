#include "SceneTown1.h"
#include <sstream>

#include "Scene_2.h"
#include "SceneTown2.h"
#include "SceneTown3.h"

#include "GameMap.h"
#include "GameObject.h"

std::string SceneTown1::id_ = "Scene Town 1";

SceneTown1::SceneTown1()
    : SceneEntity()
{
    framerates = 0;
    setName(id_);
    theInteractiveMap = nullptr;
}

SceneTown1::~SceneTown1()
{

}

void SceneTown1::Init()
{
    GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());

    // Set Terrain Size
    TerrainScale.Set(700.f, 100.f, 700.f);

    Mtx44 perspective;
    perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
    projectionStack->LoadMatrix(perspective);

    camera.Init(Vector3(0, 5, -5), Vector3(0, 5, 0), Vector3(0, 1, 0));

    // Initiallise Model Specific Meshes Here
    Mesh* newMesh = MeshBuilder::GenerateTerrain("Town 1", "Image//heightmap_Town1.raw", m_heightMap);
    newMesh->textureArray[0] = LoadTGA("Image//RockTex.tga");
    newMesh->textureArray[1] = LoadTGA("Image//GrassStoneTex.tga");
    SceneGraphics->meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

    Application::cA_MinimumTerrainY = TerrainScale.y * ReadHeightMap(m_heightMap, camera.position.x / TerrainScale.x, camera.position.z / TerrainScale.z) + camera.PlayerHeight;
    Application::cA_CurrentTerrainY = Application::cA_MinimumTerrainY;

    for (int i = 0; i < 8; i++)
    {
        BManager.AddHMapBillboard("Tree", m_heightMap, TerrainScale, Vector3((float)i * 10.f), Vector3(10.f, 20.f, 10.f), Vector3(), camera.position);
    }

    theInteractiveMap = new GameMap();
    GameMap *theMap = dynamic_cast<GameMap*>(theInteractiveMap);
    theMap->setName("scene town 1 logic map");
    theMap->LoadMap("Image//Town1Layout.csv", m_heightMap, TerrainScale, objVec, BManager);
}

void SceneTown1::Update(float dt)
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
    Vector3 Center(Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth / 2, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight / 2, 0);

    framerates = 1 / dt;

	if (Scene_System::accessing().cSS_InputManager->GetKeyValue('2'))
	{
		Scene_System::accessing().SwitchScene(SceneTown2::id_);
	}
	if (Scene_System::accessing().cSS_InputManager->GetKeyValue('3'))
	{
		Scene_System::accessing().SwitchScene(SceneTown3::id_);
	}
	if (Scene_System::accessing().cSS_InputManager->GetKeyValue('4'))
	{
		Scene_System::accessing().SwitchScene(Scene_2::id_);
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

    BManager.UpdateContainer(dt, camera.position);

    camera.Update(dt);
}

void SceneTown1::RenderTerrain()
{
    GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
    modelStack->PushMatrix();
    modelStack->Scale(TerrainScale.x, TerrainScale.y, TerrainScale.z);
    SceneGraphics->RenderMesh("Town 1", true);
    modelStack->PopMatrix();
}

void SceneTown1::RenderShadowCasters()
{
    RenderTerrain();
    GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
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
}

void SceneTown1::RenderSkybox()
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

void SceneTown1::RenderPassGPass()
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

void SceneTown1::RenderPassMain()
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

    //RenderTerrain();
    //RenderSkyplane();
    RenderSkybox();
    RenderShadowCasters();

    SceneGraphics->RenderMesh("reference", false);

    //<!> will remove soon <!>
    for (auto it : objVec)
    {
        it->Render();
    }
    //<!> will remove soon <!>

    SceneGraphics->SetHUD(true);
    std::ostringstream ss;
    ss.str("");
    ss << "Scene 1 - FPS:" << framerates;
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

void SceneTown1::Render()
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

void SceneTown1::Exit()
{
    if (theInteractiveMap)
        delete theInteractiveMap;
    for (auto it : objVec)
    {
        if (it)
            delete it;
    }
}