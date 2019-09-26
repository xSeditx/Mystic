#include"Texture.h"
#include"Shader.h"
#include"Camera.h"
#include"Common.h"
#include"Primitive.h"

Texture::Texture(std::string file)
	:
//	Asset(file),
	Type(0)
{
	TextureID = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
}
Texture::Texture(Image &image)
	:
//	Asset("Texture" + std::to_string(ElementCount)),
	Picture(&image),
	Type(0),
	TextureID(0),
	MipmapComplete(0)
{
	glGenTextures(1, &TextureID);
    Load();
}
Texture::Texture(Image &image, int _x, int _y, int _width, int _height)

{// Generate a Texture from a Sub image 
    Picture = new Image();
    *Picture = Image::FlippedImage(image);

    glGenTextures(1, &TextureID);
    Bind();

    SetMinFiltering(GL_LINEAR_MIPMAP_LINEAR);  // Texture Layout when sampling past texture
    SetMagFiltering(GL_LINEAR);
    SetWrap(GL_REPEAT); // Texture Layout when sampling past texture

    size_t Stride = Picture->Channels;
    size_t SrcByteHeight = Picture->Size.y * Stride;
    size_t SrcByteWidth = Picture->Size.x * Stride;

    size_t DestByteHeight = _height * Stride;
    size_t DestByteWidth = _width * Stride;

    size_t RowSize = DestByteWidth;

    size_t DestIndex = 0, SourceIndex = 0;// (_x * Stride);

    unsigned char *TempImage = new unsigned char[DestByteHeight * DestByteWidth];
    size_t Buffered = 0;
    if (DestByteWidth + (_x * Stride) > SrcByteWidth)
    {
        RowSize -= (_x * Stride);
        DestByteWidth -= (_x * Stride);
        Buffered = (_x * Stride);
    }

    int Counter = 0;
    for (int py = 0; py < _height; ++py)
    {
        unsigned char *Src = &Picture->Data[SourceIndex];
        unsigned char *Dest = &TempImage[DestIndex];

        memcpy(Dest, Src, RowSize);

        SourceIndex += SrcByteWidth;
        DestIndex += DestByteWidth;

        Counter++;
    }
    Picture->Data = TempImage;


    *Picture = Image::FlippedImage(*Picture);

    if (Picture->Channels == 4)
    {
        Format = GL_RGBA;
        InternalFormat = GL_RGBA;
    }
    else if (Picture->Channels == 3)
    {
        Format = GL_RGB;
        InternalFormat = GL_RGB;
    }


    glTexImage2D(Target, 0, InternalFormat, (Picture->Size.x), Picture->Size.y, 0, Format, GL_UNSIGNED_BYTE, Picture->Data); // 	_GL(glTexImage2D(Target, 0, GL_RGB, Picture.Size.x, Picture.Size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, Picture.Data));
    glGenerateMipmap(Target);

    Handle = glGetTextureHandleARB(TextureID);
    glMakeTextureHandleResidentARB(Handle);
    delete (TempImage);
    Unbind();
}



//glGetTextureSubImage
//(
//   GL_TEXTURE_2D,
//   NULL,
//   _x, _y,
//   NULL,
//   _width, _height,
//   1,
//   tex.Format, GL_UNSIGNED_BYTE,
//   DestByteHeight * DestByteWidth,
//   TempImage
//);
//Picture = new Image(TempImage, Vec2(_width, _height), image.Channels);




//==============================================================================================================================================================================
// FLIP THE IMAGE TO PLAY NICE WITH OPENGL
//==============================================================================================================================================================================
//{
//    unsigned char *Data = new unsigned char[image.DataSize];
//    size_t RowSize = image.Size.x * sizeof(unsigned char) * image.Channels;
//    size_t ImageSize = RowSize * image.Size.y;
//
//    unsigned char *TempImage = new unsigned char[ImageSize] {0};
//
//    for_loop(I, image.Size.y)
//    {
//        memcpy(TempImage + ((size_t)I * RowSize), (Data + (ImageSize - (((size_t)I + 1) * RowSize))), RowSize);// Copy bottom row into top of Temp buffer
//    }
//
//    memcpy(Data, TempImage, ImageSize);
//    delete[](TempImage);
//}
//==============================================================================================================================================================================


bool Texture::Load()
{
	Bind();

	SetMinFiltering(GL_LINEAR_MIPMAP_LINEAR);  // Texture Layout when sampling past texture
	SetMagFiltering(GL_LINEAR);

	SetWrap(GL_REPEAT); // Texture Layout when sampling past texture
 

	if (Picture->Channels == 4)
	{
		Format = GL_RGBA;
		InternalFormat = GL_RGBA;
	}
	else if (Picture->Channels == 3)
	{
		Format = GL_RGB;
		InternalFormat = GL_RGB;
	}
	_GL(glTexImage2D(Target, 0, InternalFormat, Picture->Size.x, Picture->Size.y, 0, Format, GL_UNSIGNED_BYTE, Picture->Data)); // 	_GL(glTexImage2D(Target, 0, GL_RGB, Picture.Size.x, Picture.Size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, Picture.Data));
	glGenerateMipmap(Target);
	Handle = glGetTextureHandleARB(TextureID);
	glMakeTextureHandleResidentARB(Handle);

	Unbind();
	return true;
}

//Target
void Texture::SetTarget(unsigned int param)
{
	Target = param;
}
//Texture Wrapping
void Texture::SetWrap(unsigned int param)
{
	glTexParameteri(Target, GL_TEXTURE_WRAP_S, param);
	glTexParameteri(Target, GL_TEXTURE_WRAP_T, param);
}
void Texture::SetWrapX(unsigned int param)
{
	glTexParameteri(Target, GL_TEXTURE_WRAP_S, param);

}
void Texture::SetWrapY(unsigned int param)
{
	glTexParameteri(Target, GL_TEXTURE_WRAP_T, param);
}
// Filtering
void Texture::SetFiltering(unsigned int param)
{
	glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, param);
	glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, param);
}
void Texture::SetMagFiltering(unsigned int param)
{
	_GL(glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, param ));
}
void Texture::SetMinFiltering(unsigned int param)
{
	_GL(glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, param ));
}
// State Handler
void Texture::Bind()
{
	_GL(glBindTexture(Target, TextureID));
}
void Texture::Unbind()
{
	glBindTexture(Target, 0);
}
// MipMapping
void Texture::MipmapOn()
{
	glTexParameteri(Target, GL_GENERATE_MIPMAP, GL_TRUE);
}
void Texture::MipmapOff()
{
	glTexParameteri(Target, GL_GENERATE_MIPMAP, GL_TRUE);
}



void Texture::Render(int _x, int _y, int _w, int _h)
{

	Mat4 Transform = glm::mat4(1.0f); //  Set Identity and Rotate all axis followed with the Translation.
	Transform = glm::translate(Transform, Vec3(_x, _y, 0));
	Transform = glm::rotate(Transform, glm::radians(0.f), Vec3(1.0f, 0.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(0.f), Vec3(0.0f, 1.0f, 0.0f));
	Transform = glm::rotate(Transform, glm::radians(0.f), Vec3(0.0f, 0.0f, 1.0f));

	Shader::GetActiveShader()->SetUniform("ProjectionMatrix", OrthographicMatrix(640, 480));
	Shader::GetActiveShader()->SetUniform("ViewMatrix", Mat4(1));
 	Shader::GetActiveShader()->SetUniform("ModelMatrix", Transform);

	Plane temp(Vec3(0), Vec3(0), (float)_w, (float)_h);
	Shader::GetActiveShader()->SetTextureUniform("DiffuseTexture", this);
	temp.Render(*Shader::GetActiveShader());
}







Cubemap::Cubemap()
{
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

//Projection = glm::perspective(45.0f, (float)640 / (float)480, 0.1f, 100.0f);
//View = glm::mat4(1.0f);
//Model = glm::scale(glm::mat4(1.0f), glm::vec3(50, 50, 50));
}
Cubemap::Cubemap(Image *xpos, Image *xneg, Image *ypos, Image *yneg, Image *zpos, Image *zneg)
{
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	Create(xpos, xneg, ypos, yneg, zpos, zneg);
}

void Cubemap::Update(Mat4 _p, Mat4 _m)
{
	//if (kb.getKeyState(KEY_UP))    alpha += 180.0f*elapsed0;
	//if (kb.getKeyState(KEY_DOWN))  alpha -= 180.0f*elapsed0;
	//if (kb.getKeyState(KEY_LEFT))  beta -= 180.0f*elapsed0;
	//if (kb.getKeyState(KEY_RIGHT)) beta += 180.0f*elapsed0;
		//jp[0] = js.joystickPosition(0);
		//alpha += jp[0].y*deltaTime*180.0f;
		//beta += jp[0].x*deltaTime*180.0f;
//	float alpha = 0, beta = 0;
	//glm::mat4 RotateX = glm::rotate(glm::mat4(1.0f), alpha, glm::vec3(-1.0f, 0.0f, 0.0f));
//	glm::mat4 RotateY = glm::rotate(RotateX, beta, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 M = _p * _m;
	//glUniformMatrix4fv(ProjectionViewMatrixLOC, 1, GL_FALSE, glm::value_ptr(PV));
	//Program->Enable();
	glUseProgram(ShaderID);
   //     Program->SetUniform("PV", M);
        glDepthMask(GL_FALSE);
        // ... set view and projection matrix
        glUniform4fv(glGetAttribLocation(ShaderID, "ProjectionViewMatrix"), 1, glm::value_ptr(M));
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_indices);

        glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // ... draw rest of the scene
        glDepthMask(GL_TRUE);
	//Program->Disable();
		
}
void Cubemap::Create(Image *xpos, Image *xneg, Image *ypos, Image *yneg, Image *zpos, Image *zneg)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, xpos->Size.x, xpos->Size.y, 0, xpos->Channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, xpos->Data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, xneg->Size.x, xneg->Size.y, 0, xneg->Channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, xneg->Data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, ypos->Size.x, ypos->Size.y, 0, ypos->Channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, ypos->Data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, yneg->Size.x, yneg->Size.y, 0, yneg->Channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, yneg->Data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, zpos->Size.x, zpos->Size.y, 0, zpos->Channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, zpos->Data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, zneg->Size.x, zneg->Size.y, 0, zneg->Channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, zneg->Data);
//  Vec3 Vertices[] = 
//  {
//  	Vec3( -1.0f,  1.0f, -1.0f),
//  	Vec3( -1.0f, -1.0f, -1.0f),
//  	Vec3(  1.0f, -1.0f, -1.0f),
//  	Vec3(  1.0f, -1.0f, -1.0f),
//  	Vec3(  1.0f,  1.0f, -1.0f),
//  	Vec3( -1.0f,  1.0f, -1.0f),
//  
//  	Vec3( -1.0f, -1.0f,  1.0f),
//  	Vec3( -1.0f, -1.0f, -1.0f),
//  	Vec3( -1.0f,  1.0f, -1.0f),
//  	Vec3( -1.0f,  1.0f, -1.0f),
//  	Vec3( -1.0f,  1.0f,  1.0f),
//  	Vec3( -1.0f, -1.0f,  1.0f),
//  
//  	Vec3(  1.0f, -1.0f, -1.0f),
//  	Vec3(  1.0f, -1.0f,  1.0f),
//  	Vec3(  1.0f,  1.0f,  1.0f),
//  	Vec3(  1.0f,  1.0f,  1.0f),
//  	Vec3(  1.0f,  1.0f, -1.0f),
//  	Vec3(  1.0f, -1.0f, -1.0f),
//  
//  	Vec3( -1.0f, -1.0f,  1.0f),
//  	Vec3( -1.0f,  1.0f,  1.0f),
//  	Vec3(  1.0f,  1.0f,  1.0f),
//  	Vec3(  1.0f,  1.0f,  1.0f),
//  	Vec3(  1.0f, -1.0f,  1.0f),
//  	Vec3( -1.0f, -1.0f,  1.0f),
//  
//  	Vec3( -1.0f,  1.0f, -1.0f),
//  	Vec3(  1.0f,  1.0f, -1.0f),
//  	Vec3(  1.0f,  1.0f,  1.0f),
//  	Vec3(  1.0f,  1.0f,  1.0f),
//  	Vec3( -1.0f,  1.0f,  1.0f),
//  	Vec3( -1.0f,  1.0f, -1.0f),
//  
//  	Vec3( -1.0f, -1.0f, -1.0f),
//  	Vec3( -1.0f, -1.0f,  1.0f),
//  	Vec3(  1.0f, -1.0f, -1.0f),
//  	Vec3(  1.0f, -1.0f, -1.0f),
//  	Vec3( -1.0f, -1.0f,  1.0f),
//  	Vec3(  1.0f, -1.0f,  1.0f)
//  };
//  
//  VAO.Attach(VERTEX, new VertexBufferObject<Vec3>(&Vertices[0], 32));

	CompileShader();
	CreateCube();
}
void Cubemap::Delete()
{
	glDeleteTextures(1, &TextureID);
}
void Cubemap::CompileShader()
{
	GLuint ShaderV = glCreateShader(GL_VERTEX_SHADER);
	GLuint ShaderF = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* vShaderSource =
		R"(
                #version 330
                in vec3 VertexPosition;
                uniform mat4 ProjectionViewMatrix;
                
                out vec3 texCoord;
                void main()
                {
                	gl_Position = ProjectionViewMatrix * vec4(VertexPosition, 1.0);
                	texCoord = VertexPosition;
                })";

	const GLchar* fShaderSource =
		R"(
                #version 330
                in vec3 texCoord;
                out vec4 fragColor;
                uniform samplerCube cubemap;
                
                void main(void)
                {
                	fragColor = texture(cubemap, texCoord);
                })";

	glShaderSource(ShaderV, 1, &vShaderSource, NULL);
	glShaderSource(ShaderF, 1, &fShaderSource, NULL);
//	delete[] vShaderSource;
	//delete[] fShaderSource;

	glCompileShader(ShaderV);
	glCompileShader(ShaderF);

	ShaderID = glCreateProgram();

	glAttachShader(ShaderID, ShaderV);
	glAttachShader(ShaderID, ShaderF);

	glLinkProgram(ShaderID);
	glUseProgram(ShaderID);

	// shader logs
	int  vlength, flength;
	char vlog[2048], flog[2048];
	glGetShaderInfoLog(ShaderV, 2048, &vlength, vlog);
	glGetShaderInfoLog(ShaderF, 2048, &flength, flog);
	Print(vlog << std::endl << std::endl << flog);

	// grab the pvm matrix and vertex location from our shader program
	ProjectionViewMatrixLOC = glGetUniformLocation(ShaderID, "ProjectionViewMatrix");
	VertexPositionLOC = glGetAttribLocation(ShaderID, "VertexPosition");
}
void Cubemap::CreateCube()
{
	// cube vertices for vertex buffer object
	GLfloat cube_vertices[] = {
	  -1.0,  1.0,  1.0,
	  -1.0, -1.0,  1.0,
	   1.0, -1.0,  1.0,
	   1.0,  1.0,  1.0,
	  -1.0,  1.0, -1.0,
	  -1.0, -1.0, -1.0,
	   1.0, -1.0, -1.0,
	   1.0,  1.0, -1.0,
	};

	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	// cube indices for index buffer object
	GLushort cube_indices[] = {
	  0, 1, 2, 3,
	  3, 2, 6, 7,
	  7, 6, 5, 4,
	  4, 5, 1, 0,
	  0, 3, 7, 4,
	  1, 2, 6, 5,
	};

	glGenBuffers(1, &ibo_cube_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(VertexPositionLOC);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (char*)0);
}








// Dragon model
//https://free3d.com/3d-model/black-dragon-rigged-and-game-ready-92023.html

// glTexParameteri(Target, Pname, Param);
// Target:
//     GL_TEXTURE_1D, GL_TEXTURE_1D_ARRAY, GL_TEXTURE_2D, GL_TEXTURE_2D_ARRAY,
//     GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_2D_MULTISAMPLE_ARRAY, GL_TEXTURE_3D,
//     GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_ARRAY, or GL_TEXTURE_RECTANGLE.
// Pname:
//   Specifies the symbolic name of a single - valued texture parameter.pname can be one of the following:
//     GL_DEPTH_STENCIL_TEXTURE_MODE, GL_TEXTURE_BASE_LEVEL, GL_TEXTURE_COMPARE_FUNC,
//     GL_TEXTURE_COMPARE_MODE, GL_TEXTURE_LOD_BIAS, GL_TEXTURE_MIN_FILTER, 
//     GL_TEXTURE_MAG_FILTER, GL_TEXTURE_MIN_LOD, GL_TEXTURE_MAX_LOD, GL_TEXTURE_MAX_LEVEL, 
//     GL_TEXTURE_SWIZZLE_R, GL_TEXTURE_SWIZZLE_G, GL_TEXTURE_SWIZZLE_B, 
//     GL_TEXTURE_SWIZZLE_A, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T,  GL_TEXTURE_WRAP_R.

// glActiveTexture(GL_TEXTURE0 + ID);
// Sampling mode for blending textals
// glPixelStorei(GL_PACK_ALIGNMENT, 2);

//=SOIL_LOAD_AUTO,
//SOIL_CREATE_NEW_ID,
//SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA