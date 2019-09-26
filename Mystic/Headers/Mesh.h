#pragma once
/*========================================================================================================================================================================================
WE IMPLIMENT THE API AS SUCH.
Make your Object via the appropriate object type,
Sprite generally for HUD or Sprites since it produces a renderable Quad containing a Texture Material whos UV coords can be adjusted for various animation effects.
Mesh is generally for 3D objects, Primatives are avalible for creation and are derived from the Mesh class as well as an OBJ loader( currently not working ).
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
   USE:
_________
Create your object:
___________________
____| Create Primative
____| Load Mesh OBJ file
Set its Position:
________________
____| void Set_Position(Vec3 pos)
Set its Rotation:  ( generally in part the job of the Ctor of the Object type if its a Primative Object )
_________________________________
____| void Set_Rotation(Vec3 rot)
Set its Scale: (normally just 1.0f but other values are acceptable yet may lead to problems and one should be cautious )
_____________
____| void Set_Scale(Vec3 scale)
Or.....
Set full Transform if Desired:
_____________________________
____| void Set_Transform(Matrix mat)
Add a Material to the object using...
_____________________________________
____| Add(Material *) method;
Add a Vertices and Attributes:
_____________________________
____| Add(VAOBuffer *vao)
if needed For Added Flexibility in future use.
Allows for the creation of a blank object which can be built later.
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Set_VertexCount(GLuint Vcount)
The Following Methods of the Object class are pure Virtual and are implemented in the Appropriate Derived class type
void Bind() override;
---- Binds all your Buffers Via a VAOBuffer Bind which handles all the binding of any Buffers and VAOs it has.
---- It also binds any Materials the Object contains and sets up and Uniforms and Texture Samplers the Shaders will need
void Unbind() override;
---- Reverses the process of the Above clearing OpenGLs state for the various stuff
void Render() override;
---- Handles the Bind() function described above...
---- Performs the Drawcall for the Object rendering it to the screen or currently active Framebuffer
void Update() override;
---- Updates any Transforms or Uniforms that will need to be bound using bind. This allows for seperation of the Rendering from the Physics if Implemented properly.
========================================================================================================================================================================================*/

#include"Window.h"

//#include"Uniform.h"
#include"Assimp.h"

#include"GLBuffers.h"
#include"Common.h"
#include"Material.h"
#include"Asset.h"

#include<GameObject.h> /// Possibly changed later but good enough for now


class Shader;
struct MatProps
{
	unsigned int Semantic;
	unsigned int Index;
	unsigned int DataLength;
	aiPropertyTypeInfo Type;
	char *Data;
};
 


///struct Material : public GameObject
///{
///	Material() { TODO("Implement a Material class, you called its Constructor"); }
///};

class MYSTIC Mesh
	: public GameObject
{
public:
	struct indirect
	{
		uint32_t  count;
		uint32_t  instanceCount;
		uint32_t  firstIndex;
		uint32_t  baseVertex;
		uint32_t  baseInstance;
	}IndirectDraw;

	unsigned int MaterialID;
	unsigned int PrimativeType = GL_TRIANGLES;

	VertexArrayObject *VAO; /// Figure something out here

	std::vector<Mesh> Children;
	std::vector<Material> Surfaces;

	Mesh();
	Mesh(std::string file);

 	void Bind(Shader &_shader);
	void Render(Shader &_shader) override;

	void Unbind() override;
 	void Update() override;

	void GetPrimativeType(unsigned int input)
	{
		switch (input)
		{
		case aiPrimitiveType_LINE:
			PrimativeType = GL_LINES;
				break;
		case aiPrimitiveType_POINT: 
			PrimativeType = GL_POINTS;
			break;
		case aiPrimitiveType_TRIANGLE:	
			PrimativeType = GL_TRIANGLES;
			break;
		case aiPrimitiveType_POLYGON:
			PrimativeType = GL_TRIANGLE_STRIP;
			/// "I think this is wrong, there appears to be no GL_POLYGONS so Triangle strip seems the closest thing I can think of");
            __debugbreak();
			break;
		}
	}
//inline void AddVertexArray( VertexArrayObject &&_vao)
//{
//	//*VAO = &_vao;
//}

protected:

	std::string Filepath;
	bool needsUpdated = false; // #ATOMIC NEEDED
	bool Static = true;
};



// GL_LINE_STRIP,
// GL_LINE_LOOP,
// GL_LINE_STRIP_ADJACENCY,
// GL_LINES_ADJACENCY,
// GL_TRIANGLE_STRIP,
// GL_TRIANGLE_FAN, 
// GL_TRIANGLE_STRIP_ADJACENCY,
// GL_TRIANGLES_ADJACENCY 
// GL_PATCHES
// 





/*
GameObject
---- Empty
---- 2D
---- 3D
---- Sprite
---- Effect
---- Light
---- Audio
---- Video
---- Camera
*/