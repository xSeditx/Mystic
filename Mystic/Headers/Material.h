
#pragma once
#include<vector>
#include<map>

#include"Window.h"
#include"Assimp.h"
#include"Texture.h"


/*==============================================================================================================================================================
														MATERIAL CLASS
				Contains the surface Color, Vertex Colors, per Object shader if applicable
==============================================================================================================================================================*/

class Shader;



struct ColorMaterial
{
	enum Type
	{
		DIFFUSE,
		AMBIANT,
		EMISSIVE,
		REFLECTIVE,
		CLEAR,
		SPECULAR,
		SHINE,
		NONE
	}Index;

	ColorMaterial() = default;
	ColorMaterial
	(
		Vec4 _Diffuse,
		Vec4 _Ambient,
		Vec4 _Emissive,
		Vec4 _Reflective,
		Vec4 _Transparent,
		Vec4 _Specular,
		float _Shine
	)
		:
		Diffuse(_Diffuse),
		Ambient(_Ambient),
		Emissive(_Emissive),
		Reflective(_Reflective),
		Transparent(_Transparent),
		Specular(_Specular),
		Shine(_Shine),
		Index(NONE)
	{}

	fRGBA
		Diffuse,
		Ambient,
		Emissive,
		Reflective,
		Transparent,
		Specular;

	float Shine;
	std::vector<fRGBA> Colors;
	std::vector<bool> Has = { false };
	void Set(Type _ty, fRGBA val)
	{ 
		if (Colors.empty())
		{
			Colors.resize((sizeof(Type) / sizeof(int)));
			Has.resize((sizeof(Type) / sizeof(int)));
		}
		Colors[_ty] = val;
		Has[_ty] = true;
		Index = _ty;
	}
};

struct SurfaceMaterial
{// This will likely go in favor of the Enum grabbing the indexed texture
	SurfaceMaterial() = default;
	enum Type
	{
		DIFFUSE,
		NORMALS,
		EMISSIVE,
		DISPLACEMENT,
		HEIGHT,
		AMBIANT,
		LIGHTMAP,
		OPACITY,
		REFLECTION,
		SHININESS
	}Index;


	/// JUST BIT MAP THE TYPES IT HAS IN A 32 BIT INT OR SOMETHING

	Texture *Picture;
	SurfaceMaterial(Type _ty, Texture &_tex)
	{
		Picture = &_tex;
		Index = _ty;
	}
 
	void Bind();
	void Bind(Shader *_shader);
};



struct MaterialProperties
{
	MaterialProperties() = default;
	MaterialProperties
	(
		const char* _key,
		unsigned int _sem,
		unsigned int _index,
		unsigned int _length,
		char *_data,
		aiPropertyTypeInfo _type
	) 
		:
		Key(_key),
		Semantic(_sem),
		Index(_index),
		DataLength(_length),
		Type(_type),
		Data(nullptr)
	{
		Data = new char[_length];
		Data = _data;
	}

	std::string Key;
	unsigned int Semantic = 0;
	unsigned int Index = 0;
	unsigned int DataLength = 0;
	char *Data = 0;

	aiPropertyTypeInfo Type;
};
/// ASSIMP MATERIAL SYSTEM
//http://assimp.sourceforge.net/lib_html/materials.html

struct Material
{
	Material(Shader *_shader) 
		:
		Program(_shader)
	{}

	float Shininess;
	float Shine_Strength;

	void SetShine(float _a, float _b)
	{
		Shininess = _a;
		Shine_Strength = _b;
	}

	Shader *Program;
	std::map<std::string, MaterialProperties> Properties;

	std::vector<SurfaceMaterial> Surfaces;
	std::vector<ColorMaterial> Colors;

	std::string Path;
	std::vector<Material> Extract(const aiScene*_scene);

	void Bind(Shader *_shader);
	void Bind();
	void AddBinaryProperty(const void *pInput, unsigned int pSizeInBytes, const char *pKey, unsigned int type, unsigned int index, aiPropertyTypeInfo pType);
	void AddProperty(const aiString *pInput, const char *pKey, unsigned int type = 0, unsigned int index = 0);
///	void AddProperty(const TYPE *pInput, unsigned int pNumValues, const char *pKey, unsigned int type = 0, unsigned int index = 0);
};


/// aiMaterial()	aiMaterial
/// Clear()	aiMaterial
/// CopyPropertyList(aiMaterial *pcDest, const aiMaterial *pcSrc)	aiMaterial	static
/// Get(const char *pKey, unsigned int type, unsigned int idx, Type *pOut, unsigned int *pMax) const	aiMaterial
/// Get(const char *pKey, unsigned int type, unsigned int idx, Type &pOut) const	aiMaterial
/// GetTexture(aiTextureType type, unsigned int index, aiString *path, aiTextureMapping *mapping = NULL, unsigned int *uvindex = NULL, float *blend = NULL, aiTextureOp *op = NULL, aiTextureMapMode *mapmode = NULL) const	aiMaterial
/// GetTextureCount(aiTextureType type) const	aiMaterial
/// mNumAllocated	aiMaterial
/// mNumProperties	aiMaterial
/// mProperties	aiMaterial
/// RemoveProperty(const char *pKey, unsigned int type = 0, unsigned int index = 0)	aiMaterial
/// ~aiMaterial()	aiMaterial



///Material* BrickMaterial();
///Material* StoneMaterial();
///Material* MoonMaterial();