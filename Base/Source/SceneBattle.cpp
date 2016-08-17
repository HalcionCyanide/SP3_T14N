#include "SceneBattle.h"
#include <sstream>

#include "Scene_2.h"
#include "GameMap.h"
#include "GameObject.h"

std::string SceneBattle::id_ = "Scene Town 1";

SceneBattle::SceneBattle()
    : SceneEntity()
{
    framerates = 0;
    setName(id_);
    theInteractiveMap = nullptr;
}

SceneBattle::~SceneBattle()
{

}

void SceneBattle::Init()
{
    GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());

    Mtx44 perspective;
    perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
    projectionStack->LoadMatrix(perspective);

    camera.Init(Vector3(0, 5, -5), Vector3(0, 5, 0), Vector3(0, 1, 0));
}

void SceneBattle::Update(float dt)
{
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

    BManager.UpdateContainer(dt, camera.position);

    camera.Update(dt);
}

void SceneBattle::RenderShadowCasters()
{
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

void SceneBattle::RenderPassGPass()
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

void SceneBattle::RenderPassMain()
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

void SceneBattle::Render()
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

void SceneBattle::Exit()
{
    if (theInteractiveMap)
        delete theInteractiveMap;
    for (auto it : objVec)
    {
        if (it)
            delete it;
    }
}