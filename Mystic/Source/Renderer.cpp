#include"Renderer.h"

 /*
 This entire Module sucks and needs to go.
 Just burn it down and start anew as it should have never existed in the first place and is just the result of boredom
 */


Scene::Scene(std::string _file)
	: 
	Filepath(_file),
	Camera(&Camera3D(Vec3(0), Vec3(0))),
    Transform(Mat4(1))
{
	Load();
}

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



//https://www.ebay.com/itm/AXIOM-4x-4GB-DDR3-1600MHZ-Ram-Desktop-Computer-Memory-Parts-others-Available/392309999483


void Scene::Load()
{
	// Create an instance of the Importer class
    const aiScene *RawScene = NULL;
	Assimp::Importer importer;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll 
	// propably to request more postprocessing than we do in this example.
	 
	RawScene = importer.ReadFile
	(
		Filepath,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType
	);

	if (!RawScene) 	// If the import failed, report it
	{ //DoTheErrorLogging(importer.GetErrorString());
		Print(importer.GetErrorString());
		__debugbreak();
	}

	Transform = *(Mat4*)RawScene->mRootNode->mTransformation[0];

	//====================================================================================================================================
	// LOAD ALL EMBEDDED TEXTURES FOR THE SCENE : note Rarely ever used
	//====================================================================================================================================
	if (RawScene->HasTextures())
	{// Have yet to see this used
		for_loop(TexIndex, RawScene->mNumTextures)
		{
			aiTexel *A = RawScene->mTextures[TexIndex]->pcData;
			__debugbreak();
		}
	}
	//====================================================================================================================================


	//====================================================================================================================================
	// LOAD ALL MATERIALS FOR THE SCENE
	//====================================================================================================================================
	if (RawScene->HasMaterials())
	{
		LoadMaterials(RawScene);
	}
	//====================================================================================================================================
	// LOAD ALL MESH ATTACHED TO THE SCENE
	//====================================================================================================================================

	if (RawScene->HasMeshes())
	{
		LoadMeshes(RawScene);
	}
}
/*
ORDER OF EXPENSE
================

FrameBufferObjects 60K/s
ShaderPrograms 300 K/s
ROP ??? 
TextureBinding 1.5 M/s
VertexFormat
UBO
VertexBindings 
UniformUpdates 10m/s
*/
void Scene::LoadMaterials(const aiScene * RawScene)
{

	for_loop(MatIndex, RawScene->mNumMaterials)
	{
		Material MatPush(Shader::GetActiveShader());

		//======================================= STUFF I AM TESTING GETTING ALL INFO FROM TEXTURE ======================================
		ai_real Blend = 0;
		aiString Path;
		unsigned int UVIndex = 0;
		aiTextureOp TextureOP = aiTextureOp_Multiply;
		aiTextureMapMode MapMode = aiTextureMapMode_Wrap;

		RawScene->mMaterials[MatIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, &UVIndex, &Blend, &TextureOP, &MapMode);
		std::string RealPath = Filepath.substr(0, Filepath.find_last_of("/\\")) + Path.C_Str(); // Find the Directory of the File we are parsing

		if (Path.length != 0)
		{
			Texture *PushTexture = new Texture(*&Image(RealPath));
			//PushTexture.Load();/// only use this if loading from image
			Textures.push_back(PushTexture);
			MatPush.Surfaces.push_back({ SurfaceMaterial::Type::DIFFUSE, *PushTexture });
		}

		Path = "";
		RawScene->mMaterials[MatIndex]->GetTexture(aiTextureType_HEIGHT, 0, &Path, NULL, &UVIndex, &Blend, &TextureOP, &MapMode);
		RealPath = Filepath.substr(0, Filepath.find_last_of("/\\")) + Path.C_Str(); // Find the Directory of the File we are parsing
		if (Path.length != 0)
		{
			Texture *PushTexture = new Texture(*&Image(RealPath));
			//PushTexture.Load();/// only use this if loading from image
			Textures.push_back(PushTexture);
			MatPush.Surfaces.push_back({ SurfaceMaterial::Type::NORMALS, *PushTexture });
		}

		//	Path = "";
		//	RawScene->mMaterials[MatIndex]->GetTexture(aiTextureType_SPECULAR, 0, &Path, NULL, &UVIndex, &Blend, &TextureOP, &MapMode);
		//	RealPath = Filepath.substr(0, Filepath.find_last_of("/\\")) + Path.C_Str(); // Find the Directory of the File we are parsing
		//	if (Path.length != 0)
		//	{
		//		Texture PushTexture = Texture(Image("no", RealPath));
		//		PushTexture.Load();/// only use this if loading from image
		//		Textures.push_back(PushTexture);
		//		MatPush.Surfaces.push_back({ SurfaceMaterial::Type::SHININESS, PushTexture });
		//	}
		//
		//	Path = "";
		//	RawScene->mMaterials[MatIndex]->GetTexture(aiTextureType_AMBIENT, 0, &Path, NULL, &UVIndex, &Blend, &TextureOP, &MapMode);
		//	RealPath = Filepath.substr(0, Filepath.find_last_of("/\\")) + Path.C_Str(); // Find the Directory of the File we are parsing
		//	if (Path.length != 0)
		//	{
		//		Texture PushTexture = Texture(Image("no", RealPath));
		//		PushTexture.Load();/// only use this if loading from image
		//		Textures.push_back(PushTexture);
		//		MatPush.Surfaces.push_back({ SurfaceMaterial::Type::AMBIANT, PushTexture });
		//	}
		//
		//	Path = "";
		//	RawScene->mMaterials[MatIndex]->GetTexture(aiTextureType_EMISSIVE, 0, &Path, NULL, &UVIndex, &Blend, &TextureOP, &MapMode);
		//	RealPath = Filepath.substr(0, Filepath.find_last_of("/\\")) + Path.C_Str(); // Find the Directory of the File we are parsing
		//	if (Path.length != 0)
		//	{
		//		Texture PushTexture = Texture(Image("no", RealPath));
		//		PushTexture.Load();/// only use this if loading from image
		//		Textures.push_back(PushTexture);
		//		MatPush.Surfaces.push_back({ SurfaceMaterial::Type::EMISSIVE, PushTexture });
		//	}
		//
		//	Path = "";
		//	RawScene->mMaterials[MatIndex]->GetTexture(aiTextureType_SHININESS, 0, &Path, NULL, &UVIndex, &Blend, &TextureOP, &MapMode);
		//	RealPath = Filepath.substr(0, Filepath.find_last_of("/\\")) + Path.C_Str(); // Find the Directory of the File we are parsing
		//	if (Path.length != 0)
		//	{
		//		Texture PushTexture = Texture(Image("no", RealPath));
		//		PushTexture.Load();/// only use this if loading from image
		//		Textures.push_back(PushTexture);
		//		MatPush.Surfaces.push_back({ SurfaceMaterial::Type::NORMALS, PushTexture });
		//	}
		//
					//https://www.ics.com/blog/qt-and-opengl-loading-3d-model-open-asset-import-library-assimp


		//--------------------------------------------THAT TUTORIAL------------------------------------------------------------------------------

		int textureIndex = 0;
		aiString textureFilename;
		aiReturn isTexturePresent = RawScene->mMaterials[MatIndex]->GetTexture
		(
			aiTextureType_DIFFUSE,
			textureIndex,
			&textureFilename
		);
		while (isTexturePresent == AI_SUCCESS)
		{
			// textureNameMap.insert(std::pair<std::string, GLuint>(textureFilename.data, 0));
			// more textures? more than one texture could be associated with a material
			textureIndex++;
			isTexturePresent = RawScene->mMaterials[MatIndex]->GetTexture(aiTextureType_DIFFUSE, textureIndex, &textureFilename);
			if (isTexturePresent == AI_SUCCESS)
			{
				Print("Second Texture");
			}
		}
		//----------------------------------------------------------------------------------------------------------------------------------------


		for_loop(PropIndex, RawScene->mMaterials[MatIndex]->mNumProperties)
		{ // Load All of the Properties
			MatPush.Properties[RawScene->mMaterials[MatIndex]->mProperties[PropIndex]->mKey.C_Str()] =
				MaterialProperties
				(
					RawScene->mMaterials[MatIndex]->mProperties[PropIndex]->mKey.C_Str(),
					RawScene->mMaterials[MatIndex]->mProperties[PropIndex]->mSemantic,
					RawScene->mMaterials[MatIndex]->mProperties[PropIndex]->mIndex,
					RawScene->mMaterials[MatIndex]->mProperties[PropIndex]->mDataLength,
					RawScene->mMaterials[MatIndex]->mProperties[PropIndex]->mData,
					RawScene->mMaterials[MatIndex]->mProperties[PropIndex]->mType
				);
		}
		
		
		float Shine = 0, Strength = 0;
		if (RawScene->mMaterials[MatIndex]->Get(AI_MATKEY_SHININESS, Shine) == AI_SUCCESS)
		{
			MatPush.Shininess = Shine;
		}
		if (RawScene->mMaterials[MatIndex]->Get(AI_MATKEY_SHININESS_STRENGTH, Strength) == AI_SUCCESS)
		{
			MatPush.Shine_Strength = Strength;
		}

		
		Materials.push_back(MatPush);
	}
}


///	MaterialProperties prop;
/// prop.DataLength = RawScene->mMaterials[MatIndex]->mProperties[PropIndex]->mDataLength;
/// prop.Index = RawScene->mMaterials[MatIndex]->mProperties[PropIndex]->mIndex;
/// prop.Semantic = RawScene->mMaterials[MatIndex]->mProperties[PropIndex]->mSemantic;
/// prop.Type = RawScene->mMaterials[MatIndex]->mProperties[PropIndex]->mType;
/// // Allocate space for Data
/// prop.Data = new char[sizeof(RawScene->mMaterials[MatIndex]->mProperties[PropIndex]->mDataLength)];
/// prop.Data = RawScene->mMaterials[MatIndex]->mProperties[PropIndex]->mData;// Apply Data
/// aiString K = RawScene->mMaterials[MatIndex]->mProperties[PropIndex]->mKey;
 
void Scene::LoadMeshes(const aiScene *_scene)
{
    TODO("I do not believe the Materials are being loaded correctly, they all have the same KEy and data as though It is not properly cycling over them");

	for_loop(MeshIndex, _scene->mNumMeshes)
	{ // Cycle Through all Mesh Objects in the file
		std::vector<Vec3> Vertices;
		std::vector<Vec3> Colors;
		std::vector<Vec3> Normals;
		std::vector<Vec2> UVcoords;
		std::vector<GLuint> Indices;
		std::vector<Vec3> Tangents;
		std::vector<Vec3> BiTangents;

		Mesh MeshPush;

		// Lookup for what material to use
		MeshPush.MaterialID = _scene->mMeshes[MeshIndex]->mMaterialIndex;
		MeshPush.GetPrimativeType(_scene->mMeshes[MeshIndex]->mPrimitiveTypes);
		// RawScene->mMaterials[_meshID]
		// create material uniform buffer
	    // aiMaterial *mtl = sc->mMaterials[mesh->mMaterialIndex];
		// Data->mNumMaterials[0].
		//====================================================================================================================================
		/// LOAD ALL VERTICES AND ATTRIBUTES FOR THE MESH
		//====================================================================================================================================

		for_loop(VertexIndex, _scene->mMeshes[MeshIndex]->mNumVertices)
		{// Loads all the Vertices from the Mesh File
			Vec3 Push;
			Push.x = _scene->mMeshes[MeshIndex]->mVertices[VertexIndex].x;
			Push.y = _scene->mMeshes[MeshIndex]->mVertices[VertexIndex].y;
			Push.z = _scene->mMeshes[MeshIndex]->mVertices[VertexIndex].z;

			Vertices.push_back(Push);

			if (_scene->mMeshes[MeshIndex]->HasVertexColors(VertexIndex))
			{// If the Vertices contain a Color Add it to the Mesh
				Vec4 ColorPush;
				ColorPush.r = _scene->mMeshes[MeshIndex]->mColors[VertexIndex]->r;
				ColorPush.g = _scene->mMeshes[MeshIndex]->mColors[VertexIndex]->g;
				ColorPush.b = _scene->mMeshes[MeshIndex]->mColors[VertexIndex]->b;
				ColorPush.a = _scene->mMeshes[MeshIndex]->mColors[VertexIndex]->a;
				Colors.push_back(ColorPush);
			}// END OF Colors Load

			//================================================================================================================================
			///LOAD NORMALS
			//================================================================================================================================
			if (_scene->mMeshes[MeshIndex]->HasNormals())
			{// If the Vertex contains Normals, supply them
				Vec3 NormPush;
				NormPush.x = _scene->mMeshes[MeshIndex]->mNormals[VertexIndex].x;
				NormPush.y = _scene->mMeshes[MeshIndex]->mNormals[VertexIndex].y;
				NormPush.z = _scene->mMeshes[MeshIndex]->mNormals[VertexIndex].z;
				Normals.push_back(NormPush);

				Vec4 ColorPush; /// TEMP JUST TO COLOR THE VERTICES WITH THE NORMALS
				ColorPush.r = _scene->mMeshes[MeshIndex]->mNormals[VertexIndex].x; /// TEMP JUST TO COLOR THE VERTICES
				ColorPush.g = _scene->mMeshes[MeshIndex]->mNormals[VertexIndex].y; /// TEMP JUST TO COLOR THE VERTICES
				ColorPush.b = _scene->mMeshes[MeshIndex]->mNormals[VertexIndex].z; /// TEMP JUST TO COLOR THE VERTICES
				ColorPush.a = 1.0;										    /// TEMP JUST TO COLOR THE VERTICES
				Colors.push_back(ColorPush); /// TEMP
			}// END OF Normals Load

			//====================================================================================================================================
			/// LOAD ALL TEXTURE COORDS AND THE MESH
			//====================================================================================================================================

			if (_scene->mMeshes[MeshIndex]->HasTextureCoords(0))
			{  // Assuming only one texture is attached to this mesh
				Vec2 UVPush;
				UVPush.x = _scene->mMeshes[MeshIndex]->mTextureCoords[0][VertexIndex].x;
				UVPush.y = _scene->mMeshes[MeshIndex]->mTextureCoords[0][VertexIndex].y;
 				UVcoords.push_back(UVPush);
			}

			if (_scene->mMeshes[MeshIndex]->HasTangentsAndBitangents())
			{
				Vec3 TangentPush;
				TangentPush.x = _scene->mMeshes[MeshIndex]->mTangents[VertexIndex].x;
				TangentPush.y = _scene->mMeshes[MeshIndex]->mTangents[VertexIndex].y;
				TangentPush.z = _scene->mMeshes[MeshIndex]->mTangents[VertexIndex].z;
				Tangents.push_back(TangentPush);
			}
		}// END OF MESH AND PROPERTIES
		std::vector<Vec3> CorrectedV;
		//Vec3 Avg;
		//for (auto & V: Vertices)
		//{
		//	Avg = V;
		//}
		//Avg /= Vertices.size();
		//MeshPush.Position = Avg;

		//====================================================================================================================================
		/// LOAD ALL INDICES
		//====================================================================================================================================

		for_loop(FaceIndex, _scene->mMeshes[MeshIndex]->mNumFaces)
		{// Load All Faces from the Mesh File
			GLuint Push;
			for_loop(Index3, _scene->mMeshes[MeshIndex]->mFaces[FaceIndex].mNumIndices)
			{// Gets all the Indices from the Mesh Faces
				Push = _scene->mMeshes[MeshIndex]->mFaces[FaceIndex].mIndices[Index3];
				Indices.push_back(Push);
			}
		}

		//MeshPush.Transform = glm::mat4(1.0f); // Set Identity and Rotate all axis followed with the Translation.
		//MeshPush.Transform = glm::rotate(Transform, glm::radians(MeshPush.Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
		//MeshPush.Transform = glm::rotate(Transform, glm::radians(MeshPush.Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
		//MeshPush.Transform = glm::rotate(Transform, glm::radians(MeshPush.Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
		//MeshPush.Transform = glm::translate(Transform, MeshPush.Position);
        MeshPush.VAO = new VertexArrayObject();
		MeshPush.VAO->Attach(BufferTypes::VERTEX, new VertexBufferObject<Vec3>(&Vertices[0], Vertices.size()));
		if (Normals.size())    MeshPush.VAO->Attach(BufferTypes::NORMAL,    new VertexBufferObject<Vec3>( &Normals[0],    Normals.size()));
		if (UVcoords.size())   MeshPush.VAO->Attach(BufferTypes::UVCOORD,   new VertexBufferObject<Vec2>( &UVcoords[0],   UVcoords.size()));
		if (Colors.size())     MeshPush.VAO->Attach(BufferTypes::COLOR,     new VertexBufferObject<Vec3>( &Colors[0],     Colors.size()));
		if (Tangents.size())   MeshPush.VAO->Attach(BufferTypes::TANGENT,   new VertexBufferObject<Vec3>( &Tangents[0],   Tangents.size()));

		MeshPush.VAO->Attach(BufferTypes::INDICE, new VertexBufferObject<GLuint>(&Indices[0], Indices.size()));
//		Renderer::Instance().Submit(MeshPush);
		Meshes.push_back(MeshPush);
	}
}







//====================================================================================================================================================================================
/// RENDERING STUFF. I INTEND TO BREAK THIS INTO MULTI-THREADED RENDERING AND PSEUDO MT RENDERING AND DISCARD THIS RENDERER IN THE FUTURE IN FAVOR OF TASK BASED RENDERING
//====================================================================================================================================================================================
//
//void Scene::Render()
//{
//	for (auto &G : Groups)
//	{
//		G.Render();
//	}
//}
//
void Scene::Update()
{/// We can do this in Parallel
//for (auto &G : Groups)
//{
//	G.Update();
//}
}

#include"Camera.h"


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


//void Renderer::Submit(VertexArrayObject &_vao, Material &_mat)
//{
  //  _mat.
//}

                       
void Renderer::Submit(Renderable &_object)
{
//    if (MaterialMeshMap[_object.MaterialID]);
}









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

//MultiDraw loop
// foreach(object)
// {
//     WriteUniformData(object, &UniformData[i]);
//     WriteDrawCommand(object, &commands[i]); <- Use parallelism whenever possible, GPU computer if possible
// }

// Shader takes struct of Uniforms 
// Takes Array of Structs for Multidraw indirect commands SSBO if lartge 
// use ShaderParams mat4 MVP = ShaderParams[gl_DrawIDARB].mvp
// if Not Supported use BaseInstance field of DrawStruct cmd->baseInstance=  drawCounter++;
// gl_InstanceID always counts from 0
// use VertexAttribute, glVertexAttribDivisor
// FillBuffer with your own IDs
// Some HW it is faster than using gl_DrawIDs


// Pack 2D Textures into Arrays
// Array Groups Textures With shame Shape Dimensions Format Mips MSAA
// TextureBiews push same size formats together
// Bind All Arrays to pipeline at once
//uniform sampler2Darray AllSamplers[MAX_ARRAY_TEXTURES] <- might run out of memory
// * Pair array with different sampler objs
// * Create Views of Array with Different States
// Each Combination needs new binding slot;



// AZDO
// Persistant Mapped Buffers
// Indirect and MultiDraws
// Packing 2D Textures into Arrays
// Bindless Textures - Shaders Access Textures By handle
// Sparse Textures


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







//aiTextureType_OPACITY      
//aiTextureType_DISPLACEMENT 
//aiTextureType_LIGHTMAP     
//aiTextureType_REFLECTION   