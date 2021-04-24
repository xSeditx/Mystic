#pragma once

#include"Asset.h"
#include"Image.h"

#include"OpenGL.h"
/*
Texture Base Class
Discussion: Should Derivation from the Asset class really be a thing or should there be an Asset Manager object in each Asset composing instead of inheriting.
This has already caused a good amount of issues during destruction when deep copies are made as blocks are being invalidated and the Asset class is attempting to be destroyed twice.
Asset derivation needs to cease. It is a pain in the ass and needs to go. It is attempting to double delete objects for whatever reasons and by far if a Manager is needed
an Object of static Asset<T> type should be created in its place instead.
*/

class Texture
{
public:
    NO_COPY_OR_ASSIGNMENT(Texture);

    Texture() = default;

	Texture(std::string file);
	Texture(Bitmap &_image);
    Texture(Bitmap &_image, int x,int y, int width, int height);
	Texture(uint32_t _width, uint32_t _height, GLenum _type = GL_RGB, GLenum Format = GL_RGB, GLenum _wrapMode = GL_REPEAT, GLenum _filtering = GL_LINEAR, GLenum _internalFormat = GL_RGB);
	//GL_RED, GL_RG, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_BGR_INTEGER, GL_RGBA_INTEGER, GL_BGRA_INTEGER, GL_STENCIL_INDEX, GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL.
	GLuint TextureID = 0;
	GLenum Target = GL_TEXTURE_2D;

	Bitmap *Picture;

	uint64_t Handle = 0;

    // = glGetTextureHandleARB(tex);
	// glMakeTextureHandleResidentARB(Handle);
	// Communicate Handle to shader somehow (uniform)
	
	unsigned int
		Type, // GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT32 and GL_DEPTH_COMPONENT32F.
		Format = GL_RGB,
		WrapMode,
		Filtering,
		InternalFormat = GL_RGB;

	void SetTarget(unsigned int param);

	void SetFiltering(unsigned int param);
	void SetMagFiltering(unsigned int param);
	void SetMinFiltering(unsigned int param);

	void SetWrap(unsigned int param);
	void SetWrapX(unsigned int param);
	void SetWrapY(unsigned int param);

	//	OpenGL has a particular syntax for writing its color format enumerants.It looks like this: GL_[components?][size?][type?]
	bool Load();
	inline void Bind();
	inline void Unbind();
	inline void MipmapOn();
	inline void MipmapOff();

	void Render(int _x, int _y, int _w, int _h);
private:
	bool MipmapComplete;
	bool CubemapComplete;
	bool ImageFormatComplete;
};



/*
SkyBox
*/
class Cubemap : public Texture
{
public:
	Cubemap();
	Cubemap(Bitmap *xpos, Bitmap *xneg, Bitmap *ypos, Bitmap *yneg, Bitmap *zpos, Bitmap *zneg);

	void Update(Mat4 _p, Mat4 _m);
	void Create(Bitmap *xpos, Bitmap *xneg, Bitmap *ypos, Bitmap *yneg, Bitmap *zpos, Bitmap *zneg);
	void Delete();
	void CompileShader();
	void CreateCube();

	//VertexArrayObject VAO;

	GLuint vbo_cube_vertices;
	GLuint ibo_cube_indices;
	GLint VertexPositionLOC;
	GLint ProjectionViewMatrixLOC;
//	Shader *Program;
	GLuint ShaderID;
	//http://mi.eng.cam.ac.uk/~agk34/resources/textures/sky/
};



/*
TextureBufferObject
Accept a flat formated block of memory for sequential writing in the Shader
Discussion:
Should this be in the GLBuffers.h header instead of here as it functions more like a Buffer Object then a Texture Object
*/
template<typename T>
struct TextureBufferObject
{
	TextureBufferObject() = default;
	TextureBufferObject(std::string _name, std::vector<T> _data);
	void Bind(GLuint _slot);

	GLuint TextureID;
	GLuint BufferID;
	GLuint UniformLocation;
	size_t Size;
	std::string Name;
	std::vector<T> Data;
};


template<typename T>
TextureBufferObject<T>::TextureBufferObject(std::string _name, std::vector<T> _data)
	:
	Name(_name),
	Data(_data)
{
	glGenBuffers(1, &BufferID);
	glBindBuffer(GL_TEXTURE_BUFFER, BufferID);
	glBufferData(GL_TEXTURE_BUFFER, sizeof(T) * _data.size(), _data.data(), GL_STATIC_DRAW);
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_BUFFER, TextureID);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, BufferID);

}

template<typename T>
void TextureBufferObject<T>::Bind(GLuint _slot)
{
	UniformLocation = glGetUniformLocation(Shader::GetActiveShader()->g_ID(), Name.c_str());
	glUniform1i(UniformLocation, _slot);
	glActiveTexture(GL_TEXTURE0 + _slot);
	glBindTexture(GL_TEXTURE_BUFFER, TextureID);
}




//http://ogldev.atspace.co.uk/www/tutorial25/tutorial25.html
