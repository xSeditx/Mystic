#include"GLBuffers.h"
///=================================================================================================================
///=================================================================================================================
Attribute::Attribute()
    :
    AttributeType(BufferTypes::NONE),
    ElementCount(NULL),
    Address(NULL)
{}
Attribute::Attribute(BufferTypes t)
    :
    AttributeType(t)
{}

void Attribute::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, GL_Handle);
}
void Attribute::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
}
void Attribute::Release()
{// Should I just orphan this? Should I delete it? I wish to reclaim the name and free any allocation associate with it.
    Print("Yeah, idk what the fuck to do with this. Release in VertexBufferObject class"); // Maybe I just need to delete it and forget it ever existed
    __debugbreak();
}
void Attribute::Destroy()
{// Totaly remove the VAO from Graphic Memory and recover it by the Operating system
    glDeleteBuffers(1, &GL_Handle);
    delete(this); // This may or maynot work.
}

void *Attribute::Map(GLenum accessflags)
{// Map the OpenGL Buffer and return pointer to that memory
    Bind();
    void *results = glMapBuffer(GL_ARRAY_BUFFER, accessflags);
    return results;
}
void *Attribute::MapRange(int offset, int count, GLenum accessflags)
{// Possibly only avalible on later versions of OpenGL > 3.0
    Bind();
    return glMapBufferRange(GL_ARRAY_BUFFER, offset, count, accessflags);
}

size_t Attribute::MappedSize()
{
    Bind();
    int sz;
    if (AttributeType == INDICE)
    {
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &sz);
    }
    else
    {
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &sz);
    }
    return sz;
}
bool Attribute::isMapped()
{
    Bind();
    int RETURN;
    if (AttributeType == INDICE)
    {
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_MAPPED, &RETURN);
    }
    else
    {
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_MAPPED, &RETURN);
    }
    return (RETURN != 0);
}

///=================================================================================================================



///=================================================================================================================
///=================================================================================================================
 
ElementArrayBuffer::ElementArrayBuffer(GLuint *_data, size_t _count)
    :
    GL_Handle(NULL),
    ElementCount(_count)
{
    Data = new GLuint[_count];
    Data = _data;/// HMMM....

    glGenBuffers(1, &GL_Handle);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(GLuint), _data, MYSTIC_DEFAULT_BUFFER_ACCESS);
}
ElementArrayBuffer::ElementArrayBuffer(GLenum _access, GLuint *_data, size_t _count)
    :
    GL_Handle(NULL),
    ElementCount(_count)
{
    Data = new GLuint[_count];
    Data = _data;/// HMMM....

    glGenBuffers(1, &GL_Handle);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(GLuint), _data, _access);
}

void ElementArrayBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_Handle);
}
void ElementArrayBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
///=================================================================================================================



VertexArrayObject::VertexArrayObject()
    :
    GL_Handle(NULL),
    ElementCount(NULL)
{
    Buffers.reserve(5);
}
VertexArrayObject::VertexArrayObject(VertexBufferObject<Vec3> &vbo)
    :
    ElementCount(0),
    GL_Handle(NULL)
{
    _GL(glGenVertexArrays(1, &GL_Handle));
    Buffers.reserve(5);
}
#ifdef BINDLESS_ATTRIBUTES
void VertexArrayObject::Bind() {}
void VertexArrayObject::Unbind() {}

#else
void VertexArrayObject::Bind()
{
    _GL(glBindVertexArray(GL_Handle));
}
void VertexArrayObject::Unbind()
{
    _GL(glBindVertexArray(0));
}

#endif
void VertexArrayObject::Render()
{
    Bind();
    _GL(glDrawElements(GL_TRIANGLES, ElementCount, GL_UNSIGNED_INT, nullptr));
}
#include"Texture.h"
FrameBufferObject::FrameBufferObject(std::pair<Texture&, AttachmentPoint_t> _attachments)
{
	glGenFramebuffers(1, &FrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, _attachments.second, _attachments.first.Target, _attachments.first.Handle, 0);//0 is mipmap level

	glDrawBuffer(DrawBuffer);// 
	glReadBuffer(ReadBuffer);

	ValidateFrameBuffer();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

FrameBufferObject::FrameBufferObject(int _width, int _height, GLenum _datatype , GLenum _internal, GLenum _format)
    :
    Width(_width),
    Height(_height),
    DataType(_datatype),
    InternalFormat(_internal),
    PixelFormat(_format)
{
    glGenFramebuffers(1, &FrameBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);

    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_2D, TextureID);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureID, 0);

    glGenTextures(1, &DepthTexture);
    glBindTexture(GL_TEXTURE_2D, DepthTexture);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthTexture, 0);


    ValidateFrameBuffer();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
void FrameBufferObject::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void FrameBufferObject::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FrameBufferObject::ValidateFrameBuffer()
{
    std::string
        ErrorString,
        PossibleSolution;
    int error_number = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (error_number)
    {
    case GL_FRAMEBUFFER_COMPLETE:

        ErrorString = "No Error, FrameBuffer Healthy";
        PossibleSolution = "You did something right for a change, High five yourself!";
        break;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:

        ErrorString = "Incomplete FrameBuffer";
        PossibleSolution = "Check FrameBuffer Textures and Attachment Points";
        break;

    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:

        ErrorString = "Incomplete FrameBuffer Dimensions";
        PossibleSolution = "Not all attached Images have the same width and height. Check all Image sizes";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:

        ErrorString = "FrameBuffer Missing Attachment";
        PossibleSolution = "Attach an Image to the FrameBuffer";
        break;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        ErrorString = "FrameBuffer Unsupported";
        PossibleSolution = "Try a Different format, or combination of, for the Attached Textures";
        break;
    }

    Print("ERROR" << ErrorString.c_str());
    Print("Possible Fix:" << PossibleSolution.c_str());

}
 





//	glGenTextures(1, &TextureID);
//	glBindTexture(_attachments.first.Target, TextureID);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//  glTexImage2D(_attachments.first.Target, 0, GL_RGBA32F, Width, Height, 0, GL_RGBA, GL_FLOAT, 0);
//  glTexParameteri(_attachments.first.Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//  glTexParameteri(_attachments.first.Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//  glTexParameteri(_attachments.first.Target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//  glTexParameteri(_attachments.first.Target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureID, 0);

//  glGenTextures(1, &DepthTexture);
//  glBindTexture(_attachments.first.Target, DepthTexture);
//  //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//  glTexImage2D(_attachments.first.Target, 0, _attachments.first.Type, Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
//  glTexParameteri(_attachments.first.Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//  glTexParameteri(_attachments.first.Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//  glTexParameteri(_attachments.first.Target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//  glTexParameteri(_attachments.first.Target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

