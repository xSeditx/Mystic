#include"Assimp.h"
#include<vector>


class Mesh;
class Material;

class Exporter
{};

#include"Common.h"
#include"Mesh.h"
#include"Material.h"
class Importer
{
public:
//	static void ImportMesh(std::string _file, std::vector<Mesh> &_mesh, std::vector<Material> &_materials);
//	static void LoadMaterials(std::string _file, const aiScene * RawScene, std::vector<Material> &_materials);
//	static void LoadMeshes(std::string _file, const aiScene *_scene, std::vector<Mesh> & _mesh);


	static void ImportScene(std::string _file, std::vector<Mesh> &_mesh, std::vector<Material> &_materials)
	{

		// Create an instance of the Importer class
		const aiScene *RawScene = NULL;
		Assimp::Importer importer;
		// And have it read the given file with some example postprocessing
		// Usually - if speed is not the most important aspect for you - you'll 
		// propably to request more postprocessing than we do in this example.

		RawScene = importer.ReadFile
		(
			_file,
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

		///_mesh.Transform = *(Mat4*)RawScene->mRootNode->mTransformation[0];

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
			LoadMaterials(_file, RawScene, _materials);
			///	LoadMaterials(_file, RawScene, _mesh.IndirectDraw);
		}
		//====================================================================================================================================
		// LOAD ALL MESH ATTACHED TO THE SCENE
		//====================================================================================================================================

		if (RawScene->HasMeshes())
		{
			Importer::LoadMeshes(_file, RawScene, _mesh);
		}
	}

	static void LoadMaterials(std::string _file, const aiScene * RawScene, std::vector<Material> &_materials)
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
			std::string RealPath = _file.substr(0, _file.find_last_of("/\\")) + Path.C_Str(); // Find the Directory of the File we are parsing

			if (Path.length != 0)
			{
				Texture *PushTexture = new Texture(*&Bitmap(RealPath));
				//PushTexture.Load();/// only use this if loading from Image
				MatPush.Surfaces.push_back({ SurfaceMaterial::Type::DIFFUSE, *PushTexture });
			}

			Path = "";
			RawScene->mMaterials[MatIndex]->GetTexture(aiTextureType_HEIGHT, 0, &Path, NULL, &UVIndex, &Blend, &TextureOP, &MapMode);
			RealPath = _file.substr(0, _file.find_last_of("/\\")) + Path.C_Str(); // Find the Directory of the File we are parsing
			if (Path.length != 0)
			{
				Texture *PushTexture = new Texture(*&Bitmap(RealPath));
				MatPush.Surfaces.push_back({ SurfaceMaterial::Type::NORMALS, *PushTexture });
			}


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


			_materials.emplace_back(MatPush);
		}
	}
	static void LoadMeshes(std::string _file, const aiScene *_scene, std::vector<Mesh> & _mesh)
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
			//_scene->mMeshes[MeshIndex].
			for_loop(VertexIndex, _scene->mMeshes[MeshIndex]->mNumVertices)
			{// Loads all the Vertices from the Mesh File
				Vec3 Push;
				Push.x = _scene->mMeshes[MeshIndex]->mVertices[VertexIndex].x;
				Push.y = _scene->mMeshes[MeshIndex]->mVertices[VertexIndex].y;
				Push.z = _scene->mMeshes[MeshIndex]->mVertices[VertexIndex].z;

				Vertices.emplace_back(Push);

				if (_scene->mMeshes[MeshIndex]->HasVertexColors(VertexIndex))
				{// If the Vertices contain a Color Add it to the Mesh
					Vec4 ColorPush;
					ColorPush.r = _scene->mMeshes[MeshIndex]->mColors[VertexIndex]->r;
					ColorPush.g = _scene->mMeshes[MeshIndex]->mColors[VertexIndex]->g;
					ColorPush.b = _scene->mMeshes[MeshIndex]->mColors[VertexIndex]->b;
					ColorPush.a = _scene->mMeshes[MeshIndex]->mColors[VertexIndex]->a;
					Colors.emplace_back(ColorPush);
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
					Normals.emplace_back(NormPush);

					Vec4 ColorPush; /// TEMP JUST TO COLOR THE VERTICES WITH THE NORMALS
					ColorPush.r = _scene->mMeshes[MeshIndex]->mNormals[VertexIndex].x; /// TEMP JUST TO COLOR THE VERTICES
					ColorPush.g = _scene->mMeshes[MeshIndex]->mNormals[VertexIndex].y; /// TEMP JUST TO COLOR THE VERTICES
					ColorPush.b = _scene->mMeshes[MeshIndex]->mNormals[VertexIndex].z; /// TEMP JUST TO COLOR THE VERTICES
					ColorPush.a = 1.0;										    /// TEMP JUST TO COLOR THE VERTICES
					Colors.emplace_back(ColorPush); /// TEMP
				}// END OF Normals Load

				//====================================================================================================================================
				/// LOAD ALL TEXTURE COORDS AND THE MESH
				//====================================================================================================================================

				if (_scene->mMeshes[MeshIndex]->HasTextureCoords(0))
				{  // Assuming only one texture is attached to this mesh
					Vec2 UVPush;
					UVPush.x = _scene->mMeshes[MeshIndex]->mTextureCoords[0][VertexIndex].x;
					UVPush.y = _scene->mMeshes[MeshIndex]->mTextureCoords[0][VertexIndex].y;
					UVcoords.emplace_back(UVPush);
				}

				if (_scene->mMeshes[MeshIndex]->HasTangentsAndBitangents())
				{
					Vec3 TangentPush;
					TangentPush.x = _scene->mMeshes[MeshIndex]->mTangents[VertexIndex].x;
					TangentPush.y = _scene->mMeshes[MeshIndex]->mTangents[VertexIndex].y;
					TangentPush.z = _scene->mMeshes[MeshIndex]->mTangents[VertexIndex].z;
					Tangents.emplace_back(TangentPush);
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
					Indices.emplace_back(Push);
				}
			}

			//MeshPush.Transform = glm::mat4(1.0f); // Set Identity and Rotate all axis followed with the Translation.
			//MeshPush.Transform = glm::rotate(Transform, glm::radians(MeshPush.Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
			//MeshPush.Transform = glm::rotate(Transform, glm::radians(MeshPush.Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
			//MeshPush.Transform = glm::rotate(Transform, glm::radians(MeshPush.Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
			//MeshPush.Transform = glm::translate(Transform, MeshPush.Position);
			MeshPush.VAO = new VertexArrayObject();
			MeshPush.VAO->Attach(BufferTypes::VERTEX, new VertexBufferObject<Vec3>(&Vertices[0], Vertices.size()));
			if (Normals.size())    MeshPush.VAO->Attach(BufferTypes::NORMAL, new VertexBufferObject<Vec3>(&Normals[0], Normals.size()));
			if (UVcoords.size())   MeshPush.VAO->Attach(BufferTypes::UVCOORD, new VertexBufferObject<Vec2>(&UVcoords[0], UVcoords.size()));
			if (Colors.size())     MeshPush.VAO->Attach(BufferTypes::COLOR, new VertexBufferObject<Vec3>(&Colors[0], Colors.size()));
			if (Tangents.size())   MeshPush.VAO->Attach(BufferTypes::TANGENT, new VertexBufferObject<Vec3>(&Tangents[0], Tangents.size()));

			MeshPush.VAO->Attach(BufferTypes::INDICE, new VertexBufferObject<GLuint>(&Indices[0], Indices.size()));
			//		Renderer::Instance().Submit(MeshPush);
			MeshPush.Position = { _scene->mRootNode->mTransformation.a4, _scene->mRootNode->mTransformation.b4, _scene->mRootNode->mTransformation.c4 };
			_mesh.emplace_back(MeshPush);
		}
	}
};

