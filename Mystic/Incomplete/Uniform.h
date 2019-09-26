#pragma once

#include"Common.h"
#include"OpenGL.h"

class uniform_t
{
public:
	uniform_t()
		:
		ShaderID(NULL),
		Name(NULL),
		Location(NULL),
		Size(0),
		value_ptr(nullptr)
	{}

	uniform_t(const char* name)
		:
		ShaderID(NULL),
		Name(name),
		Location(NULL),
		Size(0),
		value_ptr(nullptr)
	{
	}

	uniform_t(GLuint shaderID, const char* name)
		:
		ShaderID(shaderID),
		Name(name),
		Location(GetUniformLocation(shaderID, name)),
		Size(0),
		value_ptr(nullptr)
	{
	}

	virtual void Bind() = pure_virtual;

	void SetShaderID(GLuint& _id)
	{ 
		if (Name != NULL)
		{
			Location = glGetUniformLocation(_id, Name);
		}
		ShaderID = _id; 
	}
protected:
	GLuint Location;
	const char *Name;
	///------ Implemented a GetUniformLocation that also takes in the ShaderID so that might fix this problem.....
	GLuint ShaderID; /// Currently only needed to get the Uniform location but might be useful down the road
	 /// for cache reasons this I think needs to go, at least replace it with a pointer or just the shader program ID like it represents anyway
private:
	/// For future implementation linking the Union to a variable on the CPU side via pointer
	void *value_ptr;
	size_t Size;

	static GLint GetUniformLocation(GLuint shadeId, const char  *name);
};

class intUniform : public uniform_t
{
public:
	intUniform(GLuint shaderid, const char* name);
	intUniform(const char* name);

	void Set(int _value);
	void Bind() override;
private:
	GLint Value;
};
class floatUniform : public uniform_t
{
public:
	floatUniform(GLuint shaderid, const  char* name);
	floatUniform(const char* name);

	void Set(float _value);
	void Bind() override;
private:
	float Value;
};
class vec2Uniform : public uniform_t
{
public:
	vec2Uniform(GLuint shaderid, const char* name);
	vec2Uniform(const char* name);

	void Set(Vec2 _value);
	void Bind() override;
private:
	Vec2 Value;
};
class vec3Uniform : public uniform_t
{
public:
	vec3Uniform(GLuint shaderid, const char* name);
	vec3Uniform(const char* name);

	void Set(Vec3 _value);
	void Bind() override;
private:
	Vec3 Value;
};
class vec4Uniform : public uniform_t
{
public:
	vec4Uniform(GLuint shaderid, const char* name);
	vec4Uniform(const char* name);

	void Set(Vec4 _value);
	void Bind() override;
private:
	Vec4 Value;
};
class mat4Uniform : public uniform_t
{
public:
	mat4Uniform() :
		uniform_t(),
		Value(0)
	{
	}

	mat4Uniform(GLuint shaderid, const  char* name);
	mat4Uniform(const char* name);

	void Set(Mat4 _value);
	void Bind() override;
private:
	Mat4 Value;
};
class textureUniform :public uniform_t
{
public:
	textureUniform() = default;
	textureUniform(GLuint shaderid, GLuint texid,const char* name);
	textureUniform(const char* name);

	void Set(int _value);
	void AddTexture(unsigned int texid);
	void Bind() override;
private:
	GLuint Value = 0;
	GLuint Slot = 0;
	GLuint TextureID;
};
