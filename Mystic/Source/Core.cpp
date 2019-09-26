#include"Common.h"
#include"OpenGL.h"

bool GLLogCall(const char *function, const char *file, int line)
{
	GLenum error = glGetError();
	if (error != 0)
	{
		//GLEW!  Print("[OpenGL ERROR]: %s\n" << gladGetErrorString(error));
		Print("Error Number: " << error);
		Print("FILE: " << file);
		Print("LINE NUMBER: " << line);
		Print("FUNCTION: " << function);

		system("PAUSE");
		__debugbreak();
		return false;
	}
	return true;
}
void GLCheckError()
{
	GLenum err;
	while (err = glGetError())
	{
		std::cout << "GL Check Error:" << err;
	}
}
void GLClearError()
{
    GLenum Error = -1;
    while ((Error = glGetError()) != GL_NO_ERROR)
    {
        if(Error)Print("Error:" << Error);
    };
}

std::ostream& operator<<(std::ostream &lhv, Vec2 const &rhv)
{
	lhv << " X:" << rhv.x << " Y:" << rhv.y;
	return lhv;
}
std::ostream& operator<<(std::ostream &lhv, Vec3 const &rhv)
{
	lhv << " X:" << rhv.x << " Y:" << rhv.y << " Z:" << rhv.z;
	return lhv;
}
std::ostream& operator<<(std::ostream &lhv, Vec4 const &rhv)
{
	lhv << " X:" << rhv.x << " Y:" << rhv.y << " Z:" << rhv.z << " W:" << rhv.w;
	return lhv;
}


GLenum ActiveTextureUnit()
{
	//	GL_ACTIVE_TEXTURE
	//	params returns a single value indicating the active multitexture unit.The initial value is GL_TEXTURE0.See glActiveTexture.
	GLenum ActiveTexture;
	glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&ActiveTexture);
	return ActiveTexture;
}
GLenum ActiveTexture()
{
	//	GL_TEXTURE_BINDING_2D
	//	params returns a single value, the name of the texture currently bound to the target GL_TEXTURE_2D for the active multitexture unit.The initial value is 0. See glBindTexture.
	GLenum ActiveTexture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&ActiveTexture);
	return ActiveTexture;
}

GLuint ActiveShader()
{
	GLint program;
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);
	return program;
}
GLuint BoundSampler()
{
	GLint last_sampler;
	glGetIntegerv(GL_SAMPLER_BINDING, &last_sampler);
	return last_sampler;
}

GLuint ActiveVAO()
{
	//GL_ARRAY_BUFFER_BINDING
	//params returns a single value, the name of the buffer object currently bound to the target GL_ARRAY_BUFFER.If no buffer object is bound to this target, 0 is returned.The initial value is 0. See glBindBuffer.
	GLint last_array_buffer;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
	return last_array_buffer;
}

//
//GLuint *PolygonMode()
//{
//	GLint last_polygon_mode[2]; 
//	glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
//	return *last_polygon_mode;
//}

//#include"Utility.h"
Vec4 ActiveViewport()
{
	GLint last_viewport[4];
	glGetIntegerv(GL_VIEWPORT, last_viewport);
	Vec4 results;
	results.x = last_viewport[0];
	results.y = last_viewport[1];
	results.z = last_viewport[2];
	results.w = last_viewport[3];
	return results;
}

bool GetBlendState()
{// returns a single boolean value indicating whether blending is enabled.The initial value is GL_FALSE.See glBlendFunc.
	GLboolean results;
	glGetBooleanv(GL_BLEND, &results);
	return results;
}

