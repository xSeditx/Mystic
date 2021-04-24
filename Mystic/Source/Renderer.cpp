#include"Renderer.h"
#include"FileIO.h"
#include"Camera.h"

 /*
 This entire Module sucks and needs to go.
 Just burn it down and start anew as it should have never existed in the first place and is just the result of boredom
 */
//http://duriansoftware.com/joe/An-intro-to-modern-OpenGL.-Chapter-2.2:-Shaders.html
/// Nimmy Engine
//https://github.com/JulianThijssen/Flux/blob/master/Engine/Shaders/Lighting.frag
/// ASSIMP MODEL LOADER
//https://www.ics.com/blog/qt-and-opengl-loading-3d-model-open-asset-import-library-assimp
//https://www.researchgate.net/publication/221895787_Shading_of_a_computer-generated_hologram_by_zone_plate_modulation
/*

Load Scene
- Load Meshes *
- Meshes have Material ID *
-  Materials Have Textures
-- Texture 0 is Diffuse

Before Rendering Scene

Render(Shader*)
 
-- Enable any FrameBuffer Passes
-- Enable Shader
-- Enable Material(uniforms & Textures)
--- Bind Geometry
--- SetUniforms for Camera
---- Setuniforms for lights
----- Vertices
DRAWCALL


Cycle over Meshes
 We bind the Diffuse Texture The Mesh Material ID says to
 We bind and Render the Mesh drawcall
 We go back and loop over all meshes.

  2:
*/

#include"GLBuffers.h"

/*

A Surface should Consist of Textures Describing its suface, Color, Metallic, Albedo, Normals etc....std::vector<Textures>/std::vector<TypeEnum> Types;
Material Should consist of a Shader and a Bunch of Surfaces std::vector<Surface> Surfaces;
Render Pass should consist of Geometry along with Materials; 	std::vector<std::vector<uint32_t>> MaterialGroups;

*/
 


RenderPass::RenderPass(DeferredRenderer* _render, FrameBufferObject& _fbo, Shader* _shader)
:
	ParentRenderer(_render),
	FrameBuffer(_fbo),
	DefaultShader(_shader)
{
	MaterialGroups.resize(_render->Materials.size());
}

void RenderPass::Submit(uint32_t _meshID, uint32_t _materialID)
{
	MYSTIC_ASSERT(_materialID < MaterialGroups.size(), "Attempting to Use a Material that does not exist: \n Material ID out of Range");
//if (MaterialGroups.size() < _materialID)
//{// If the Index we wish to insert this Mesh into does not exist yet, Create it. [#PERF: This is not Ideal].
//	MaterialGroups.resize(_materialID);
//}
	MaterialGroups[_materialID].push_back(_meshID);//. .resize(_render->Materials.size());
//uint32_t mID{ 0 };
//for (auto& M : ParentRenderer->Meshes)
//{
////	TODO("Ended right here... Trying to Abstract away the Render pass so that when I submit I Submit to a specific pass giving it my FrameBuffer and Default Shader information. This likely will chance so that my Render pass is nothing more than a FrameBuffer and Possibly Vector of Geometry,.")
//		//Submit(mID, M.MaterialID);
//	++mID;
//}


	MaterialGroups[_materialID].push_back(_meshID);
}


DeferredRenderer::DeferredRenderer(std::string _file)
	:
	Filepath(_file)
{
	Importer::ImportScene(_file, Meshes, Materials);
	Camera = new Camera3D({ 0,0,0 }, { 0,0,0 });
}
void DeferredRenderer::Render(Shader& _shader)// Must fix this as the shader should be determined by the Material and pass 
{
	uint32_t mID{ 0 };
	for (auto& P : Passes)
	{// Cycle over all Rendering Passes
		for_loop(index, P.MaterialGroups.size())
		{// Cycle over all Material Groups in the Pass[#NOTE: All set to single NULL group for Shadow and Geometry Rendering with no Material]
			Materials[index].Bind(&_shader);
			for (auto& M_index : P.MaterialGroups[index])
			{
				Meshes[M_index].Render(_shader);
			}
		}
	}
}
void DeferredRenderer::Update()
{
}
void DeferredRenderer::CreateRenderPass(FrameBufferObject& _fbo, Shader* _shader)
{
	Passes.emplace_back( this, _fbo, _shader );
}

// const Mesh& _mesh, Material& _material)

/*std::vector<std::vector<uint32_t>> MaterialGroups;*/


Scene::Scene(std::string _file)
	: 
	Filepath(_file),
	Camera(&Camera3D(Vec3(0), Vec3(0))),
    Transform(Mat4(1))
{
	Importer::ImportScene(_file, Meshes, Materials);
}



//====================================================================================================================================================================================
/// RENDERING STUFF. I INTEND TO BREAK THIS INTO MULTI-THREADED RENDERING AND PSEUDO MT RENDERING AND DISCARD THIS RENDERER IN THE FUTURE IN FAVOR OF TASK BASED RENDERING
//====================================================================================================================================================================================

void Scene::Update()
{/// We can do this in Parallel
   // for (auto &G : Groups)
   // {
   // 	G.Update();
   // }
	for (auto& M : Meshes)
	{///THIS WILL BE A PER MESH IDEA. MESH LINKS TO UNIFORM. WE BIND A UBO LIKE OBJECT THEN RENDER MESH. MESH IS NOT MATERIAL. MESH HAS MATERIAL ID
		M.Update();
	}
}



void Scene::Bind(Shader &_shader)
{
	_shader.SetUniform("ModelMatrix", Transform);
}
void Scene::Render(Shader &_shader)
{
	for (auto& M : Meshes)
	{///THIS WILL BE A PER MESH IDEA. MESH LINKS TO UNIFORM. WE BIND A UBO LIKE OBJECT THEN RENDER MESH. MESH IS NOT MATERIAL. MESH HAS MATERIAL ID
		Materials[M.MaterialID].Bind(&_shader);
		M.Render(_shader);
	}
}

//
//void Scene::Render()
//{
//	for (auto &G : Groups)
//	{
//		G.Render();
//	}
//}
//


void Renderer::Submit(Mesh &_mesh)
{
	/// Better to glBufferData() Allocate as much as needed. Add buffer one... add buffer two.... instead of this clusterfuck
	//TODO("This is Fucking God Awful, Fix the Attribute and VertexBufferObject classes to map properly")
	int VertexID = 0;
	int S = _mesh.VAO->Buffers.size();
	for_loop (I , S)
	{
		if (_mesh.VAO->Buffers[I]->AttributeType == VERTEX)
		{
			VertexID = I;
		}
	}
 
	if (!VAO.Buffers.empty())
	{
		int VertexID2 = 0;
		size_t sz = VAO.Buffers.size();
		for_loop(I2, sz)
		{
			if (VAO.Buffers[I2]->AttributeType == VERTEX)
			{
				VertexID2 = I2;
			}
		}

		//-------------------------------------------------------------
		std::vector<Vec3> V1;
		Vec3 *Verts = (Vec3*)_mesh.VAO->Buffers[VertexID]->Map(GL_READ_ONLY);
		int SZ = _mesh.VAO->Buffers[VertexID]->MappedSize();

		for_loop(I, SZ / sizeof(Vec3))
		{
			V1.push_back(Verts[I]);
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		//-------------------------------------------------------------
		std::vector<Vec3> V2;
		Vec3 *Verts2 = (Vec3*)VAO.Buffers[VertexID2]->Map(GL_READ_ONLY);
		size_t SZ2 = VAO.Buffers[VertexID2]->MappedSize();
		for_loop(I, SZ2 / sizeof(Vec3))
		{
			V2.push_back(Verts2[I]);
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		//-------------------------------------------------------------

		VAO.Buffers[VertexID2]->Bind();
		glBufferData(GL_ARRAY_BUFFER, SZ + SZ2, 0, MYSTIC_DEFAULT_BUFFER_ACCESS);
		glBufferSubData(GL_ARRAY_BUFFER, 0, SZ2, &V2[0]);
		glBufferSubData(GL_ARRAY_BUFFER, SZ2, SZ, &V1[0]);
	}
	else
	{
		std::vector<Vec3> V;
		Vec3 *Verts = (Vec3*)_mesh.VAO->Buffers[VertexID]->Map(GL_READ_ONLY);
		int SZ = _mesh.VAO->Buffers[VertexID]->MappedSize();
		glUnmapBuffer(GL_ARRAY_BUFFER);
		for_loop(I, SZ / sizeof(Vec3))
		{
			V.push_back(Verts[I]);
		}
		VAO.Attach(VERTEX, new VertexBufferObject<Vec3>(&V[0], V.size()));
	}
}




void Scene::Attach(Mesh& _mesh) { Meshes.push_back(_mesh); }
void Scene::Attach(Material& _mat) { Materials.push_back(_mat); }



//
//foreach(Object)
//{
//	WriteUniformData(object, &UniformData[i]);
//	WriteDrawCommand(object, &commands[i]);
//}
//glMultiDrawElementsIndirect
//(
//	GL_TRIANGLES,
//	GL_UNSIGNED_SHORT,
//	commands,
//	commandCount,
//	0);
// glDrawElements(M.PrimativeType, M.VAO.ElementCount, GL_UNSIGNED_INT, nullptr);
//Textures[M.MaterialID].Bind();
// Soon my precious.... Sooooonnnn!!!!!
//	Update();
//	Bind();// BIND SHOULD HANDLE CAMERA.... UM..... ummm..... That.... WTF else? Bind->Scene..... Bind the fucking scene... AH lights.... CAmera Action... Booom Bitches!
///	Bind Lights...
///	Bind Camera
// This will replace the main test soon
//	Shader::Shader::GetActiveShader()->ViewMatrix.Set(Camera->ViewMatrix);
///	Render GEO for Action...
///KILLL ITTTT		glActiveTexture(GL_TEXTURE0);/// HMMMM WRONG!!!!!
//
//
//- Mesh-184
// ---- PrimativeType
// ---- (N)Vertices
// ---- (N)Faces
// ---- Normals
// ---- Tangents
// ---- Bitangents
// ---- Colors
// ---- (N)TextureCoords
// ---- ---- { 2,0,0,0,0,0,0 }
// ---- (N)Bones
// ---- MaterialIndex
// ---- Name- Name of the Mesh....
// ---- (N)AnimMeshes
// ---- Method
//-Material-184
// ---- (N)Properties
// ---- (NumAllocated)
// ----
//----
//----
//----
//--Properties
//
// Naive DrawLoop
// foreach(object)
// {
// 	//bindFrameBuffer
// 	// Set depth blending etc...states
// 	// bind Shaders
// 	// Bind Textures
// 	// Bind Vertex/Index Buffers
// 	WriteUniformData(object)
// 	glDrawElementsBaseVertex(GL_TRIANGLES, object->indexCount, GL_UNSIGNED_SHORT, object->indexDataOffset, object->baseVertex);
// }
//
//
//foreach(render target) // Framebuffer
//foreach(render pass) // Depth blending etc states
//foreach(material)   // Shaders
//foreach(material instance) // Textures
//foreach(vertex format) // VertexBuffer < - Pack multiple buffers into single VBO
//foreach(object)
//{
// WriteUniformData(object);
// glDrawElementsBaseVertex(GL_TRIANGLES, object->indexCount, GL_UNSIGNED_SHORT, object->indexDataOffset, object->baseVertex);
// /\ Make this Indirect drawcall
//}
//
// Allocate with BufferStorage NOTE: Doing this requires glFenceSync() and glClientWaitSync();  glMemoryBarrier(GL_COMMAND_BARRIER_BIT) 
// glBufferStorage(GL_ARRAY_BUFFER, ringSize, NULL, flags);
// User GLbitfield flats = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT
// Streaming Geometry
// RingBuffer
// void *data = glMapBuffer(GL_ARRAY_BUFFER, ringOffset, dataSize, GL_MAP_UNSYNCHRONIZED | GL_MAP_WRITE_BIT);
// WriteGEometry(data, ...)
// glUnmapBuffer
// RingOffset += datasize <- deal with wrap arounds. 
//NOTE: BufferSubData() on Intel is bad, Map(UNSYNCH) bad for threaded Drivers like NVidia
// Persistant Mapping 2-20x better
//
//MultiDraw loop
// foreach(object)
// {
//     WriteUniformData(object, &UniformData[i]);
//     WriteDrawCommand(object, &commands[i]); <- Use parallelism whenever possible, GPU computer if possible
// }
//
// Shader takes struct of Uniforms 
// Takes Array of Structs for Multidraw indirect commands SSBO if lartge 
// use ShaderParams mat4 MVP = ShaderParams[gl_DrawIDARB].mvp
// if Not Supported use BaseInstance field of DrawStruct cmd->baseInstance=  drawCounter++;
// gl_InstanceID always counts from 0
// use VertexAttribute, glVertexAttribDivisor
// FillBuffer with your own IDs
// Some HW it is faster than using gl_DrawIDs
//
// Pack 2D Textures into Arrays
// Array Groups Textures With shame Shape Dimensions Format Mips MSAA
// TextureBiews push same size formats together
// Bind All Arrays to pipeline at once
//uniform sampler2Darray AllSamplers[MAX_ARRAY_TEXTURES] <- might run out of memory
// * Pair array with different sampler objs
// * Create Views of Array with Different States
// Each Combination needs new binding slot;
//
//// AZDO
// Persistant Mapped Buffers
// Indirect and MultiDraws
// Packing 2D Textures into Arrays
// Bindless Textures - Shaders Access Textures By handle
// Sparse Textures
//
// Typical Texture
//  glGenTextures(10, &TextureID[0]);
//  glBindTexture(GL_TEXTURE_2D, TextureID[0]);
//  glTexStorage(GL_TEXUTRE_2D, ...);
//  foreach(draw in draws) {
//  	foreach(texture in draw->textures) {
//  		glBindTexture(GL_TEXTURE_2D, TextureID[texture]);
//  	}
//  	//otherstuff
//  	glDrawElements(...);
//  }
//
//aiTextureType_OPACITY      
//aiTextureType_DISPLACEMENT 
//aiTextureType_LIGHTMAP     
//aiTextureType_REFLECTION   