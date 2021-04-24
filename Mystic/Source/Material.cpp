#include"Material.h"
#include"Texture.h"
#include"GLBuffers.h"

#include"Shader.h"
#include"Texture.h"


#define MAT_DEFAULT_SHINE 20
#define MAT_DEFAULT_COLOR_DIFFUSE     aiColor4D(0,0,0,0)
#define MAT_DEFAULT_COLOR_AMBIENT     aiColor4D(0,0,0,0)
#define MAT_DEFAULT_COLOR_EMISSIVE    aiColor4D(0,0,0,0)
#define MAT_DEFAULT_COLOR_REFLECTIVE  aiColor4D(0,0,0,0)
#define MAT_DEFAULT_COLOR_TRANSPARENT aiColor4D(0,0,0,0)
#define MAT_DEFAULT_COLOR_SPECULAR    aiColor4D(0,0,0,0)

glm::vec4 toGLM(aiColor4D& in)
{
	return { in.r, in.g, in.b, in.a };
}
/// Create an Overload Operator Vec4 to replace this

void Material::Bind(Shader *_shader)
{
	//_shader->Enable();
	if (Properties.size())
	{
		_shader->SetUniform("SpecularPower", 32);
		_shader->SetUniform("SpecularStrength", .5f);
	}// SET ANY UNIFORMS FOR PROPERTIES

	if (Colors.size())
	{}// SET ANY COLOR UNIFORMS 

	if (Surfaces.size())
	{// BINDS ANY TEXTURES TO CORRECT SLOT
		for (auto &S : Surfaces)
		{
			S.Bind(_shader);
		}
	}
	//_shader->Disable();
}

void SurfaceMaterial::Bind(Shader *_shader)
{
	switch (Index)
	{
	case DIFFUSE:

#ifdef _BINDLESS_
		_shader->SetTextureUniform("DiffuseTexture", Picture);
#else
		_shader->SetUniform("DiffuseTexture", Picture.Handle);
#endif
		break;

	case NORMALS:
#ifdef _BINDLESS_
		_shader->SetTextureUniform("NormalTexture", Picture);
#else
		_shader->SetUniform("NormalTexture", Picture.Handle);
#endif
		break;
	}
}