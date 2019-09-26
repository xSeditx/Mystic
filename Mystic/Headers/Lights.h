#pragma once
#include "Common.h"
#include "GameObject.h"
class Shader;

#include"GLBuffers.h"

struct LightProperties
{
	Vec3 LightPosition;
	Vec3 LightRotation;
	Vec3 LightColor;
};

class Light : public GameObject
{
public:
	Light();
	Light(Vec3 _pos);

	void Update();
    void Bind();

	void Render(Shader &_shader);
	void Bind(Shader &_shader);
	void Unbind();

	Vec3 Color;
	void SetPosition(Vec3 _pos) { Position = _pos; }// LightPosition.Set(Position);
};


class SpotLight : public Light
{
    SpotLight(Vec3 _pos, Vec3 _rot);

    Vec3 Rotation;
};





//NOTE: Up direction in Tangent Space is just the Original Normal of the Given Plane
/// BENNYBOX SPECULAR LIGHTING
//https://www.youtube.com/watch?v=ZYb0Lg6ko9k

/// LIGHTING TUTORIAL
// http://ogldev.atspace.co.uk/www/tutorial19/tutorial19.html