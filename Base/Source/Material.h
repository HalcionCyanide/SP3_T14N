#ifndef MATERIAL_H
#define MATERIAL_H

struct Component
{
	float r, g, b, a = 1.f;
	Component(float r = 0.1f, float g = 0.1f, float b = 0.1f) {Set(r, g, b);}
	void Set(float r, float g, float b) {this->r = r; this->g = g; this->b = b;}
};
struct Material
{
	Component kAmbient;
	Component kDiffuse;
	Component kSpecular;
	float kShininess;

	Material()
	{
		//some default values
		kAmbient.Set(0.3f, 0.3f, 0.3f);
		kDiffuse.Set(0.6f, 0.6f, 0.6f);
		kSpecular.Set(0.3f, 0.3f, 0.3f);
		kShininess = 5.f;
	}
};

// https://www.opengl.org/discussion_boards/showthread.php/175008-using-alpha-blending-with-shaders

#endif