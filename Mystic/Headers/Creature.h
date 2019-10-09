 #pragma once
#include <vector>
#include <iostream>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <array>

#include"Common.h"
#include"Mesh.h"
#include"Primitive.h"
//http://www.mathematik.uni-dortmund.de/~goeddeke/gpgpu/tutorial.html
class Organism;
class Node;
class Edge;
using range = std::pair<unsigned int, unsigned int>;

extern bool isWriting;
extern bool isReading;

template<typename T>
class ShaderBufferStorageObject
{
public:
	ShaderBufferStorageObject(std::vector<T> _data) 
	{
	//
	//glGenBuffers(1, &SSBO_ID);
	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_ID);
	//glBufferData(GL_SHADER_STORAGE_BUFFER,_data.size() * sizeof(T)​, _data​.data(), GL_STATIC_COPY);
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, SSBO_ID);
	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind
	}
	GLuint SSBO_ID;
};

#if 1

#include<iostream>
#include<fstream>
#include<sstream>

#define CREATURE_COUNT  1024

struct CreatureManager
{
	CreatureManager& Instance()
	{
		static CreatureManager instance;
		return instance;
	}

	CreatureManager(int cCount);
	bool Compute();
	void Bind();

	Vec4 Data[CREATURE_COUNT];
	Vec4 Results[CREATURE_COUNT];

	/// Constructor
 

	/// VAO and Attributes
	VertexArrayObject QuadVAO;
	Vec3 Quad[4];
	Vec2 TextureCoords[4];
	GLuint Indices[6];

	/// Local Textures and FrameBuffers
//	GLuint InputTexture = 0;
//	GLuint Input2Texture = 0;
	GLuint OutputTextureID = 0;
	GLuint TextureSize = 0;
	GLuint FrameBufferID = 0;

	/// OpenGL State Variables
	GLuint Target = GL_TEXTURE_RECTANGLE;
	GLuint InternalFormat = GL_RGBA32F;
	GLuint Format = GL_RGBA;

	/// Texture Uniform for linking Values to the Shader
//	textureUniform Input;

	/// Kernal for performing Calculations
	Shader *CreatureUpdater;

	GLuint Convolution;
	GLuint Kernal;

	void LoadCreatures();
	void Cleanup()
	{
		// clean up
		delete[](&Data);
		delete[](&Results);

		glDeleteFramebuffers(1, &FrameBufferID);
		glDeleteTextures(1, &OutputTextureID);
	}
	bool CheckFramebufferStatus()
	{
		GLenum status;
		status = (GLenum)glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);
		switch (status) {
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			return true;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			Print("Framebuffer incomplete,incomplete attachment\n");

			return false;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			Print("Unsupported framebuffer format\n");
			return false;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			Print("Framebuffer incomplete,missing attachment\n");
			return false;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			Print("Framebuffer incomplete,attached Images must have same dimensions\n");
			return false;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			Print("Framebuffer incomplete,attached Images must have same format\n");
			return false;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			Print("Framebuffer incomplete,missing draw buffer\n");
			return false;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			Print("Framebuffer incomplete,missing read buffer\n");
			return false;
		}
		return false;
	}

	std::vector<Vec3> VertexBuffer;
	///==========================================================================================================================
	/// CREATURE STUFF DOD Stuff
	///==========================================================================================================================

	TextureBufferObject<Vec4> OrganismBuffer;
	TextureBufferObject<Vec4> CellBuffer;
	TextureBufferObject<Vec4> SpringBuffer;
	TextureBufferObject<Vec4> Velocity;
	TextureBufferObject<Vec4> Force;
	TextureBufferObject<Vec4> Position;
	TextureBufferObject<Vec4> Acceleration;
	TextureBufferObject<Vec4> RestLength;
	TextureBufferObject<Vec4> Disp;

	std::vector<Organism> Creatures;
	std::vector <Vec4> CreatureData;

	std::vector <Node> Cells;
	std::vector <Vec4> CellData;
	std::vector <Vec4> cVelocity;
	std::vector <Vec4> cForce;
	std::vector <Vec4> cPosition;
	std::vector <Vec4> cAcceleration;

	std::vector <Edge> Springs;
	std::vector <Vec4> SpringData;
	std::vector <Vec4> sDisplacement;
	std::vector <Vec4> sRestLength;
private:
	CreatureManager() {}
	CreatureManager(CreatureManager& other) = delete;

};



class Organism
{
public:
	Organism(Mesh *_mesh);
	Vec3 Position;
	unsigned int ID;

	void MakeCell(int _id, Vec3 _pos);

	unsigned int CellStart;
	unsigned int CellEnd;

	static unsigned int NumberofCreatures;
 	static CreatureManager *Manager;
};

class Edge
{
public:
	Edge(unsigned int a, unsigned int b, float _tension);

	unsigned int Parent;
	unsigned int Other;

	Vec3 RestLength;
	Vec3 Displacement;

	float Tension;
	float Damper;

	Vec3 GetDisplacement(Vec3 _a, Vec3 _b)
	{
		return Vec3
		(
			_b.x - _a.x,
			_b.y - _a.y,
			_b.z - _a.z
		);
	}
};

class Node
{
public:
	Node(unsigned int _parent, Vec3 _pos);
	unsigned int Parent;

	unsigned int EdgeStart;
	unsigned int EdgeEnd;

	void MakeEdge(unsigned int a, unsigned int b);

	unsigned int VertexIndex;
	Vec3 Position;
	Vec3 Velocity;
	Vec3 Force;
	Vec3 Acceleration;
};


#else

#define THREAD_COUNT 4

struct CreatureManager
{
	CreatureManager()
	{
		HardwareThreads = std::thread::hardware_concurrency();
	}
	std::vector<Organism>  Creatures;
	std::vector<Node>  Cells;
	std::vector<Edge> Springs;
	std::vector<Vec3> Vertis;
	std::vector<Vec3> VertexBuffer;

	std::vector<std::vector<Edge*>> Groups;

	std::thread *UpdateLoop;

	static void MakeGroups();
	void UpdateVertices();
	void UpdateOrganisms();

	void UpdateCells();
	void UpdateSprings();

	void Scheduler();

	void GroupUpdater(int _group);
	void Updater(int &_trigger);
	int Trigger[THREAD_COUNT] = { 0 };

	std::atomic<int> GroupsFinished = 0;
	bool UpdaterLocked = false;
	unsigned int HardwareThreads;

	std::mutex UpdateLock;
	std::condition_variable CVready;
	
	std::atomic<bool> Done;
	void Close() {
		UpdateLoop->join();
	}
};
class Organism
{
public:
	Organism(Mesh *_mesh);

	Vec3 Position;

	unsigned int ID;
	void MakeCell(Vec3 _pos);
	void Update();
	unsigned int CellStart;
	unsigned int CellEnd;
	static unsigned int NumberofCreatures;

	static CreatureManager Manager;


};

class Edge
{
public:
	Edge(unsigned int a, unsigned int b, float _tension);

	unsigned int Parent;
	unsigned int Other;

	bool /// Likely Make this Atomic
		Busy = false,
		Idle = true,
		Finished = false;


	void Update();
	Vec3 RestLength;
	Vec3 Displacement;
	float Tension;
	float Damper;

	float Distance(Vec3 _first, Vec3 _second)
	{
		return sqrt(Squared(_first.x - _second.x) + Squared(_first.y - _second.y) + Squared(_first.z - _second.z));
	}

	Vec3 GetDisplacement(Vec3 _a, Vec3 _b)
	{
		return Vec3
		(
			_b.x - _a.x,
			_b.y - _a.y,
			_b.z - _a.z
		);
	}

};
class Node
{
public:
	Node(unsigned int _parent, Vec3 _pos);
	unsigned int Parent;
	unsigned int EdgeStart;
	unsigned int EdgeEnd;
	void MakeEdge(unsigned int a, unsigned int b);
	void Update();

	unsigned int VertexIndex;
	Vec3 Position;
	Vec3 Velocity;
	Vec3 Force;
	Vec3 Acceleration;
};

Node *GetCell(unsigned int _index);
#endif