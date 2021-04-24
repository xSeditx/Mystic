#pragma once
#include<vector>


#include"OpenGL.h"


// My Includes
#include"Common.h"
#include"Shader.h"

#define BUFFER_OFFSET(i)   ((char *)NULL + (i))

#define MYSTIC_DEFAULT_BUFFER_ACCESS   GL_DYNAMIC_DRAW // For later implementation

 
enum BufferTypes
{
	VERTEX, COLOR, NORMAL, UVCOORD, TEXTURE, INDICE, TANGENT, BITANGENT, NONE
};


//#define BINDLESS_ATTRIBUTES

class MYSTIC Attribute
{
public:
    Attribute();
    Attribute(BufferTypes t);

    void Bind();
    void Unbind();

    void *Map(GLenum accessflags);
    void *MapRange(int offset, int count, GLenum accessflags);

    size_t MappedSize();
    bool isMapped();
    void Release();
    void Destroy();

	GLuint GL_Handle = 0;
	BufferTypes AttributeType;
	GLint Location = 0;
	GLuint ElementCount = 0;

	GLuint64EXT Address;
	GLuint      Size;
};

template<class T = Vec3>
class MYSTIC VertexBufferObject
    : 
    public Attribute
{ // OpenGL Buffer Object which holds raw Buffer data which can be given to the VAO as a specific Attribute type by assigning it that usecase.
public:

    NO_COPY_OR_ASSIGNMENT(VertexBufferObject);


	using value_type = T;
	using pointer_type = T*;

	VertexBufferObject() = default;
	VertexBufferObject(T *data, GLsizei count)
	{
		ElementCount = count;
		Data.resize(count);
		Data.insert(Data.begin(), count, *data);

		Stride = sizeof(T);

		glGenBuffers(1, &GL_Handle);
		glBindBuffer(GL_ARRAY_BUFFER, GL_Handle);
		glBufferData(GL_ARRAY_BUFFER, (ElementCount) * Stride, data, MYSTIC_DEFAULT_BUFFER_ACCESS);
		
			
		/// ------------------------Bindless Address stuff---------------------------------
		glGetBufferParameterui64vNV(GL_ARRAY_BUFFER, GL_BUFFER_GPU_ADDRESS_NV,	&Address);
		glMakeBufferResidentNV(GL_ARRAY_BUFFER, GL_READ_ONLY);
		Size = ElementCount * Stride;
		///--------------------------------------------------------------------------------
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	VertexBufferObject(GLenum access, T *data, GLsizei count)
		: /// Specify default access
		ElementCount(count),
		GL_Handle(0)
	{
		Data.resize(count);
		Data.insert(Data.begin(), count, *data);
		Stride = sizeof(T);
		glGenBuffers(1, &GL_Handle);
		glBindBuffer(GL_ARRAY_BUFFER, GL_Handle);
		glBufferData(GL_ARRAY_BUFFER, ElementCount * sizeof(T), data, access);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	VertexBufferObject(std::vector<T> _vec)
	{// Create from a Vector
		*this = &VertexBufferObject<T>(& _vec[0], _vec.size());
	}
	void Update(std::vector<T> *_data)
	{
		Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, _data.size() * sizeof(T), _data);
	}
	void Update(void* _data, size_t _sz)
	{
		Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, _sz * sizeof(T), _data);
	}

	GLuint GetBufferSize() 
	{
		GLint results;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &results);
		return results;
	}
	size_t size() const { return Data.size(); }


	VertexBufferObject operator = (std::vector<T> data)
	{// Map the whole buffer, resize if needed and make the data of the buffer equal to that of the Rvalue
		Update(data);
	}
	VertexBufferObject operator = (VertexBufferObject &other)
	{// Same but perform a shallow copy of the buffer
		return *other;
	}
	VertexBufferObject operator += (VertexBufferObject &other)
	{ // Map the buffer and add to the end of it, updating the data, and size while retaining access type and GL_Handle
		Bind();
		glBufferData(GL_ARRAY_BUFFER, 
			(size() * sizeof(value_type)) + (other.size() * sizeof(other.value_type)), 
			0,
			MYSTIC_DEFAULT_BUFFER_ACCESS);

		std::move(other.Data.begin(), other.Data.end(), std::back_inserter(Data));
		glBufferSubData(GL_ARRAY_BUFFER, 0, size() * sizeof(value_type), Data);
	}

protected:
	GLuint Stride;
	std::vector<T> Data;
};


class MYSTIC ElementArrayBuffer
{
public:
    NO_COPY_OR_ASSIGNMENT(ElementArrayBuffer);

    ElementArrayBuffer() = default;
    ElementArrayBuffer(GLuint *_data, size_t _count);
    ElementArrayBuffer(GLenum _access, GLuint *_data, size_t _count);

    void Bind();
    void Unbind();

	GLuint *Data;// Later make private and Accessors
	GLuint GL_Handle;
	size_t ElementCount;
};


class MYSTIC VertexArrayObject
{ // OpenGL VAO container which holds the state information for the Object contained on the GPU

public:

    NO_COPY_OR_ASSIGNMENT(VertexArrayObject);

    VertexArrayObject();
    VertexArrayObject(VertexBufferObject<Vec3> &vbo);

	GLuint GL_Handle;
	int ElementCount;

	std::vector<Attribute*> Buffers;

    // Simple move assignment operator
 //VertexArrayObject& operator=(VertexArrayObject&& other)
 //{
 //   // *this = 
 //    return *std::move(other);//
 //}
#ifdef BINDLESS_ATTRIBUTES
	void Bind() {}
	void Unbind() {}

	template<typename T>
	void Attach(BufferTypes bufferT, VertexBufferObject<T> *buffer)
	{
		if (GL_Handle == NULL)
		{
			glGenVertexArrays(1, &GL_Handle);
		}
		Bind();
		GLint Amount = sizeof(T) / sizeof(float);
		Buffers.push_back(buffer);

		switch (bufferT)
		{
		case INDICE:
			buffer->AttributeType = INDICE;
			return;
			break;
		case VERTEX:
			buffer->AttributeType = VERTEX;
			break;
		case COLOR:
			buffer->AttributeType = COLOR;
			break;
		case NORMAL:
			buffer->AttributeType = NORMAL;
			break;
		case UVCOORD:
			buffer->AttributeType = UVCOORD;
			break;
		case TANGENT:
			buffer->AttributeType = TANGENT;
			break;
		}
	} 
#else

    void Bind();
    void Unbind();

    template<typename T>
    void Attach(BufferTypes bufferT, VertexBufferObject<T> *buffer)
    {
        /// I HAD THIS BECAUSE COLOR WAS CAUSING ISSUES LIKELY BECAUSE ITS A VEC 3 and Not a Vec4		if (bufferT == COLOR) return;
        if (GL_Handle == NULL)
        {
            _GL(glGenVertexArrays(1, &GL_Handle));
        }

        Bind(); // First Bind the buffer to make it Current in the OpenGL Context

        GLint Amount = sizeof(T) / sizeof(float);

        Buffers.push_back(buffer);

        switch (bufferT)
        {/// Case statement of Buffer types. Indices pushback then exit the function so that EnableVertexAttribArray and glVertexAttribPointer are never called.
        case BufferTypes::INDICE:
            ElementCount = buffer->ElementCount;
            buffer->AttributeType = INDICE;
            _GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->GL_Handle));
            return; //  If VBO is Indices then bind it to the VAO and return before attribute is set.
            break;


        case BufferTypes::VERTEX:
		{
			buffer->Location = Shader::GetActiveShader()->AttributeLocation("VertexPosition");
			if (buffer->Location == -1) return;
			buffer->AttributeType = VERTEX;
			break;
		}
        case BufferTypes::COLOR:
		{
			buffer->Location = Shader::GetActiveShader()->AttributeLocation("VertexColor");
			if (buffer->Location == -1) return;
			buffer->AttributeType = COLOR;
			break;
		}
        case BufferTypes::NORMAL:
		{
			buffer->Location = Shader::GetActiveShader()->AttributeLocation("VertexNormal");
			if (buffer->Location == -1) return;
			buffer->AttributeType = NORMAL;
			break;
		}
        case BufferTypes::UVCOORD:
		{
			buffer->Location = Shader::GetActiveShader()->AttributeLocation("TextureCoord");
			if (buffer->Location == -1) return;
			buffer->AttributeType = UVCOORD;
			break;
		}
		case BufferTypes::TANGENT:
		{
			buffer->Location = Shader::GetActiveShader()->AttributeLocation("VertexTangent");
			if (buffer->Location == -1) return;
			buffer->AttributeType = TANGENT;
			break;
		}
		case BufferTypes::BITANGENT:
		{
			buffer->Location = Shader::GetActiveShader()->AttributeLocation("VertexBitangent");
			if (buffer->Location == -1) return;
			buffer->AttributeType = BITANGENT;
			break;
		}
        }
        _GL(glBindBuffer(GL_ARRAY_BUFFER, buffer->GL_Handle));
        _GL(glEnableVertexAttribArray(buffer->Location));
        _GL(glVertexAttribPointer(buffer->Location, Amount, GL_FLOAT, GL_FALSE, 0, (char *)NULL));
    }
#endif

    void Render();
};

enum AttachmentPoint_t
{
	COLOR_ATTACHMENT_0 = GL_COLOR_ATTACHMENT0, // These are an implementation - dependent number of attachment points.You can query GL_MAX_COLOR_ATTACHMENTS to determine the number of color attachments that an implementation will allow.The minimum value for this is 8, so you are guaranteed to be able to have at least color attachments 0 - 7. These attachment points can only have Images bound to them with color - renderable formats.All compressed Image formats are not color - renderable, and thus cannot be attached to an FBO.
	COLOR_ATTACHMENT_1 = GL_COLOR_ATTACHMENT1,
	COLOR_ATTACHMENT_2 = GL_COLOR_ATTACHMENT2,
	COLOR_ATTACHMENT_3 = GL_COLOR_ATTACHMENT3,
	COLOR_ATTACHMENT_4 = GL_COLOR_ATTACHMENT4,
	COLOR_ATTACHMENT_5 = GL_COLOR_ATTACHMENT5,
	COLOR_ATTACHMENT_6 = GL_COLOR_ATTACHMENT6,
	COLOR_ATTACHMENT_7 = GL_COLOR_ATTACHMENT7,
	COLOR_ATTACHMENT_10 = GL_COLOR_ATTACHMENT10,
	COLOR_ATTACHMENT_11 = GL_COLOR_ATTACHMENT11,
	COLOR_ATTACHMENT_12 = GL_COLOR_ATTACHMENT12,
	COLOR_ATTACHMENT_13 = GL_COLOR_ATTACHMENT13,
	COLOR_ATTACHMENT_14 = GL_COLOR_ATTACHMENT14,
	COLOR_ATTACHMENT_15 = GL_COLOR_ATTACHMENT15,
	COLOR_ATTACHMENT_16 = GL_COLOR_ATTACHMENT16,
	COLOR_ATTACHMENT_17 = GL_COLOR_ATTACHMENT17,
	COLOR_ATTACHMENT_18 = GL_COLOR_ATTACHMENT18,
	COLOR_ATTACHMENT_19 = GL_COLOR_ATTACHMENT19,
	COLOR_ATTACHMENT_20 = GL_COLOR_ATTACHMENT20,
	COLOR_ATTACHMENT_21 = GL_COLOR_ATTACHMENT21,
	COLOR_ATTACHMENT_22 = GL_COLOR_ATTACHMENT22,
	COLOR_ATTACHMENT_23 = GL_COLOR_ATTACHMENT23,
	COLOR_ATTACHMENT_24 = GL_COLOR_ATTACHMENT24,
	COLOR_ATTACHMENT_25 = GL_COLOR_ATTACHMENT25,
	COLOR_ATTACHMENT_26 = GL_COLOR_ATTACHMENT26,
	COLOR_ATTACHMENT_27 = GL_COLOR_ATTACHMENT27,
	COLOR_ATTACHMENT_28 = GL_COLOR_ATTACHMENT28,
	COLOR_ATTACHMENT_29 = GL_COLOR_ATTACHMENT29,
	COLOR_ATTACHMENT_30 = GL_COLOR_ATTACHMENT30,
	COLOR_ATTACHMENT_31 = GL_COLOR_ATTACHMENT31,
	DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT,//GL_DEPTH_ATTACHMENT : This attachment point can only have Images with depth formats bound to it.The Image attached becomes the Depth Buffer for the FBO. **NOTE** Even if you don't plan on reading from this depth_attachment, an off screen buffer that will be rendered to should have a depth attachment.
	STENCIL_ATTACHMENT = GL_STENCIL_ATTACHMENT,// This attachment point can only have Images with stencil formats bound to it.The Image attached becomes the stencil buffer for the FBO.
	DEPTH_STENCIL_ATTACHMENT = GL_DEPTH_STENCIL_ATTACHMENT// This is shorthand for "both depth and stencil".The Image attached becomes both the depth and stencil buffers. Note : If you use GL_DEPTH_STENCIL_ATTACHMENT, you should use a packed depth - stencil internal format for the texture or renderbuffer you are attaching.
};

class MYSTIC FrameBufferObject
{
public:


 	FrameBufferObject() = default;
	FrameBufferObject(std::pair<Texture&, AttachmentPoint_t> _attachments);
    FrameBufferObject(int _width, int _height, GLenum _datatype = GL_FLOAT, GLenum _internal = GL_RGBA32F, GLenum _format = GL_RGBA);

	Vec2 Size;
	int Width;
	int Height;
	GLuint FrameBufferID;
	GLuint TextureID;
	GLuint DepthTexture;
	GLenum InternalFormat;
	GLenum PixelFormat;
	GLenum DataType;

    void Bind();
    void Unbind();
	void ValidateFrameBuffer();

	// GL_NONE,GL_FRONT_LEFT,GL_FRONT_RIGHT,GL_BACK_LEFT,GL_BACK_RIGHT,GL_FRONT,GL_BACK,GL_LEFT,GL_RIGHT,GL_FRONT_AND_BACK
	GLenum DrawBuffer = GL_NONE;
	GLenum ReadBuffer = GL_NONE;

	AttachmentPoint_t Attachments[10];
};

template<typename T>
struct MYSTIC UniformBufferObject
{
	UniformBufferObject() = default;
	UniformBufferObject(Shader *_program, const char *_name, unsigned int _slot)
		:
		BlockSize(sizeof(T)),
		Name(_name),
        Slot(_slot),
		Program(_program->GL_Handle)
	{
		Index = fetchIndex(Name);
		SetBinding(Slot);

		Create();
		Bind();
		SetData(NULL);

		BindBase(_slot);
		//* Same as above glBindBufferRange(GL_UNIFORM_BUFFER, 0, GL_Handle, 0, BlockSize);
		SetSubData(0, BlockSize, NULL);
		Unbind();
	}

	void Create()
	{
		glCreateBuffers(1, &GL_Handle);
	}
	void BindBase(GLuint _slot)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, _slot, GL_Handle);
	}
	void SetBinding(GLuint _slot)
	{
		glUniformBlockBinding(Program, Index, _slot);
	}
	void Update()
	{
		Bind();
		SetSubData(0, BlockSize, &Data);
		Unbind();
	}

	void SetData(T* _data)
	{
		glBufferData(GL_UNIFORM_BUFFER, BlockSize, _data, GL_DYNAMIC_DRAW);
	}
	void SetSubData(size_t _offset, size_t _range, void* _data)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 0, _range, _data);
	}

	void Bind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, GL_Handle);
	}
	void Unbind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void LockBuffer()
	{
		if (gSync)
		{
			glDeleteSync(gSync);
		}
		gSync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	}
	void WaitBuffer()
	{
		if (gSync)
		{
			while (true)
			{
				GLenum waitReturn = glClientWaitSync(gSync, GL_SYNC_FLUSH_COMMANDS_BIT, 1);
				if (waitReturn == GL_ALREADY_SIGNALED || waitReturn == GL_CONDITION_SATISFIED)
				{
					return;
				}
			}
		}
	}

	int fetchIndex(const char *_name)
	{
		return glGetUniformBlockIndex(Program, _name);
	}
	inline GLuint g_Index()
	{
		return Index;
	}

	T Data;
private:
	// http://docs.gl/gl4/glBufferSubData
	unsigned int BlockSize;

	const char *Name;

	GLsync gSync;
    GLuint Slot;
	GLuint Index;
	GLuint GL_Handle;
	GLuint Program;
};


enum MYSTIC ShaderType
{/// COMPLETELY TEMPORARY
    TESSLATION_CONTROL_SHADER = GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER,
    TESSLATION_EVALUATION_SHADER = GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER,
    GEOMETRY_SHADER = GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER,
    FRAGMENT_SHADER = GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER,
    COMPUTE_SHADER = GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER,
    VERTEX_SHADER = GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER
};

template<typename T>
struct MYSTIC PersistantUniformBufferObject
{
    PersistantUniformBufferObject() = default;
    PersistantUniformBufferObject(Shader *_program, const char *_name, unsigned int _bindLOC)
        :
        BlockSize(sizeof(T)),
        Name(_name),
        SyncObject(glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0)),
        Slot(_bindLOC),
        Index(NULL),
        GL_Handle(NULL),
        Pointer(NULL),
        Program(_program->g_ID())
    {
        Index = fetchIndex(Name);
        SetBinding(Slot);

        Create();
        Bind();
        BindBase(Slot);

        unsigned int flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glBufferStorage(GL_UNIFORM_BUFFER, BlockSize, 0, flags);
        Pointer = Map(flags);
    }
    void Update()
    {// Ensures previous write has completed and copies the contents of Data over to Pointer
        WaitBuffer();
        SetData(&Data);
        LockBuffer();
    }

    T* Map(GLuint _flags)
    {// Since it is persistant this only needs to be called once and this abstraction is likely uneeded later on.
        return (T*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, BlockSize, _flags);
    }
    void Unmap()
    {
        glUnmapBuffer(GL_UNIFORM_BUFFER);
    }
    void Create()
    {
        glCreateBuffers(1, &GL_Handle);
    }
    void BindBase(GLuint _slot)
    {

        glBindBufferBase(GL_UNIFORM_BUFFER, _slot, GL_Handle);
        //	glBindBufferRange(GL_UNIFORM_BUFFER, 0, GL_Handle, 0, BlockSize);
    }
    void SetBinding(GLuint _slot)
    {// Binds the Buffer to a Specified Uniform slot on the GPU.
        glUniformBlockBinding(Program, Index, _slot);

    }

    void SetData(T* _data)
    {// When class is finished likely this will just be replaced by proper syncing and direct access to memory block elements via T
        memcpy(Pointer, _data, BlockSize);
    }
    void SetSubData(size_t _offset, size_t _range, void* _data)
    {// Fills in specified range of data. ( likely not needed in future use as Pointer and Data may become a single entity and Access will be performed directly on respective elements)
        memcpy(Pointer + _offset, _data, _range);
    }

    void Bind()
    {// Need it at Initialization and potentially if the buffer is ever needed for other use
        glBindBuffer(GL_UNIFORM_BUFFER, GL_Handle);
    }
    void Unbind()
    {// Both likely uneeded due to persistancy of buffer
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void LockBuffer()
    {// Creates a Fence object 
        if (SyncObject)
        {// Destroys Sync Objects if it already exist
            glDeleteSync(SyncObject);
        }
        SyncObject = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    }
    void WaitBuffer()
    {// Syncs operations to ensure Buffer write is finished before proceeding 
        if (SyncObject)
        {
            while (1)
            {
                GLenum waitReturn = glClientWaitSync(SyncObject, GL_SYNC_FLUSH_COMMANDS_BIT, 1);
                if (waitReturn == GL_ALREADY_SIGNALED || waitReturn == GL_CONDITION_SATISFIED)
                    return;
            }
        }
    }


    //TODO: Allow Direct Acces to pointer when working correctly
    T Data;
    T *Pointer;

    // OpenGL API for Uniform Buffer information

    GLint isReferenced(ShaderType _shadertype)
    {
        GLint results = 0;
        glGetActiveUniformBlockiv(Program, Index, _shadertype, &results);
        return results;
    }
    int fetchIndex(const char *_name)
    {// Return Index of the uniform block with specific name
        return glGetUniformBlockIndex(Program, _name);
    }
    int fetchActiveBlockBinding() {
        int results = 0;
        glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_BINDING, &results);
        return results;

    }
    int fetchBlockSize()
    {
        int results = 0;
        glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_DATA_SIZE, &results);
        return results;
    }
    int fetchNameLength()
    {
        int results = 0;
        glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_NAME_LENGTH, &results);
        return results;
    }
    int fetchActiveUniforms()
    {
        int results = 0;
        glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &results);
        return results;
    }
    int fetchActiveUniformIndices()
    {
        int results = 0;
        glGetActiveUniformBlockiv(Program, Index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &results);
        return results;
    }

private:

    unsigned int BlockSize;

    const char *Name;

    GLsync SyncObject;
    GLuint Slot;
    GLuint Index;
    GLuint GL_Handle;
    GLuint Program;
};














#define SetMember(name) offsetof(T, name)




/// Not currently in use yet however Intentions are to Stack the entire State of OpenGL as much as possible for Efficient Scene Transversal
struct OpenGLState
{
	OpenGLState()
		:
		BlendingEnabled(true),
		CullFaceEnabled(true)
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);

		BlendingOperation.push({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
		CullFace.push(GL_BACK);
	}

	//______________ BLENDING ________________________
	bool BlendingEnabled;
	struct blending
	{
		GLenum 
			Sfactor = GL_SRC_ALPHA,
			Dfactor = GL_ONE_MINUS_SRC_ALPHA;
	};

	
	std::stack<blending> BlendingOperation;
	void SetBlendingState(GLenum _sfactor, GLenum _dfactor)
	{//void glBlendFunc(GLenum sfactor, GLenum dfactor);
		BlendingOperation.push({ _sfactor ,_dfactor }); 
		glBlendFunc(_sfactor, _dfactor);
	}
	//_________________________________________________

	//void glCullFace(GLenum mode);
	//_____________ CULL FACES ________________________

	bool CullFaceEnabled = true;
	std::stack<GLenum> CullFace;

	void SetCullFaceMode(GLenum _mode)
	{
		glCullFace(_mode);
	}
	//_________________________________________________


};




/*
Striped down version https://pastebin.com/Kr8U7mGu

Persistantly Mapped Uniform Buffer with Coherent memory access making writes immediately availible to the GPU
Usage:
PersistantUniformBufferObject<STRUCT> UBOobject(Pointer to Shader, Uniforms Block Name, Uniform Binding Location);

 Members of Data can be access and Updated at will however currently visibility will not take place until void Update() is called

 NOTE: 7/5/19
 Current Implementation is incorrect as the second Buffer I create comes through Corrupted. The first I pass a Matrices Struct containing Projection and View Matrices,
 the Second I pass lighting information such as Position and Color. All Images on Screen turn bright green as the Light information is corrupted completely and has
 Random values.
*/


//glBufferStorage(GL_UNIFORM_BUFFER, BlockSize, 0, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
//BufferHandle = (GPUptr*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, BlockSize, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
//GLenum glCheckError_(const char *file, int line)
//{
//	GLenum errorCode;
//	while ((errorCode = glGetError()) != GL_NO_ERROR)
//	{
//		std::string error;
//		switch (errorCode)
//		{
//		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
//		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
//		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
//		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
//		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
//		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
//		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
//		}
//		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
//	}
//	return errorCode;
//}
//  1280 GL_INVALID_ENUM
//  1281 GL_INVALID_VALUE
//  1282 GL_INVALID_OPERATION
//  1283 GL_STACK_OVERFLOW
//  1284 GL_STACK_UNDERFLOW
//  1285 GL_OUT_OF_MEMORY
//  
//  //  switch (bufferT)
//  {/// Case statement of Buffer types. Indices pushback then exit the function so that EnableVertexAttribArray and glVertexAttribPointer are never called.
//  
//  case BufferTypes::VERTEX:
//  	Amount = sizeof(T) / sizeof(float);
//  	buffer->AttributeType = VERTEX;
//  	glBindBuffer(GL_ARRAY_BUFFER, buffer->GL_Handle);
//  	Buffers.push_back(*buffer);
//  	break;
//  
//  case BufferTypes::COLOR:
//  	Amount = sizeof(T) / sizeof(float);
//  	buffer->AttributeType = COLOR;
//  	glBindBuffer(GL_ARRAY_BUFFER, buffer->GL_Handle);
//  	Buffers.push_back(*buffer);
//  	break;
//  
//  case BufferTypes::NORMAL:
//  	Amount = sizeof(T) / sizeof(float);
//  	buffer->AttributeType = NORMAL;
//  	glBindBuffer(GL_ARRAY_BUFFER, buffer->GL_Handle);
//  	Buffers.push_back(*buffer);
//  	break;
//  
//  case BufferTypes::UVCOORD:
//  	Amount = sizeof(T) / sizeof(float);
//  	buffer->AttributeType = UVCOORD;
//  	glBindBuffer(GL_ARRAY_BUFFER, buffer->GL_Handle);
//  	Buffers.push_back(*buffer);
//  	break;
//  
//  case BufferTypes::INDICE:
//  	ElementCount = buffer->ElementCount;
//  	buffer->AttributeType = INDICE;
//  	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->GL_Handle);
//  	Buffers.push_back(*buffer);
//  	return; //  Exit since the Element Buffer is not an Attribute and continuing through in the function would result in attempting to bind the element buffer to an attribute slot
//  }



/// RANDOM CODE FROM OLDER PROJECT THAT I MIGHT USE OR AT LEAST WANT TO KEEP IN ORDER TO HAVE IDEAS
//  GameObject::GameObject()
//  {
//  	Set_Position(Vec3(0.0f));
//  	Set_Rotation(Vec3(0.0f));
//  	Set_Scale(Vec3(1.0f));
//  }
//  
//  
//  void GameObject::Set_Transform(Vec3 pos, Vec3 rot, Vec3 scale)
//  {
//  	Transform = glm::mat4(1.0f); // Set Identity and Rotate all axis followed with the Translation.
//  	Transform = glm::translate(Transform, pos);
//  	Transform = glm::rotate(Transform, glm::radians(rot.x), Vec3(1.0f, 0.0f, 0.0f));
//  	Transform = glm::rotate(Transform, glm::radians(rot.y), Vec3(0.0f, 1.0f, 0.0f));
//  	Transform = glm::rotate(Transform, glm::radians(rot.z), Vec3(0.0f, 0.0f, 1.0f));
//  	Transform = glm::scale(Transform, scale);
//  	// WARNING: NOT SURE SCALING IN THE PROPER ORDER
//  	Print("GameObject::Set_Transform() -  Unsure if Scaling is being applied properly. Double check.");
//  }

