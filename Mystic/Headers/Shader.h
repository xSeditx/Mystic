﻿#pragma once

#include<stack>
#include<unordered_map>

#include"OpenGL.h"

// My Includes
#include"Common.h"
//#include"Uniform.h"

class uniform_t;
class Texture;

class Shader
{
	enum ErrorType
	{
		Vert,
		Frag,
		Program
	};

public:
	Shader(const char* filepath);
	~Shader();


	void Delete();

	void Enable();
	void Disable();
	void Reload();
	virtual void Bind() {}

	std::unordered_map<std::string, GLuint> ShaderAttribute;

	/* returns the Location slow of a named Uniform */
	GLint GetUniformLocation(const char  *name);

	/* returns the OpenGL ID name */
	GLuint g_ID(){	return GL_Handle;	}


	void T()
	{
		if (ActiveShader.size())
		{
			Print("A");
		}
		else {
			Print("B");
		}
	}
	static GLuint  GetActiveID()     {  return (ActiveShader.size() > 0)  ? ActiveShader.top()->GL_Handle : 0;  }
	static Shader* GetActiveShader() {  return (ActiveShader.size() <= 0) ? nullptr : ActiveShader.top();	    }

    void Push(Shader *shad);
	Shader *Pop();

	GLuint GetResourceLocation();

	int GetFunctionCount(int _shadertype);
	std::string GetFunctionName(int _shadertype, int _index);

	void SetUniform(const char* _name, bool _val);
	void SetUniform(const char* _name, float _val);
	void SetUniform(const char* _name, float _val, float _val2);
	void SetUniform(const char* _name, float _val, float _val2, float _val3);
	void SetUniform(const char* _name, uint64_t _val);
	void SetUniform(const char* _name, int _val);
	void SetUniform(const char* _name, int _val, int _val2);
	void SetUniform(const char* _name, int _val, int _val2, int _val3);
	void SetUniform(const char* _name, Vec2 _val);
	void SetUniform(const char* _name, Vec3 _val);
	void SetUniform(const char* _name, Vec4 _val);
	void SetUniform(const char* _name, Mat3 _val);
	void SetUniform(const char* _name, Mat4 _val);
	void SetTextureUniform(const char *_name, Texture *_tex);
	void SetTextureUniform(const char *_name, uint64_t _tex);

  	int AttributeLocation(const char *_name);
	int GetCSMaxBlocks(); 
	int GetMaxShaderStorageBlocks();


	static GLint GetBoundShader();

	/// FIGURE OUT AN ABSTRACTION FOR THIS
	/// glGetUniformIndices(GLuint program,
	/// 	GLsizei uniformCount,
	/// 	const GLchar **uniformNames,
	/// 	GLuint *uniformIndices);

	void SetShaderStorageBlock(const char *_name, GLuint _slot)
	{
		glShaderStorageBlockBinding(GL_Handle, GetShaderStorageBlock(_name), _slot);
	}
	GLuint GetShaderStorageBlock(const char *_name)
	{
		GLuint result = glGetProgramResourceIndex(GL_Handle, GL_SHADER_STORAGE_BLOCK, _name);
		return result;
	}

protected:
	GLuint GL_Handle;//8
	unsigned int RecursionCount = 0;//8
	GLuint VertID, FragID; // Now these are destroyed at the end.. Find a better solution to handling the Shader errors as Currently I need to store this in order to pass to the Error response

	const char* Filepath;//8
	bool Active = false;//1

	static std::stack<Shader *> ActiveShader;

	GLuint Load();
	void GetShaderError(ErrorType t);
};










//glGenBuffers(1, &uboExampleBlock);
//glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
//glBufferData(GL_UNIFORM_BUFFER, 152, NULL, GL_STATIC_DRAW); // allocate 150 bytes of memory



