#ifndef GRAPHICS_ENTITY_H
#define GRAPHICS_ENTITY_H

#include "SceneEntity.h"
#include "MeshBuilder.h"
#include "Vector3.h"
#include "Light.h"
#include <map>
#include "DepthFBO.h"
#include <algorithm>
#include "Application.h"
#include "shader.hpp"
#include "LoadTGA.h"
#include "LoadOBJ.h"

#ifndef MAX_NUM_LIGHTS
#define MAX_NUM_LIGHTS 2
#endif

class GraphicsEntity : public SceneEntity
{
public:
    static std::string id_;
    GraphicsEntity();
    virtual ~GraphicsEntity();
    enum UNIFORM_TYPE
    {
        U_MVP = 0,
        U_MODELVIEW,
        U_MODELVIEW_INVERSE_TRANSPOSE,
        U_MATERIAL_AMBIENT,
        U_MATERIAL_DIFFUSE,
        U_MATERIAL_SPECULAR,
        U_MATERIAL_SHININESS,
        U_LIGHTENABLED,
        U_NUMLIGHTS,
        U_LIGHT0_TYPE,
        U_LIGHT0_POSITION,
        U_LIGHT0_COLOR,
        U_LIGHT0_POWER,
        U_LIGHT0_KC,
        U_LIGHT0_KL,
        U_LIGHT0_KQ,
        U_LIGHT0_SPOTDIRECTION,
        U_LIGHT0_COSCUTOFF,
        U_LIGHT0_COSINNER,
        U_LIGHT0_EXPONENT,
        U_LIGHT1_TYPE,
        U_LIGHT1_POSITION,
        U_LIGHT1_COLOR,
        U_LIGHT1_POWER,
        U_LIGHT1_KC,
        U_LIGHT1_KL,
        U_LIGHT1_KQ,
        U_LIGHT1_SPOTDIRECTION,
        U_LIGHT1_COSCUTOFF,
        U_LIGHT1_COSINNER,
        U_LIGHT1_EXPONENT,

        U_COLOR_TEXTURE_ENABLED,
        U_COLOR_TEXTURE_ENABLED1,
        U_COLOR_TEXTURE_ENABLED2,

        U_COLOR_TEXTURE,
        U_COLOR_TEXTURE1,
        U_COLOR_TEXTURE2,

		U_SHADOW_COLOR_TEXTURE,
		U_SHADOW_COLOR_TEXTURE1,
		U_SHADOW_COLOR_TEXTURE2,
		U_SHADOW_COLOR_TEXTURE_ENABLED,
		U_SHADOW_COLOR_TEXTURE_ENABLED1,
		U_SHADOW_COLOR_TEXTURE_ENABLED2,

		U_LIGHT_DEPTH_MVP_GPASS,
		U_LIGHT_DEPTH_MVP,
		U_SHADOW_MAP,

		U_SHADOW_SAMPLING_AMOUNT, // Sample Count

		U_FOG_COLOR,
		U_FOG_START,
		U_FOG_END,
		U_FOG_DENSITY,
		U_FOG_TYPE,
		U_FOG_ENABLED,

        U_TEXT_ENABLED,
        U_TEXT_COLOR,
      
        U_TOTAL,
    };

	enum RENDER_PASS
	{
		RENDER_PASS_PRE,
		RENDER_PASS_MAIN,
	};

    std::map<std::string, Mesh*> meshList;

    virtual void Init();
    virtual void Update(float dt);
    virtual void Render();
    virtual void Exit();

    unsigned m_parameters[U_TOTAL];
    unsigned m_programID;
    unsigned m_vertexArrayID;
    Light lights[MAX_NUM_LIGHTS];

    virtual void RenderText(Mesh& mesh, const std::string &text, Color &color);
    virtual void RenderText(const std::string &text, Color &color);
    virtual void RenderTextOnScreen(Mesh& mesh, const std::string &text, Color &color, const float &size, const float &x, const float &y);
    virtual void RenderTextOnScreen(const std::string &text, Color &color, const float &size, const float &x, const float &y);
    virtual void RenderMeshIn2D(Mesh &mesh, const bool &enableLight, const float &size = 1.0f, const float &x = 0.0f, const float &y = 0.0f);
    virtual void RenderMesh(Mesh &mesh, const bool &enableLight);
    virtual void RenderText(const std::string &meshName, const std::string &text, Color &color);
    virtual void RenderTextOnScreen(const std::string &meshName, const std::string &text, Color &color, const float &size, const float &x, const float &y);
	virtual void RenderMeshIn2D(const std::string &meshName, const bool &enableLight, const float &size = 1.0f, const float &x = 0.0f, const float &y = 0.0f);
	virtual void RenderMesh(const std::string &meshName, const bool &enableLight);

    void SetHUD(const bool& m_bHUDmode);

	// My stuff
	unsigned m_gPassShaderID;
	DepthFBO m_lightDepthFBO;
	Mtx44 m_lightDepthProj;
	Mtx44 m_lightDepthView;
	RENDER_PASS m_renderPass;

private:
    bool m_bIsInOrthogonalMode;
    bool loadingMeshDriven(const std::string &fileLocation);
    Mesh *ExportedFont;
};

#endif