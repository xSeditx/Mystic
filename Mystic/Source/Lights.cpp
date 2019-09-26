#include"Lights.h"
#include<Shader.h>
// PBR Learn OpenGL https://learnopengl.com/PBR/Lighting
//https://www.researchgate.net/figure/Images-of-specular-tracing-in-the-Crytek-Sponza-scene-From-left-to-right-and-top-to_fig29_258008934

Light::Light()
	:
	GameObject(),
	Color(1.0f, 1.0f, 1.0f)
{
}
Light::Light(Vec3 _pos)
	:
 	GameObject(_pos, Vec3(0.0f), Vec3(1.0f)),
	Color(1.0f, 1.0f, 1.0f)
{
}

void Light::Update()
{
	Transform = glm::mat4(1.0f); //  Set Identity and Rotate all axis followed with the Translation.
	Transform = glm::translate(Transform, Position);
	Transform = glm::rotate(Transform, glm::radians(Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
}

void Light::Render(Shader &_shader)
{//Generate a Debug Quad for rendering the Light view
}

void Light::Bind()
{// Pass Uniforms to the Active shader letting it know light positions
	Update();
   __debugbreak();
}
void Light::Bind(Shader &_shader)
{
	_shader.SetUniform("LightPosition", Position.x, Position.y, Position.z);
	//_shader->SetUniform("LightRotation", Rotation.x, Rotation.y, Rotation.z);
	_shader.SetUniform("LightColor", Color);
}

void Light::Unbind()
{}

//http://www.conitec.net/shaders/shader_work3.htm

 