#pragma once
// http://assimp.sourceforge.net/lib_html/files.html
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#pragma comment(lib,"assimpmt.lib")

#include"Common.h"

/*
Will likely convert my entire Mesh structure into something that mimics the structure Assimp supplies for their loaded format.
*/

struct ImportedScene
{
	bool hasLights;
	bool hasMaterial;
	bool hasMeshes;
	bool hasTextures;
	bool hasAnimations;
	bool hasCameras;
	bool hasFlags;
	aiAnimation Animations;
	aiCamera Cameras;
	//aiFlags Flags;
	aiLight Lights;
	aiMaterial Materials;
	aiMesh Meshes;
	//aiFlags
	 
	unsigned int NumAnimations;
	unsigned int NumCameras;
	unsigned int NumLights;
};


#define AI_Call(x) if((int err = x) != 0){\
switch(err){\
case aiReturn_FAILURE:\
DEBUG_TRACE("Call Failed");\
break;\
case aiReturn_OUTOFMEMORY:\
DEBUG_TRACE("AssImp: Out Of Memory");\
break;\
}\
}



struct AssImpMesh
{
	unsigned int mPrimitiveTypes;

	unsigned int mNumVertices;

	unsigned int mNumFaces;

	C_STRUCT aiVector3D* mVertices;
	C_STRUCT aiVector3D* mNormals;
	C_STRUCT aiVector3D* mTangents;
	C_STRUCT aiVector3D* mBitangents;
	C_STRUCT aiColor4D* mColors[AI_MAX_NUMBER_OF_COLOR_SETS];
	C_STRUCT aiVector3D* mTextureCoords[AI_MAX_NUMBER_OF_TEXTURECOORDS];

	unsigned int mNumUVComponents[AI_MAX_NUMBER_OF_TEXTURECOORDS];

	C_STRUCT aiFace* mFaces;

	unsigned int mNumBones;

	C_STRUCT aiBone** mBones;

	unsigned int mMaterialIndex;

	C_STRUCT aiString mName;
	unsigned int mNumAnimMeshes;
	C_STRUCT aiAnimMesh** mAnimMeshes;
	unsigned int mMethod;

	AssImpMesh()
		: mPrimitiveTypes(0)
		, mNumVertices(0)
		, mNumFaces(0)
		, mVertices(NULL)
		, mNormals(NULL)
		, mTangents(NULL)
		, mBitangents(NULL)
		, mFaces(NULL)
		, mNumBones(0)
		, mBones(NULL)
		, mMaterialIndex(0)
		, mNumAnimMeshes(0)
		, mAnimMeshes(NULL)
		, mMethod(0)
	{
		for (unsigned int a = 0; a < AI_MAX_NUMBER_OF_TEXTURECOORDS; a++)
		{
			mNumUVComponents[a] = 0;
			mTextureCoords[a] = NULL;
		}

		for (unsigned int a = 0; a < AI_MAX_NUMBER_OF_COLOR_SETS; a++)
			mColors[a] = NULL;
	}

	//! Deletes all storage allocated for the mesh
	~AssImpMesh()
	{
		delete[] mVertices;
		delete[] mNormals;
		delete[] mTangents;
		delete[] mBitangents;
		for (unsigned int a = 0; a < AI_MAX_NUMBER_OF_TEXTURECOORDS; a++) {
			delete[] mTextureCoords[a];
		}
		for (unsigned int a = 0; a < AI_MAX_NUMBER_OF_COLOR_SETS; a++) {
			delete[] mColors[a];
		}

		// DO NOT REMOVE THIS ADDITIONAL CHECK
		if (mNumBones && mBones) {
			for (unsigned int a = 0; a < mNumBones; a++) {
				delete mBones[a];
			}
			delete[] mBones;
		}

		if (mNumAnimMeshes && mAnimMeshes) {
			for (unsigned int a = 0; a < mNumAnimMeshes; a++) {
				delete mAnimMeshes[a];
			}
			delete[] mAnimMeshes;
		}

		delete[] mFaces;
	}

	//! Check whether the mesh contains positions. Provided no special
	//! scene flags are set, this will always be true
	bool HasPositions() const
	{
		return mVertices != NULL && mNumVertices > 0;
	}

	//! Check whether the mesh contains faces. If no special scene flags
	//! are set this should always return true
	bool HasFaces() const
	{
		return mFaces != NULL && mNumFaces > 0;
	}

	//! Check whether the mesh contains normal vectors
	bool HasNormals() const
	{
		return mNormals != NULL && mNumVertices > 0;
	}
	bool HasTangentsAndBitangents() const
	{
		return mTangents != NULL && mBitangents != NULL && mNumVertices > 0;
	}
	bool HasVertexColors(unsigned int pIndex) const
	{
		if (pIndex >= AI_MAX_NUMBER_OF_COLOR_SETS)
			return false;
		else
			return mColors[pIndex] != NULL && mNumVertices > 0;
	}

	bool HasTextureCoords(unsigned int pIndex) const
	{
		if (pIndex >= AI_MAX_NUMBER_OF_TEXTURECOORDS)
			return false;
		else
			return mTextureCoords[pIndex] != NULL && mNumVertices > 0;
	}

	unsigned int GetNumUVChannels() const
	{
		unsigned int n = 0;
		while (n < AI_MAX_NUMBER_OF_TEXTURECOORDS && mTextureCoords[n])++n;
		return n;
	}

	unsigned int GetNumColorChannels() const
	{
		unsigned int n = 0;
		while (n < AI_MAX_NUMBER_OF_COLOR_SETS && mColors[n])++n;
		return n;
	}

	inline bool HasBones() const
	{
		return mBones != NULL && mNumBones > 0;
	}
};

//glm::vec3& operator= (glm::vec3&, const aiVector3D&)
//{
//
//}

//  struct AssimpMesh
//  {
//  	AIVerts
//  };
//  struct AssimpMaterial
//  {
//  	void GetTexture(int type)
//  	{}
//  };
//  
//  struct AssimpScene
//  {
//  	RootNode;
//  	AssimpMesh Meshes[];
//  	AssimpMaterial Materials[];
//  };



/// <aiScene>
/// =========
///<mNumMeshes(number of meshes for the entire scene)>
///<mMeshes(array of meshes)>
///---- mNumFaces(number of faces)
///---- mFaces(array of faces, usually triangles)
///---- mNumIndices(number of vertex indices per face, usually 3)
///---- mIndices(array of vertex indices, also called elements)
///---- mNumVertices(number of vertices for this mesh)
///---- mVertices(vertex array.Each vertex is a 3 dimensional vector.)
///---- mNormals(array of normals, aligned with the vertex array)
///---- mTextureCoords(array of texture coordinates, aligned with the vertex array)
///---- mMaterialIndex(the index of the material associated with this mesh)

///< mNumMaterials(number of materials in the scene)>
///< mMaterials(array of materials)>
///---- GetTextureCount() (get the number of texture attached to this material)
///---- GetTexture() (get the path of the texture files attached to this material)

///<mRootNode(root node of the scene graph)>
/// ---- mNumChildren(the number of children for this node)
/// ---- mChildren(an array of child nodes)
// ------ mNumChildren(the number of child nodes for this node)
// ------ mChildren(an array of child nodes)
// ------ mTransformation(transformation matrix for this node)
// ------ mNumMeshes(number of meshes associated with this node)
// ------ mMeshes(an array of the indices of the meshes associated with this node)

