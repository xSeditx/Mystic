/*===============================================================================================================================================================
//
//                             SHADER MODULE
//
=================================================================================================================================================================*/


#include<string>
#include<iostream>
#include<fstream>
#include<sstream>

#include"Shader.h"
#include"Texture.h"

Shader::Shader(const char* filepath)
	:
	Filepath(filepath),
	GL_Handle(0),
	VertID(0)
{
	GL_Handle = Load();
   // _GL( glUseProgram(GL_Handle) );
}
Shader::~Shader()
{
	Delete();
}
void Shader::Delete()
{
 	Print("Called the Shader Destructor: " << Filepath);
	glDeleteProgram(GL_Handle);
}


/*
This entire Stack system feels like it may be the problem, I have shut down any stacking as I am currently only using a single shader
anyway at the moment yet the problem does not seem to lie there anyway as it still persist
*/
void Shader::Enable()
{
    if (ActiveShader.size() != 0 && ActiveShader.top()->GL_Handle == GL_Handle)
    {
        ++RecursionCount;
    }
    else
    {
        glUseProgram(GL_Handle);
        Push(this);
    }
//if (this->GL_Handle != GetActiveID())
//{
//    _GL( glUseProgram(GL_Handle) ) ;// If so, use it 
//	Push(this);// And push it to the top of the active stack
//}
//else {
//	Push(this);
//}
	
}
void Shader::Disable()
{
    if (RecursionCount > 0)
    {
        --RecursionCount;
    }
    else
    {
        glUseProgram(Pop()->GL_Handle);
    }
//int S = Pop()->GL_Handle;
//if (this->GL_Handle != S)
//{
//    _GL( glUseProgram(S) );
//}
}

//THESE TWO ARE GOING TO GO..... [12/30/18]MAYBE.....
void Shader::Push(Shader *shad)
{
	ActiveShader.push(shad);
	++RecursionCount;
}
Shader* Shader::Pop()
{
	ActiveShader.pop();
	Shader *ret = ActiveShader.top();
	--RecursionCount;
	return ret;
}

GLint Shader::GetUniformLocation(const char  *name)
{
	return 	 glGetProgramResourceLocation(GL_Handle, GL_UNIFORM, name) ;//glGetUniformLocation(ID, name);
}

GLuint Shader::Load()
{

	enum ShaderType{ VERTEX, FRAGMENT, PROGRAM };

	std::ifstream ShaderFile(Filepath, std::ios::in);
	if (!ShaderFile)
	{
		Print("Shader File Not Found:" << Filepath);
		__debugbreak();
//		EngineErrorResponse(ERROR_FILE_NOT_FOUND, 0, (char*)Filepath);
	}
	std::string Line;
	std::string VertexShader;
	std::string FragmentShader;

	ShaderType Type = (ShaderType)0;
	std::stringstream SS[2];
	while (std::getline(ShaderFile, Line))
	{
		if (Line.find("[#SHADER:") != std::string::npos)
		{
			if (Line.find("VERTEX") != std::string::npos)
			{
				Type = VERTEX;
			}
			else if (Line.find("FRAGMENT") != std::string::npos)
			{
				Type = FRAGMENT;
			}
		}
		else
		{
 			SS[(int)Type] << Line << '\n';
		}
	}
	std::string vert = SS[VERTEX].str();
	std::string frag = SS[FRAGMENT].str();
 
	GL_Handle =  glCreateProgram() ;
	VertID =  glCreateShader(GL_VERTEX_SHADER) ;
	FragID =  glCreateShader(GL_FRAGMENT_SHADER) ;

	std::string vertSourceString = vert; 
	std::string fragSourceString = frag; 

	const char* vertSource = vertSourceString.c_str();
	const char* fragSource = fragSourceString.c_str();

	_GL( glShaderSource(VertID, 1, &vertSource, NULL) );
	_GL( glCompileShader(VertID) );
	GetShaderError(Vert);

	_GL( glShaderSource(FragID, 1, &fragSource, NULL) );
	_GL( glCompileShader(FragID) );
	GetShaderError(Frag);

	_GL( glAttachShader(GL_Handle, VertID) ); 
	_GL( glAttachShader(GL_Handle, FragID) );
	_GL( glLinkProgram(GL_Handle) );
	GetShaderError(Program);

	_GL( glValidateProgram(GL_Handle) );
	_GL( glDetachShader(GL_Handle, VertID) );
	_GL( glDetachShader(GL_Handle, FragID) );
	_GL( glDeleteShader(VertID) );
	_GL( glDeleteShader(FragID) );
	return GL_Handle;
}

void Shader::Reload()
{
	enum ShaderType { VERTEX, FRAGMENT, PROGRAM };

	std::ifstream ShaderFile(Filepath, std::ios::in);
	if (!ShaderFile)
	{
		Print("Shader File Not Found:" << Filepath);
		__debugbreak();
		//		EngineErrorResponse(ERROR_FILE_NOT_FOUND, 0, (char*)Filepath);
	}
	std::string Line;
	std::string VertexShader;
	std::string FragmentShader;

	ShaderType Type{ VERTEX };
	std::stringstream SS[2];
	while (std::getline(ShaderFile, Line))
	{
		if (Line.find("[#SHADER:") != std::string::npos)
		{
			if (Line.find("VERTEX") != std::string::npos)
			{
				Type = VERTEX;
			}
			else if (Line.find("FRAGMENT") != std::string::npos)
			{
				Type = FRAGMENT;
			}
		}
		else
		{
			SS[(int)Type] << Line << '\n';
		}
	}
	std::string vert = SS[VERTEX].str();
	std::string frag = SS[FRAGMENT].str();

	VertID = glCreateShader(GL_VERTEX_SHADER);
	FragID = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertSourceString = vert;
	std::string fragSourceString = frag;

	const char* vertSource = vertSourceString.c_str();
	const char* fragSource = fragSourceString.c_str();

	glShaderSource(VertID, 1, &vertSource, NULL);
	glCompileShader(VertID);
	GetShaderError(Vert);

	glShaderSource(FragID, 1, &fragSource, NULL);
	glCompileShader(FragID);
	GetShaderError(Frag);

	glAttachShader(GL_Handle, VertID);
	glAttachShader(GL_Handle, FragID);
	glLinkProgram(GL_Handle);
	GetShaderError(Program);

	glValidateProgram(GL_Handle);
	glDetachShader(GL_Handle, VertID);
	glDetachShader(GL_Handle, FragID);
	glDeleteShader(VertID);
	glDeleteShader(FragID);

    glUseProgram(GL_Handle);
	return;
}





/*
Static Gets and Shader information
*/

// Associated Gets
//----------------
int Shader::GetFunctionCount(int _shadertype)
{//	To query the number of active subroutine functions, pname​ must be GL_ACTIVE_SUBROUTINES.
	int RETURN;
	glGetProgramStageiv(GL_Handle, _shadertype, GL_ACTIVE_SUBROUTINES, &RETURN);
	return RETURN;
}
std::string Shader::GetFunctionName(int _shadertype, int _index)
{
	char RETURN[255] = { 0 };
	GLsizei Length;
	GLsizei Size = 20;
	glGetActiveSubroutineName(GL_Handle, _shadertype, _index, Size, &Length, RETURN);
	return std::string(RETURN);
}
void Shader::GetShaderError(ErrorType T)
{
	GLint length = 0;
	GLint result;
	GLchar  error[1028];
	switch (T)
	{
	case Vert:

		glGetShaderiv(VertID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(VertID, GL_INFO_LOG_LENGTH, &length);

		if (result == GL_FALSE)
		{

			glGetShaderiv(VertID, GL_INFO_LOG_LENGTH, &length);
			//std::vector<char> error(length);
			glGetShaderInfoLog(VertID, length, &length, &error[0]);
			std::cout << "Failed to compile VertexShader: " << &error[0] << std::endl;
			glDeleteShader(VertID);
			//EngineErrorResponse(0x10, VertID, (char*)Filepath);
		}
		break;
	case Frag:

		glGetShaderiv(FragID, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			glGetShaderiv(FragID, GL_INFO_LOG_LENGTH, &length);
			glGetShaderInfoLog(FragID, length, &length, &error[0]);
			Print("Failed to compile Fragment Shader:" << &error[0]);
			glDeleteShader(FragID);
			//	EngineErrorResponse(0x11, FragID, (char*)Filepath);
		}
		break;

	case Program:
		glGetProgramiv(GL_Handle, GL_LINK_STATUS, &result);
		if (result == GL_FALSE)
		{
			glGetProgramiv(GL_Handle, GL_INFO_LOG_LENGTH, &length);
			glGetProgramInfoLog(GL_Handle, length, &length, &error[0]);
			std::cout << "Link Fail " << &error[0] << std::endl;
			glDeleteShader(GL_Handle);
			//	EngineErrorResponse(0x12, ID, (char*)Filepath);
		}
		break;
	};
}

// Statics
//--------

// STATIC 
std::stack<Shader *> Shader::ActiveShader;


GLint Shader::GetBoundShader()
{
	GLint program;
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);
	return program;
}

int Shader::GetCSMaxBlocks()
{//	data returns one value, the maximum number of active shader storage blocks that may be accessed by a compute shader.
	int results = 0;
	glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, &results);
	return results;
}
int Shader::GetMaxShaderStorageBlocks()
{//	data returns one value, the maximum total number of active shader storage blocks that may be accessed by all active shaders.
	int results = 0;
	glGetIntegerv(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, &results);
	return results;
}









void Shader::SetUniform(const char* _name, bool _val)
{
    _GL( glUniform1i(glGetUniformLocation(GL_Handle, _name), _val) );
}
void Shader::SetUniform(const char* _name, float _val)
{
    _GL(glUniform1f(glGetUniformLocation(GL_Handle, _name), _val));
}
void Shader::SetUniform(const char* _name, float _val, float _val2)
{
    _GL(glUniform2f(glGetUniformLocation(GL_Handle, _name), _val, _val2));
}
void Shader::SetUniform(const char* _name, float _val, float _val2, float _val3)
{
    _GL(glUniform3f(glGetUniformLocation(GL_Handle, _name), _val, _val2, _val3));
}
void Shader::SetUniform(const char* _name, int _val)
{
    _GL(glUniform1i(glGetUniformLocation(GL_Handle, _name), _val));
}



void Shader::SetUniform(const char* _name, uint64_t _val)
{
	GLuint Location = glGetUniformLocation(GL_Handle, _name);
	//glUniform1ui(Location, _val);
    _GL(glProgramUniformui64NV(GL_Handle, glGetUniformLocation(GL_Handle, _name), _val));
}



void Shader::SetUniform(const char* _name, int _val, int _val2)
{
    _GL(glUniform2i(glGetUniformLocation(GL_Handle, _name), _val, _val2));
}
void Shader::SetUniform(const char* _name, int _val, int _val2, int _val3)
{
	GLuint Location = glGetUniformLocation(GL_Handle, _name);
    _GL(glUniform3i(glGetUniformLocation(GL_Handle, _name), _val, _val2, _val3));
}
void Shader::SetUniform(const char* _name, Vec2 _val)
{
    _GL(glUniform2fv(glGetUniformLocation(GL_Handle, _name), 1, &_val[0]));
}
void Shader::SetUniform(const char* _name, Vec3 _val)
{
    _GL(glUniform3fv(glGetUniformLocation(GL_Handle, _name), 1, &_val[0]));
}
void Shader::SetUniform(const char* _name, Vec4 _val)
{
    _GL(glUniform4fv(glGetUniformLocation(GL_Handle, _name), 1, &_val[0]));
}
void Shader::SetUniform(const char* _name, Mat3 _val)
{
    _GL(glUniformMatrix3fv(glGetUniformLocation(GL_Handle, _name), 1, GL_FALSE, glm::value_ptr(_val)));
}
void Shader::SetUniform(const char* _name, Mat4 _val)
{
    _GL(glUniformMatrix4fv(glGetUniformLocation(GL_Handle, _name), 1, GL_FALSE, glm::value_ptr(_val)));
}
void Shader::SetTextureUniform(const char *_name, Texture *_tex)
{
    _GL(glProgramUniformui64NV(GL_Handle, glGetUniformLocation(GL_Handle, _name), _tex->Handle));
}
void Shader::SetTextureUniform(const char *_name, uint64_t _tex)
{
    _GL(glProgramUniformui64NV(GL_Handle, glGetUniformLocation(GL_Handle, _name), _tex));
}

int Shader::AttributeLocation(const char *_name)
{
	return glGetAttribLocation(GL_Handle, _name);
}

/* 

Fragment Outputs
Fragment color numbers and indices can be queried with these functions :

GLint glGetFragDataLocation(GLuint program​, const char * name​);
GLint glGetFragDataIndex(GLuint program​, const char * name​);

GL_PROGRAM_INPUT: The set of active user-defined inputs to the first shader stage in this program. 
If the first stage is a Vertex Shader, then this is the list of active attributes. If the program only contains a Compute Shader, then there are no user-defined inputs.

GL_PROGRAM_OUTPUT: The set of active user-defined outputs from the final shader stage in this program.
If the final stage is a Fragment Shader, then this represents the fragment outputs that get written to individual color buffers.
If the program only contains a Compute Shader, then there are no user-defined outputs.

*/

 //glActiveTexture(GL_TEXTURE0 + _slot);
 //glBindTexture(GL_TEXTURE_2D, _tex);
 //GLuint Location = glGetUniformLocation(ID, _name);
 //glUniform1i(Location, _slot);

//glActiveTexture(GL_TEXTURE0 + _slot);
//_tex->Bind();
//GLuint Location = glGetUniformLocation(ID, _name);
//glUniform1i(Location, _slot);