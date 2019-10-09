#pragma once
#include"Common.h"
#include"GLBuffers.h"
#include"Camera.h"

#include"Material.h"
#include"Mesh.h"

#include"Assimp.h"
//
//#include"ECSsystem.h"
//#include"ECScomponent.h"
//#include"ECS.h"
//
///Layered rendering
/*
Also known as bucketized rendering, the idea here is to assign a key to a draw call which is then used for sorting.
Typically, a key is just a single 32 - bit or 64 - bit integer, nothing more.Usually, 
a key encodes certain data like distance, material, shader, etc.of a draw call in individual bits.
Depending on where those bits are stored in the integer, you can apply different sorting criteria for the same array of draw calls,
as long as you know how the keys were built. */



// Will create a Rendering Context for our Application which will manage OpenGL(or DirectX) Rendering Context, 
// the Scene, and all primatives or Renderable items avalible

class Shader;

// Usage: 
// glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, &command); // Command is per-object parameters now sourced from Memory
// glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, commands, commandCount, 0);

// glBindBuffeR(GL_DRAW_INDIRECT_BUFFER, commandBuffer);
// glBindBuffer( GL_PARAMETER_BUFFER, countBuffer);
// glMultiDrawElementsIndirectCount

//const DrawArraysIndirectCommand  *cmd = (const DrawArraysIndirectCommand  *)indirect;
//glDrawArraysInstancedBaseInstance(mode, cmd->first, cmd->count, cmd->instanceCount, cmd->baseInstance);

//Texture Arrays are 5x performance;
// indirect​ is interpreted as an offset, in basic machine units, into the buffer bound to GL_DRAW_INDIRECT_BUFFER at the time of the call
// const IndirectArrayBuffer *cmd = (const IndirectArrayBuffer *) Indirect;
// glDrawArraysInstancedBaseInstance(mode, cmd->first,cmd->count, cmd->instanceCount, cmd->baseInstance);
class Renderer;
//   
//   COMPONENT(TransformComponent)
//   {
//     Vec3 
//         Position,
//         Rotation,
//         Scale;
//     Mat4 Transform;
//   };
//   COMPONENT(RenderableComponent)
//   {
//     VertexArrayObject *VAO;
//     Material *MeshSurface;
//   };
//   
//   struct RenderSystem : public BaseSystem
//   {
//     RenderSystem()
//         :
//         BaseSystem()
//     {
//         AddComponentType(TransformComponent::ID);
//         AddComponentType(RenderableComponent::ID);
//         #ifdef _DEBUG
//                 SystemName = (typeid(this).name());
//         #endif
//     }
//     virtual void UpdateComponents(float delta, BaseComponent** components)
//     {
//         TransformComponent* transform = (TransformComponent*)components[0];
//         RenderableComponent* mesh = (RenderableComponent*)components[1];
//       //  Context->Submit(mesh->VAO, mesh->MeshSurface);
//     }
//   
//   private:
//     Renderer *Context;
//   };
//   
//
//

//


struct IndirectArrayBuffer
{
    IndirectArrayBuffer
    (
        uint32_t  _count,
        uint32_t  _instanceCount,
        uint32_t  _firstIndex,
        uint32_t  _baseInstance
    )
        :
        count(_count),
        instanceCount(_instanceCount),
        firstIndex(_firstIndex),
        baseInstance(_baseInstance)
    {}

    uint32_t  count;
    uint32_t  instanceCount;
    uint32_t  firstIndex;
    uint32_t  baseInstance;
};
struct Indirect
{
    Indirect
    (
        uint32_t  _count,
        uint32_t  _instanceCount,
        uint32_t  _firstIndex,
        uint32_t  _baseVertex,
        uint32_t  _baseInstance
    )
        :
        count(_count),
        instanceCount(_instanceCount),
        firstIndex(_firstIndex),
        baseVertex(_baseVertex),
        baseInstance(_baseInstance)
    {}


    uint32_t  count;
    uint32_t  instanceCount;
    uint32_t  firstIndex;
    uint32_t  baseVertex;
    uint32_t  baseInstance;
};
struct Renderable
{// Baseclass for any renderable object inside of this engine
    Renderable
    (
        Indirect _drawCallIndirect,
        uint32_t _baseMaterial,
        uint32_t _materialID
    )
        :
        DrawCallIndirect(_drawCallIndirect),
        baseMaterial(_baseMaterial),
        MaterialID(_materialID)
    {}

    Indirect DrawCallIndirect;
    uint32_t baseMaterial;
    uint32_t MaterialID;
};



class Group 
{
public:
	void Bind()
	{

	}
	void Render()
	{}
	void Update(){}
};



//class Renderer
//{
//public:
//	struct Pass
//	{
//		void Add(Mesh *_mesh)
//		{
//			Meshes.push_back(_mesh);
//		}
//		void Add(Material *_material)
//		{ 
//			Materials.push_back(_material);
//		}
//		std::vector<Material*> Materials; //Shader TexturesBound here
//		std::vector<Mesh*> Meshes;
//	};
//	
//
////foreach(render target) // Framebuffer
////foreach(render pass) // Depth blending etc states
////foreach(material)   // Shaders
////foreach(material instance) // Textures
////foreach(vertex format) // VertexBuffer < - Pack multiple buffers into single VBO
////foreach(object)
//	std::vector<FrameBufferObject> RenderTargets;
//	std::vector<Pass> RenderPass;
//};
//
#include<map>

class Camera3D;


class Scene
{
public:
	Scene() = default;

	Scene(std::string file);
	std::vector<Group> Groups;

	void Render(Shader &_shader);
	void Update();

	void Bind(Shader &_shader);
	void AddGroup(const Group &_group)
	{
	}

	Camera3D *Camera = nullptr;


	void Attach(Camera3D *_cam) { Camera = _cam; }
	void Attach(Mesh _mesh) { Meshes.push_back(_mesh); }
	void Attach(Material _mat) { Materials.push_back(_mat); }

	Mat4 Transform;
	/// Design this to perform Async updates on all mesh objects prior to rendering. 
	/// NVidia cards can also do async drawcalls
	std::vector<Mesh> Meshes;
	///=============================================================================================================================================
	/// Possibly have one of these go, the Texture should be in the Material
	///=============================================================================================================================================
	/// PROS: More information
	/// CONS: Slower
	std::vector<Material> Materials;

	///=============================================================================================================================================

	std::string Filepath;

	void LoadMaterials(const aiScene *_rawscene);
	void LoadMeshes(const aiScene *_scene);

	void Load();
};


struct RenderPass
{
	FrameBufferObject FBO;
	Shader *Program;
};
class MYSTIC Renderer
{
public: 
    Renderer() = default;
	static Renderer& Instance()
	{
		static Renderer instance;
		return instance;
	}
	void DrawIndices(std::size_t _start, std::size_t count);

	void Submit(Renderable &_object);

 
	void Draw();

	VertexArrayObject VAO;
	Scene World;
 	TagID ID;

	void Submit(Mesh &_mesh);
 

	std::vector<Material> Materials;

	std::vector<VertexArrayObject> VAOs;
    std::unordered_map<uint32_t, uint32_t> MaterialMeshMap;

	///Viewport Camera;
	// Unit Test
	void TestRenderer();
};





