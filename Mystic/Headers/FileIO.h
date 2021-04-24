#include"Assimp.h"
#include<vector>


#include"Common.h"


class Mesh;
struct Material;

struct Path
{
	Path(std::string _file)
		:
		Fullpath(_file)
	{}

	std::string extract_filename(){}
	std::string Fullpath;
	std::string Filename;
};

class Exporter
{
	static void ExportScene(std::vector<Mesh>& _mesh, std::vector<Material>& _materials);
	static void ConvertFile(std::string _fileIN, std::string _fileOUT);
};

class Importer
{
public:
    static void ImportScene(std::string _file, std::vector<Mesh> &_mesh, std::vector<Material> &_materials);
    static void LoadMaterials(std::string _file, const aiScene * RawScene, std::vector<Material> &_materials);
    static void LoadMeshes(std::string _file, const aiScene *_scene, std::vector<Mesh> & _mesh);
	static void ImportAnimatedMesh(std::string _file);
};


//void ImportAnimatedMesh(const std::string& _file)
//{
	// Release the previously loaded mesh (if it exists) Clear();
//
//	// Create the VAO
//	GLuint VAO{ 0 }, VBO{ 0 };
//	glGenVertexArrays(1, &VAO);
//	glBindVertexArray(VAO);
//
//	// Create the buffers for the vertices attributes
//	glGenBuffers(1, &VBO);
//
//	bool Ret = false;
//
//	m_pScene = m_Importer.ReadFile(_file.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals |
//		aiProcess_FlipUVs);
//
//	if (m_pScene) {
//		Mat4 m_GlobalInverseTransform = m_pScene->mRootNode->mTransformation;
//		m_GlobalInverseTransform = glm::inverse(m_GlobalInverseTransform);
//
//		Ret = InitFromScene(m_pScene, _file);
//	}
//	else {
//		printf("Error parsing '%s': '%s'\n", _file.c_str(), m_Importer.GetErrorString());
//	}
//
//	// Make sure the VAO is not changed from the outside
//	glBindVertexArray(0);

//}