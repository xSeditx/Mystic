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

glm::vec4 toGLM(aiColor4D in)
{
	return Vec4(in.r, in.g, in.b, in.a);
}

std::vector<Material> Material::Extract(const aiScene*_scene)
{
	std::vector<Material> RETURN;
	for_loop(i, _scene->mNumMaterials)
	{
		Material MatPush(Shader::GetActiveShader());
	 
		std::vector<ColorMaterial> ColorList;
		{// Extrac the correct Color Information from the Specific Material Index
			float Shine;
			const aiMaterial* ai_Material = _scene->mMaterials[i];
			// The generic way
			aiTexture Text; //ARGB8888
			//if (AI_SUCCESS != ai_Material->Get(AI_MATKEY_TEXTURE_DIFFUSE(i), &Text)) {
			//	// handle epic failure here
			//}
			
			
			// .aiTexel
			Bitmap Temp((unsigned char *)Text.pcData, Vec2(Text.mWidth, Text.mHeight), 4);
			//SurfaceMaterial SurfacePush;
			aiColor4D SpecColor, DiffColor, EmissiveColor, ReflectiveColor, TransColor, AmbientColor;
			if (aiGetMaterialFloat(ai_Material, AI_MATKEY_SHININESS, &Shine) != AI_SUCCESS)
			{
				Shine = MAT_DEFAULT_SHINE;
			}
			if (aiGetMaterialColor(ai_Material, AI_MATKEY_COLOR_SPECULAR, &SpecColor) != AI_SUCCESS)
			{
				SpecColor = MAT_DEFAULT_COLOR_SPECULAR;
			}
			if (aiGetMaterialColor(ai_Material, AI_MATKEY_COLOR_DIFFUSE, &DiffColor) != AI_SUCCESS)
			{
				DiffColor = MAT_DEFAULT_COLOR_DIFFUSE;
			}
			if (aiGetMaterialColor(ai_Material, AI_MATKEY_COLOR_EMISSIVE, &EmissiveColor) != AI_SUCCESS)
			{
				SpecColor = MAT_DEFAULT_COLOR_EMISSIVE;
			}
			if (aiGetMaterialColor(ai_Material, AI_MATKEY_COLOR_REFLECTIVE, &ReflectiveColor) != AI_SUCCESS)
			{
				SpecColor = MAT_DEFAULT_COLOR_REFLECTIVE;
			}
			if (aiGetMaterialColor(ai_Material, AI_MATKEY_COLOR_AMBIENT, &AmbientColor) != AI_SUCCESS)
			{
				SpecColor = MAT_DEFAULT_COLOR_REFLECTIVE;
			}
			if (aiGetMaterialColor(ai_Material, AI_MATKEY_COLOR_TRANSPARENT, &TransColor) != AI_SUCCESS)
			{
				SpecColor = MAT_DEFAULT_COLOR_REFLECTIVE;
			}

			
			ColorList.push_back
			(// Creates and Adds to the Vector a Color Material
				ColorMaterial
				(
					toGLM(DiffColor),
					toGLM(AmbientColor),
					toGLM(EmissiveColor),
					toGLM(ReflectiveColor),
					toGLM(TransColor),
					toGLM(SpecColor),
					Shine
				)
			);
		}
		
		MatPush.Colors = ColorList;
		TODO("MATERIAL CLASS NEEDS A LOT OF WORK");
		TODO("RETURN A MATERIAL VECTOR. MUST PLACE A COLOR LIST VECTOR IN EACH MATERIAL");
		RETURN.push_back(MatPush);
	}
	return RETURN;
}


void Material::Bind(Shader *_shader)
{
	//_shader->Enable();
	if (Properties.size())
	{
		//Properties[$clr.specular]
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
		_shader->SetTextureUniform("DiffuseTexture", Picture);
		//_shader->SetUniform("DiffuseTexture", Picture.Handle);
		break;
	case NORMALS:
		_shader->SetTextureUniform("NormalTexture", Picture);
		//_shader->SetUniform("NormalTexture", Picture.Handle);
		break;
	}
}


















/// CHANGING OVER TO ASSIMP MODEL LOADER AND MATERIAL SYSTEM
/// Material::Material()
/// {
/// 	Renderer = NULL;
/// 	Skin = new TextureMaterial();
/// }
/// 
/// void Material::Bind()
/// {
/// 	if (Renderer != NULL)
/// 	{
/// 		Renderer->Enable();
/// 		/// Shader::GetActiveShader()->SetUniform3f("MaterialAmbient", SurfaceColor.Ambient);
/// 		/// Shader::GetActiveShader()->SetUniform3f("MaterialDiffuse", SurfaceColor.Diffuse);
/// 		/// Shader::GetActiveShader()->SetUniform3f("MaterialSpecular", SurfaceColor.Specular);
/// 	}
/// 	if (Skin != NULL)
/// 	{
/// 		glActiveTexture(GL_TEXTURE0);
/// 		Skin->DiffuseTexture.Bind();
/// 	///Shader::GetActiveShader()->SetTexture("DiffuseTexture", 0);
/// 	///
/// 	///Diffuse.
/// 	///textureUniform Normals;
/// 	///textureUniform Glow;
/// 	///textureUniform Specular;
/// 
/// 		glActiveTexture(GL_TEXTURE1);
/// 		Skin->NormalsTexture.Bind();
/// 	///	Shader::GetActiveShader()->SetTexture("NormalsTexture", 1);
/// 
/// 		Skin->TextureCoords.Bind();
/// 	}
/// }
/// void Material::Unbind()
/// {
/// 	if (Skin != NULL)
/// 	{
/// 		Skin->DiffuseTexture.Unbind();
/// 		Skin->TextureCoords.Unbind();
/// 	}
/// 	if (Renderer != NULL)
/// 	{
/// 		Shader::GetActiveShader()->Disable();
/// 	}
/// }
/// void Material::Attach(Skinenum style, Image *skin)
/// {
/// 	Initialized = true;
/// 	switch (style)
/// 	{
/// 	case SKIN_DIFFUSE:
/// 		Skin->hasDiffuse = true;
/// 		Skin->DiffuseTexture = *skin;
/// 		break;
/// 	case SKIN_NORMALS:	
/// 		Skin->hasNormals = true;
/// 		Skin->NormalsTexture = *skin;
/// 		break;
/// 	case SKIN_GLOW:	
/// 		Skin->hasGlow = true;
/// 		Skin->GlowTexture = *skin;
/// 		break;
/// 	case SKIN_SPECULAR:
/// 		Skin->hasSpecular = true;
/// 		Skin->SpecularTexture = *skin;
/// 		break;
/// 	///	EngineErrorResponse(0x123, NULL, "Improper Texture Format");
/// 	}
/// };
/// 
/// void Material::SetSurfaceColor(Vec3 A, Vec3 D, Vec3 S)
/// {
/// 	SurfaceColor.Ambient = A;
/// 	SurfaceColor.Diffuse = D;
/// 	SurfaceColor.Specular = S;
/// }
/// void Material::SetTextureCoords(VertexBufferObject<Vec2> coords)
/// {
/// 	Skin->TextureCoords = coords;
/// }
/// 
/// 
/// 
/// 
/// 
/// 
/// 

///     
///     Material* BrickMaterial()
///     {
///     	Material *ret = new Material();
///     	ret->Attach(Shader::GetActiveShader());
///     	ret->Attach(SKIN_DIFFUSE, Image::Manager.GetAsset("BrickDiffuse"));
///     	ret->Attach(SKIN_NORMALS, Image::Manager.GetAsset("BrickNormals"));
///     	return ret;
///     }
///     Material* StoneMaterial()
///     {
///     	Material *ret = new Material();
///     	ret->Attach(Shader::GetActiveShader());
///     	ret->Attach(SKIN_DIFFUSE, Image::Manager.GetAsset("StoneDiffuse"));
///     	ret->Attach(SKIN_NORMALS, Image::Manager.GetAsset("StoneNormals"));
///     	return ret;
///     }
///     Material* MoonMaterial()
///     {
///     	Material *ret = new Material();
///     	ret->Attach(Shader::GetActiveShader());
///     	ret->Attach(SKIN_DIFFUSE, Image::Manager.GetAsset("Moon"));
///     	//ret->Attach(SKIN_NORMALS, Image::Manager.GetAsset("Moon"));
///     	return ret;
///     }