#include"Uniform.h"

intUniform::intUniform(GLuint shaderid, const char* name)
	:
	uniform_t(shaderid, name),
	Value(0)
{}
vec3Uniform::vec3Uniform(GLuint shaderid, const char* name)
	:
	uniform_t(shaderid, name),
	Value(0)
{}
vec2Uniform::vec2Uniform(GLuint shaderid, const char* name)
	:
	uniform_t(shaderid, name),
	Value(0)
{}
vec4Uniform::vec4Uniform(GLuint shaderid, const  char* name)
	:
	uniform_t(shaderid, name),
	Value(0)
{}
mat4Uniform::mat4Uniform(GLuint shaderid, const  char* name)
	:
	uniform_t(shaderid, name),
	Value(0)
{}
floatUniform::floatUniform(GLuint shaderid, const char* name)
	:
	uniform_t(shaderid, name),
	Value(0)
{}
textureUniform::textureUniform(GLuint shaderid, GLuint texid, const  char* name)
	:
	uniform_t(shaderid, name),
	Value(0),
	Slot(0),
	TextureID(texid)
{}

intUniform::intUniform(const char* name)
	:
	uniform_t(name),
	Value(0)
{}
vec2Uniform::vec2Uniform(const char* name)
	:
	uniform_t(name),
	Value(0)
{}
vec3Uniform::vec3Uniform(const char* name)
	:
	uniform_t(name),
	Value(0)
{}
vec4Uniform::vec4Uniform(const char* name)
	:
	uniform_t(name),
	Value(0)
{}
mat4Uniform::mat4Uniform(const char* name)
	:
	uniform_t(name),
	Value(0)
{}
floatUniform::floatUniform(const char* name)
	:
	uniform_t(name),
	Value(0)
{}
textureUniform::textureUniform(const char* name)
	:
	uniform_t(name),
	Value(0),
	Slot(0)
{}

GLint uniform_t::GetUniformLocation(GLuint _shadeID, const char  *name)
{
	return glGetUniformLocation(_shadeID, name);
}

void intUniform::Set(int _value)
{
	Value = _value;// Bind();
	glUniform1i(Location, Value);
}
void intUniform::Bind()
{
	glUniform1i(Location, Value);
}

void floatUniform::Set(float _value)
{
	Value = _value;// Bind();
	glUniform1f(Location, Value);
}
void floatUniform::Bind()
{
	glUniform1f(Location, Value);
}

void vec2Uniform::Set(Vec2 _value)
{
	Value = _value;
	glUniform2f(Location, Value.x, Value.y);
}
void vec2Uniform::Bind()
{
	glUniform2f(Location, Value.x, Value.y);
}

void vec3Uniform::Set(Vec3 _value)
{
	Value = _value;
	glUniform3f(Location, _value.x, _value.y, _value.z);
}
void vec3Uniform::Bind()
{
	glUniform3f(Location, Value.x, Value.y, Value.z);
}

void vec4Uniform::Set(Vec4 _value)
{
	Value = _value;
	glUniform4f(Location, _value.x, _value.y, _value.z, _value.w);
}
void vec4Uniform::Bind()
{
	glUniform4f(Location, Value.x, Value.y, Value.z, Value.w);
}
void mat4Uniform::Set(Mat4 _value)
{
	Value = _value;
	glUniformMatrix4fv(Location, 1, GL_FALSE, glm::value_ptr(_value));
}
void mat4Uniform::Bind()
{
	glUniformMatrix4fv(Location, 1, GL_FALSE, glm::value_ptr(Value));
}
void textureUniform::Set(int _value)
{
	Value = _value;
	glActiveTexture(GL_TEXTURE0 + _value);
//	glBindTexture(GL_TEXTURE_2D, TextureID);
	glUniform1i(Location, Value);
}
void textureUniform::Bind()
{
	//glActiveTexture(GL_TEXTURE0 + Value);
	glBindTexture(GL_TEXTURE_2D,TextureID);
	glUniform1i(Location, Value);
}
void textureUniform::AddTexture(unsigned int texid)
{
	TextureID = texid;
}

