#include "GraphicsEntity.h"
#include <assert.h>
#include <fstream>
#include <sstream>
#include "Scene_System.h"

std::string GraphicsEntity::id_ = "Graphics";

void convertStringToUpperCaps(std::string &theString)
{
    for (std::string::iterator it = theString.begin(), end = theString.end(); it != end; ++it) {
        (*it) = toupper(*it);
    }
}

GraphicsEntity::GraphicsEntity()
    : SceneEntity()
{
    if (modelStack)
    {
        delete modelStack;
        modelStack = nullptr;
    }
    if (viewStack)
    {
        delete viewStack;
        viewStack = nullptr;
    }
    if (projectionStack)
    {
        delete projectionStack;
        projectionStack = nullptr;
    }
    m_bIsInOrthogonalMode = false;
}

GraphicsEntity::~GraphicsEntity()
{

}

void GraphicsEntity::Init()
{
    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GREATER, (GLclampf)0.1f);
	glEnable(GL_ALPHA_TEST);

    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

	m_gPassShaderID = LoadShaders("Shader//GPass.vertexshader", "Shader//GPass.fragmentshader");
	m_parameters[U_LIGHT_DEPTH_MVP_GPASS] = glGetUniformLocation(m_gPassShaderID, "lightDepthMVP");

	m_programID = LoadShaders("Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader");
	m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");
	m_parameters[U_SHADOW_MAP] = glGetUniformLocation(m_programID, "shadowMap");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	// Shadow Alpha
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[0]");
	m_parameters[U_SHADOW_COLOR_TEXTURE] = glGetUniformLocation(m_gPassShaderID, "colorTexture[0]");
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[1]");
	m_parameters[U_SHADOW_COLOR_TEXTURE1] = glGetUniformLocation(m_gPassShaderID, "colorTexture[1]");
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED2] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[2]");
	m_parameters[U_SHADOW_COLOR_TEXTURE2] = glGetUniformLocation(m_gPassShaderID, "colorTexture[2]");

	m_parameters[U_SHADOW_SAMPLING_AMOUNT] = glGetUniformLocation(m_programID, "shadowSamplingCount");

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");
	m_parameters[U_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
	m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");
	m_parameters[U_COLOR_TEXTURE_ENABLED2] = glGetUniformLocation(m_programID, "colorTextureEnabled[2]");
	m_parameters[U_COLOR_TEXTURE2] = glGetUniformLocation(m_programID, "colorTexture[2]");

	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// Obtain a handle for the Fog Parameters
	m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.Color");
	m_parameters[U_FOG_START] = glGetUniformLocation(m_programID, "fogParam.FogStart");
	m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fogParam.FogEnd");
	m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.Density");
	m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.Type");
	m_parameters[U_FOG_ENABLED] = glGetUniformLocation(m_programID, "fogParam.FogEnabled");

	// Use our shader
	glUseProgram(m_programID);

	m_lightDepthFBO.Init(4096, 4096);
	glUniform1i(m_parameters[U_SHADOW_SAMPLING_AMOUNT], 16);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(1600, 1200, 500);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	// Create meshes that can be used in any scene here
	// Other scene specific meshes should only be inited in the respective scene
    Mesh* newMesh = MeshBuilder::GenerateAxes("reference", 1000.f, 1000.f, 1000.f);
	meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

    assert(loadingMeshDriven("Image//MeshDriven.csv"));
    ExportedFont = meshList.find("text")->second;
}

void GraphicsEntity::Update(float dt)
{
    /*if (Application::IsKeyPressed('1')) {
        glEnable(GL_CULL_FACE);
    }
    if (Application::IsKeyPressed('2')) {
        glDisable(GL_CULL_FACE);
    }*/
    if (Application::IsKeyPressed('6')) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
	if (Application::IsKeyPressed('7')) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

void GraphicsEntity::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsEntity::Exit()
{
    if (meshList.empty() == false)
    {
        for (auto it : meshList)
        {
            delete it.second;
        }
        meshList.clear();
    }
    glDeleteProgram(m_programID);
    glDeleteVertexArrays(1, &m_vertexArrayID);
    modelStack = nullptr;
    viewStack = nullptr;
    projectionStack = nullptr;
}

void GraphicsEntity::RenderText(Mesh& mesh, const std::string &text, Color &color)
{
    if (mesh.textureArray[0] <= 0/* || mesh.textureID <= 0*/)
        return;

    glDisable(GL_DEPTH_TEST);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
    glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh.textureArray[0]);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    for (unsigned i = 0; i < text.length(); ++i)
    {
        Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.75f, 0, 0); //1.0f is the spacing of each character, you may change this value
        Mtx44 MVP = projectionStack->Top() * viewStack->Top() * modelStack->Top() * characterSpacing;
        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

        mesh.Render((unsigned)text[i] * 6, 6);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
    glEnable(GL_DEPTH_TEST);
}

void GraphicsEntity::RenderText(const std::string &text, Color &color)
{
    if (ExportedFont->textureArray[0] <= 0/* || mesh.textureID <= 0*/)
        return;

    glDisable(GL_DEPTH_TEST);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
    glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ExportedFont->textureArray[0]);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    for (unsigned i = 0; i < text.length(); ++i)
    {
        Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.75f, 0, 0); //1.0f is the spacing of each character, you may change this value
        Mtx44 MVP = projectionStack->Top() * viewStack->Top() * modelStack->Top() * characterSpacing;
        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

        ExportedFont->Render((unsigned)text[i] * 6, 6);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
    glEnable(GL_DEPTH_TEST);
}

void GraphicsEntity::RenderTextOnScreen(Mesh& mesh, const std::string &text, Color &color, const float &size, const float &x, const float &y)
{
    if (mesh.textureArray[0] <= 0)
        return;

    viewStack->PushMatrix();
    viewStack->LoadIdentity();
    modelStack->PushMatrix();
    modelStack->LoadIdentity();
    modelStack->Translate(x, y, 0);
    modelStack->Scale(size, size, size);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
    glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh.textureArray[0]);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    for (unsigned i = 0; i < text.length(); ++i)
    {
        Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.75f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
        Mtx44 MVP = projectionStack->Top() * viewStack->Top() * modelStack->Top() * characterSpacing;
        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

        mesh.Render((unsigned)text[i] * 6, 6);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
    modelStack->PopMatrix();
    viewStack->PopMatrix();
}

void GraphicsEntity::RenderTextOnScreen(const std::string &text, Color &color, const float &size, const float &x, const float &y)
{
    if (ExportedFont->textureArray[0] <= 0)
        return;

    viewStack->PushMatrix();
    viewStack->LoadIdentity();
    modelStack->PushMatrix();
    modelStack->LoadIdentity();
    modelStack->Translate(x, y, 0);
    modelStack->Scale(size, size, size);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
    glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ExportedFont->textureArray[0]);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    for (unsigned i = 0; i < text.length(); ++i)
    {
        Mtx44 characterSpacing;
        characterSpacing.SetToTranslation(i * 0.75f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
        Mtx44 MVP = projectionStack->Top() * viewStack->Top() * modelStack->Top() * characterSpacing;
        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

        ExportedFont->Render((unsigned)text[i] * 6, 6);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
    modelStack->PopMatrix();
    viewStack->PopMatrix();
}

void GraphicsEntity::RenderMeshIn2D(Mesh &mesh, const bool &enableLight, const float &size, const float &x, const float &y)
{
    viewStack->PushMatrix();
    viewStack->LoadIdentity();
    modelStack->PushMatrix();
    modelStack->LoadIdentity();
    modelStack->Translate(x, y, 0);

    modelStack->Scale(size, size, size);
    Mtx44 MVP, modelView, modelView_inverse_transpose;

    MVP = projectionStack->Top() * viewStack->Top() * modelStack->Top();
    glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)
	{
		if (mesh.textureArray[i] > 0)
		{
			//theres texture lets load it!!
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh.textureArray[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
		}
		else
		{

			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}
	}
	glUniform1i(m_parameters[U_LIGHTENABLED], enableLight);
    mesh.Render();
    if (mesh.textureArray[0] > 0 || mesh.textureID > 0)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    modelStack->PopMatrix();
    viewStack->PopMatrix();
}

void GraphicsEntity::RenderMeshIn2D(Mesh &mesh, const bool &enableLight, const float &sizeX, const float &sizeY, const float &x, const float &y)
{
    viewStack->PushMatrix();
    viewStack->LoadIdentity();
    modelStack->PushMatrix();
    modelStack->LoadIdentity();
    modelStack->Translate(x, y, 0);

    modelStack->Scale(sizeX, sizeY, 1);
    Mtx44 MVP, modelView, modelView_inverse_transpose;

    MVP = projectionStack->Top() * viewStack->Top() * modelStack->Top();
    glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
    for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)
    {
        if (mesh.textureArray[i] > 0)
        {
            //theres texture lets load it!!
            glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, mesh.textureArray[i]);
            glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
        }
        else
        {

            glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
        }
    }
    glUniform1i(m_parameters[U_LIGHTENABLED], enableLight);
    mesh.Render();
    if (mesh.textureArray[0] > 0 || mesh.textureID > 0)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    modelStack->PopMatrix();
    viewStack->PopMatrix();
}

void GraphicsEntity::RenderMesh(Mesh &mesh, const bool &enableLight)
{
    Mtx44 MVP, modelView, modelView_inverse_transpose;

	if (m_renderPass == RENDER_PASS_PRE)
	{
		Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack->Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP_GPASS], 1, GL_FALSE, &lightDepthMVP.a[0]);
		for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)
		{
			if (mesh.textureArray[i] > 0)
			{
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED + i], 1);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, mesh.textureArray[i]);
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE + i], i);
			}
			else
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED + i], 0);
		}
		mesh.Render();
		return;
	}

    MVP = projectionStack->Top() * viewStack->Top() * modelStack->Top();
    glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
    modelView = viewStack->Top() * modelStack->Top();
    glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
    modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
    glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
    if (enableLight)
    {
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);
		Mtx44 lightDepthMVP = m_lightDepthProj *	m_lightDepthView * modelStack->Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP], 1, GL_FALSE, &lightDepthMVP.a[0]);
        //load material
        glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh.material.kAmbient.r);
        glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh.material.kDiffuse.r);
        glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh.material.kSpecular.r);
        glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh.material.kShininess);
    }
    else
    {
        glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    }
    for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)
    {
        if (mesh.textureArray[i] > 0)
        {
            glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);

            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, mesh.textureArray[i]);
            glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
        }
        else
        {
            glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
        }
    }
    mesh.Render();
    if (mesh.textureArray[0] > 0)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void GraphicsEntity::RenderText(const std::string &meshName, const std::string &text, Color &color)
{
    std::map<std::string, Mesh*>::iterator it = meshList.find(meshName);
    assert(it != meshList.end());
    RenderText(*it->second, text, color);
}

void GraphicsEntity::RenderTextOnScreen(const std::string &meshName, const std::string &text, Color &color, const float &size, const float &x, const float &y)
{
    std::map<std::string, Mesh*>::iterator it = meshList.find(meshName);
    assert(it != meshList.end());
    RenderTextOnScreen(*it->second, text, color, size, x, y);
}

void GraphicsEntity::RenderMeshIn2D(const std::string &meshName, const bool &enableLight, const float &size, const float &x, const float &y)
{
	std::map<std::string, Mesh*>::iterator it = meshList.find(meshName);
	assert(it != meshList.end());
	RenderMeshIn2D(*it->second, enableLight, size, x, y);
}

void GraphicsEntity::RenderMeshIn2D(const std::string &meshName, const bool &enableLight, const float &sizeX, const float &sizeY, const float &x, const float &y)
{
    std::map<std::string, Mesh*>::iterator it = meshList.find(meshName);
    assert(it != meshList.end());
    RenderMeshIn2D(*it->second, enableLight, sizeX, sizeY, x, y);
}

void GraphicsEntity::RenderMesh(const std::string &meshName, const bool &enableLight)
{
    std::map<std::string, Mesh*>::iterator it = meshList.find(meshName);
    assert(it != meshList.end());
    RenderMesh(*it->second, enableLight);
}

void GraphicsEntity::SetHUD(const bool& m_bHUDmode)
{
    if (m_bHUDmode)
    {
        if (m_bIsInOrthogonalMode == false)
        {
            m_bIsInOrthogonalMode = true;
            glDisable(GL_DEPTH_TEST);
            Mtx44 ortho;
			ortho.SetToOrtho(0, Scene_System::accessing().cSS_InputManager->cIM_ScreenWidth, 0, Scene_System::accessing().cSS_InputManager->cIM_ScreenHeight, -10, 10);
            projectionStack->PushMatrix();
            projectionStack->LoadMatrix(ortho);
        }
    }
    else
    {
        if (m_bIsInOrthogonalMode == true)
        {
            m_bIsInOrthogonalMode = false;
            projectionStack->PopMatrix();
            glEnable(GL_DEPTH_TEST);
        }
    }
}

bool GraphicsEntity::loadingMeshDriven(const std::string &fileLocation)
{
    std::ifstream file(fileLocation.c_str());
    if (file.is_open())
    {
        std::string data = "";
        std::vector<std::string> theKeys;
        std::vector<std::string> theValues;
        std::map<std::string, GLuint> targaStuff;
        while (getline(file, data))
        {
            if (data == "" || data == "\n" || data == "\r")
                continue;
            std::string token;
            std::istringstream iss(data);
            if (theKeys.empty())
            {   //Get the keys from CSV
                while (getline(iss, token, ','))
                {
                    convertStringToUpperCaps(token);
                    theKeys.push_back(token);
                }
            }
            else {  //Begin getting all the values from the CSV
                while (getline(iss, token, ','))
                {
                    theValues.push_back(token);
                }
                //NAME
                std::vector<std::string>::iterator it;
                it = std::find(theKeys.begin(), theKeys.end(), "NAME");
                size_t pos = it - theKeys.begin();
                std::string theName = theValues[pos];
                //NAME
                Mesh *newMesh = nullptr;
                //COLORS
                float r = 0, g = 0, b = 0;
                it = std::find(theKeys.begin(), theKeys.end(), "COLORR");
                pos = it - theKeys.begin();
                if (theValues[pos] == "")
                    r = 1;
                else
                    r = stof(theValues[pos]);
                it = std::find(theKeys.begin(), theKeys.end(), "COLORG");
                pos = it - theKeys.begin();
                if (theValues[pos] == "")
                    g = 1;
                else
                    g = stof(theValues[pos]);
                it = std::find(theKeys.begin(), theKeys.end(), "COLORB");
                pos = it - theKeys.begin();
                if (theValues[pos] == "")
                    b = 1;
                else
                    b = stof(theValues[pos]);
                //COLORS
                //OBJECTYPE
                it = std::find(theKeys.begin(), theKeys.end(), "OBJECTTYPE");
                pos = it - theKeys.begin();
                convertStringToUpperCaps(theValues[pos]);
                //OBJECTYPE
                std::string objectFile;
                if (theValues[pos] == "TEXT") {
                    newMesh = MeshBuilder::GenerateText(theName, 16, 16);
                }
                else if (theValues[pos] == "QUAD") {
                    newMesh = MeshBuilder::GenerateQuad(theName, Color(r, g, b), 1.f);
                }
                else if (theValues[pos] == "3DOBJECT") {
                    it = std::find(theKeys.begin(), theKeys.end(), "OBJECTFILE");
                    pos = it - theKeys.begin();
                    newMesh = MeshBuilder::GenerateOBJ(theName, theValues[pos]);
                }
                else if (theValues[pos] == "CUBE") {
                    newMesh = MeshBuilder::GenerateCube(theName, Color(r, g, b));
                }
                else if (theValues[pos] == "SPRITE") {
                    unsigned row, col;
                    it = std::find(theKeys.begin(), theKeys.end(), "NUMROWS");
                    pos = it - theKeys.begin();
                    row = stoi(theValues[pos]);
                    it = std::find(theKeys.begin(), theKeys.end(), "NUMCOLUMNS");
                    pos = it - theKeys.begin();
                    col = stoi(theValues[pos]);
                    newMesh = MeshBuilder::GenerateSpriteAnimation(theName, row, col);
                }
                else {
                    continue;
                }
                //TEXTURES
                for (unsigned num = 0; num < Mesh::MAX_TEXTURES; ++num)
                {
                    std::ostringstream ss;
                    ss << "TEXTURE" << num + 1;
                    it = std::find(theKeys.begin(), theKeys.end(), ss.str());
                    pos = it - theKeys.begin();
                    if (theValues[pos] == "" || theValues[pos] == "\n" || theValues[pos] == "\r")
                        continue;
                    std::map<std::string, GLuint>::iterator it = targaStuff.find(theValues[pos]);
                    if (it != targaStuff.end())
                    {
                        newMesh->textureArray[num] = it->second;
                    }
                    else {
                        newMesh->textureArray[num] = LoadTGA(theValues[pos].c_str());
                        targaStuff.insert(std::pair<std::string, GLuint>(theValues[pos], newMesh->textureArray[num]));
                    }
                    if (num == 0)
                        newMesh->textureID = newMesh->textureArray[num];
                }
                //TEXTURES
                meshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));
                theValues.clear();
            }
        }
        return true;
    }
    return false;
}

