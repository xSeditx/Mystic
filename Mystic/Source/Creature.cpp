#include"Creature.h"
#include"Common.h"
#include"ThreadPool.h"

///https://www.seas.upenn.edu/~cis565/fbo.htm

#if 1

CreatureManager *Organism::Manager;
unsigned int Organism::NumberofCreatures = 0;

CreatureManager::CreatureManager(int _cCount)
{
	//std::unique_ptr<Shader> temp = std::make_unique<Shader>("Resources//Creature.sfx");
	//CreatureUpdater.swap(temp);

	Organism::Manager = this;
	//std::unique_ptr<CreatureManager> temp = std::make_unique<CreatureManager>() temp Manager;
	float data2[32 * 32];
	for_loop(I, CREATURE_COUNT)
	{
		data2[I] = (float)I;
	}

	glEnable(GL_TEXTURE_2D); 
	glEnable(GL_TEXTURE_RECTANGLE);
	glEnable(GL_TEXTURE_BUFFER);
	for_loop(I, CREATURE_COUNT)
	{/// Place data into the Data Array for Reading and Writing with the shader
		Data[I].x =  I;
		Data[I].y =  I;
		Data[I].z =  I;
		Data[I].w =  I;
		Results[I] = Vec4(44, 55, 66, 77);
	}

	/// Fill the Data structures with appropriate Data
	{
		{/// Does any of this matter anymore?
			Target =  GL_TEXTURE_2D; // GL_TEXTURE_RECTANGLE;//
			InternalFormat = GL_RGBA32F;
			Format = GL_RGBA;
			TextureSize = std::ceil(std::sqrt(CREATURE_COUNT));
			OutputTextureID = 0;
			FrameBufferID = 0;
		}
		LoadCreatures();
		/// Create the Data for Handling the Frame Buffer Object rendering properly. Basically just a Quad that extends to give us the exact amount of creatures we wish to calculate the positioning of
		{
			float Value = 1;// TextureSize;
			TextureCoords[0] = Vec2(0.0f, 0.0f);
			TextureCoords[1] = Vec2(0.0f, Value);
			TextureCoords[2] = Vec2(Value, Value);
			TextureCoords[3] = Vec2(Value, 0.0f);

			Quad[0] = Vec3(-Value, -Value, 0.0f); // 2
			Quad[1] = Vec3(-Value,  Value, 0.0f); // 0 
			Quad[2] = Vec3( Value,  Value, 0.0f); // 1
			Quad[3] = Vec3( Value, -Value, 0.0f); // 3

			//(0, 1, 3)
			Indices[0] = 0;
			Indices[1] = 2;
			Indices[2] = 1;
			//(2, 3, 1)
			Indices[3] = 0; 
			Indices[4] = 3;
			Indices[5] = 2;
		}
		/// Create a VertexArrayObject and Attach out Attribute buffers to it
		{
			QuadVAO.Attach(BufferTypes::VERTEX, new VertexBufferObject<Vec3>(&Quad[0], 4));
			QuadVAO.Attach(BufferTypes::UVCOORD, new VertexBufferObject<Vec2>(&TextureCoords[0], 4));
			QuadVAO.Attach(BufferTypes::INDICE, new VertexBufferObject<GLuint>(&Indices[0], 6));
		}
		/// Create and compile our GLSL program from the shaders
		//CreatureUpdater{ "Resources\\Creature.sfx" };
	}
	/// Set OpenGL state
	{ 
		glPolygonMode(GL_FRONT, GL_FILL);
		glViewport(0, 0, TextureSize, TextureSize);
		glDisable(GL_DEPTH_TEST);
	}
	// Creature the Texture Buffer Objects of which we are going to access in the Shader
	{
		Disp = TextureBufferObject<Vec4>("Displacement", sDisplacement);
		Force = TextureBufferObject<Vec4>("Force", cForce);
		Velocity = TextureBufferObject<Vec4>("Velocity", cVelocity);
		Position = TextureBufferObject<Vec4>("Position", cPosition);
		CellBuffer = TextureBufferObject<Vec4>("Cells", CellData);
		RestLength = TextureBufferObject<Vec4>("RestLength", sRestLength);
		Acceleration = TextureBufferObject<Vec4>("Acceleration", cAcceleration);
		SpringBuffer = TextureBufferObject<Vec4>("Springs", SpringData);
		OrganismBuffer = TextureBufferObject<Vec4>("Creatures", CreatureData);
	}
	/// CREATE A FRAMEBUFFER OBJECT
	{
		const GLenum AttachmentType[1] =
		{
			GL_COLOR_ATTACHMENT0
		};

		glGenTextures(1, &OutputTextureID);
		glBindTexture(Target, OutputTextureID);

		// Set Wrap mode to ensure clamping and getting the exact pixel we desire. 
		glDrawBuffers(sizeof(AttachmentType) / sizeof(AttachmentType[0]), AttachmentType);
		glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexImage2D(Target, 0, InternalFormat, TextureSize, TextureSize, 0, Format, GL_FLOAT, Data);

		glGenFramebuffers(1, &FrameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Target, OutputTextureID, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
	}


	if (CheckFramebufferStatus() == false) __debugbreak();
}

void CreatureManager::LoadCreatures()
{
	/// Move the Structured data into Unstructure Data Arrays for Transfer to the GPU
	{
		for (auto&O : Creatures)
		{
			CreatureData.push_back(Vec4(O.CellEnd, O.CellStart, O.ID, 0.0f));
		}
		for (auto& C : Cells)
		{
			CellData.push_back(Vec4(C.EdgeStart, C.EdgeEnd, C.Parent, 0));
			cVelocity.push_back(Vec4(C.Velocity.x, C.Velocity.y, C.Velocity.x, 0));
			cForce.push_back(Vec4(C.Force.x, C.Force.y, C.Force.z, 0));
			cPosition.push_back(Vec4(C.Position.x, C.Position.y, C.Position.z, 0));
		}

		for (auto& S : Springs)
		{
			sRestLength.push_back(Vec4(S.RestLength.x, S.RestLength.x, S.RestLength.x, S.Tension));
			sDisplacement.push_back(Vec4(S.Displacement.x, S.Displacement.y, S.Displacement.z, S.Damper));
		}
		///GL_MAX_TEXTURE_BUFFER_SIZE  Find TEXTURE_BUFFER size
	}
}
void CreatureManager::Bind()
{
	glDisable(GL_BLEND);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferID);
	glPolygonMode(GL_FRONT, GL_FILL);
	glViewport(0, 0, TextureSize, TextureSize);

	//	glClearColor(0.0f, 0.0f,0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

//	CreatureUpdater.Enable(); /// Enable Shader

//	Shader::GetActiveShader()->DiffuseTexture.Set(0);
 	OrganismBuffer.Bind(1);
	CellBuffer.Bind(2);
	RestLength.Bind(3);
    Disp.Bind(4);
    Velocity.Bind(5);
    Force.Bind(6);
    Position.Bind(7);
    Acceleration.Bind(8);
    SpringBuffer.Bind(9);
}
bool CreatureManager::Compute()
{
 	QuadVAO.Render();
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	_GL(glReadPixels(0, 0, TextureSize, TextureSize, GL_RGBA, GL_FLOAT, Results));
	return true;
}

///=======================================================================================================================================
/// Organism Class
///=======================================================================================================================================

Organism::Organism(Mesh *_mesh)
	:
	ID(NumberofCreatures++)
{
	Position = _mesh->Position;
	CellStart = Manager->Cells.size();
	std::unordered_map<GLuint, std::vector<GLuint>> MapVertices;
	std::vector<Vec3> Vertices;

	int IndicesCount = _mesh->VAO->ElementCount / 3;
	int VerticeCount = _mesh->VAO->Buffers[0]->ElementCount;
	///---------------------------------------------------------------------------------
	/// 4 is the Indices, need to fix this whole fucking Buffer Attributes system
	iVec3 *Data = (iVec3*)_mesh->VAO->Buffers[4]->Map(GL_READ_ONLY);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	Vec3 *Verts = (Vec3*)_mesh->VAO->Buffers[0]->Map(GL_READ_ONLY);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	///---------------------------------------------------------------------------------

	for_loop(I, IndicesCount)
	{
		MapVertices[Data[I].x].push_back(Data[I].y);
		MapVertices[Data[I].x].push_back(Data[I].z);
		MapVertices[Data[I].y].push_back(Data[I].x);
		MapVertices[Data[I].y].push_back(Data[I].z);
		MapVertices[Data[I].z].push_back(Data[I].y);
		MapVertices[Data[I].z].push_back(Data[I].x);
	}

	for_loop(I, VerticeCount)
	{// Make All The Cells for This Model
		MakeCell(ID, Verts[I] + Position);
	}
	CellEnd = Manager->Cells.size();

	for (int i = CellStart; i < CellEnd; i++)
	{// Get the first and the End of the Cells that make up this Organism from the Cell List
		int OffsetIndex = i - CellStart;
		Manager->Cells[i].EdgeStart = Manager->Springs.size();
		for (auto& E : MapVertices[OffsetIndex])
		{
			Manager->Cells[i].MakeEdge(i, E + CellStart);
		}
		Manager->Cells[i].EdgeEnd = Manager->Springs.size();
	}

	Manager->Creatures.push_back(*this);
}

void Organism::MakeCell(int id, Vec3 _pos)
{
	unsigned int Start = Organism::Manager->Cells.size();
	Organism::Manager->Cells.push_back(Node(id, _pos));
	unsigned int End = Organism::Manager->Cells.size();
}

///=======================================================================================================================================
/// Edge Class
///=======================================================================================================================================
Edge::Edge(unsigned int a, unsigned int b, float _tension)
	:
	Parent(a),
	Other(b),
	Tension(_tension),
	Damper(1.f),
	RestLength(0),
	Displacement(0)
{
	RestLength = GetDisplacement
	(
		Organism::Manager->Cells[Parent].Position,
		Organism::Manager->Cells[Other].Position
	);
}
 
Node *GetCell(unsigned int _index)
{
	return  &Organism::Manager->Cells[_index];
}

///=======================================================================================================================================
/// Node Class
///=======================================================================================================================================

Node::Node(unsigned int _parent, Vec3 _pos)
	:
	Position(_pos),
	Parent(_parent),
	Velocity(0),
	Force(0),
	Acceleration(0),
	EdgeEnd(0),
	EdgeStart(0)
{
	VertexIndex = Organism::Manager->VertexBuffer.size();
	EdgeStart = Organism::Manager->Springs.size();
	Organism::Manager->VertexBuffer.push_back(_pos);
}
void Node::MakeEdge(unsigned int a, unsigned int b)
{
	Organism::Manager->Springs.push_back(Edge(a, b, RANDOM(.5)));
	EdgeEnd = Organism::Manager->Springs.size();
}


#else
//Mesh Organism::CreatureList;
bool isWriting = false;
bool isReading = false;
 
CreatureManager Organism::Manager;

unsigned int Organism::NumberofCreatures = 0;
#define NUMBER_OF_GROUPS  THREAD_COUNT
/*

Organism
-- Node
----Edge
Edge Updates the Position of the Nodes
*/
using namespace std::chrono_literals;

///=======================================================================================================================================
/// Creature Manager Class
/// This is a Manager Class for the Organisms and is more of a DOD approch to the system. Its performance is indeed much faster however
/// I feel I have much cleaning up to do and the creation of the ECS system should aid me in that respects
///=======================================================================================================================================
void CreatureManager::UpdateCells()
{/// CALLED IN THE SECOND THREAD UPDATE LOOP
	//for (auto& C : Cells)
	for_loop(I, Cells.size())
	{/// Update the Physics stuff, later this will be done via seperate class and methods for the physics
		Node *C = &Cells[I];

		float Timestep = .1f;
		C->Velocity = C->Velocity * .5f;// Friction velocity loss is .95 or 95% of velocity is retained

		Vec3  Last_Acceleration = C->Acceleration;
		C->Position += C->Velocity * Timestep + (Last_Acceleration * 0.5f * Squared(Timestep));
		C->Acceleration = C->Force / 10.0f;// Mass = 10;
		C->Force = Vec3(0.0f);

		Vec3 Avg_Acceleration = (Last_Acceleration + C->Acceleration) *.5f; // 2.0f;
		C->Velocity += Avg_Acceleration * Timestep;
		VertexBuffer[C->VertexIndex] = C->Position;
	 	 C->Force.x += RANDOM_SPREAD(.1);
         C->Force.y += RANDOM_SPREAD(.1);
         C->Force.z += RANDOM_SPREAD(.1);

	}
}
void CreatureManager::MakeGroups()
{
	int Count = Organism::Manager.Springs.size();
	int Groupsize = (int)floor(Count / NUMBER_OF_GROUPS);

	int I = 0;

	for_loop(B, NUMBER_OF_GROUPS)
	{
		std::vector<Edge*> Temp;
		for_loop(C, Groupsize)
		{
			Temp.push_back(&Organism::Manager.Springs[I]);
			I++;
		}
		Organism::Manager.Groups.push_back(Temp);
	}
	int Remainder = (Count % NUMBER_OF_GROUPS);
	if (Remainder)
	{
		for_loop(R, Remainder)
		{
			Organism::Manager.Groups[NUMBER_OF_GROUPS -1].push_back(&Organism::Manager.Springs[I]);
			I++;
		}
	}
}
OPTIMIZATION_OFF()
void CreatureManager::GroupUpdater(int _group)
{
	while (true)
	{
		for (auto& G : Groups[_group])
		{
			G->Update();
		}
		atomic_region(updategroup)
		{// Aquires and owns Anon Mutex and lock guard.
			//int A = GroupsFinished;
		//	GroupsFinished.compare_exchange_weak(A, GroupsFinished++);
			//std::atomic_fetch_add(&GroupsFinished, 1);
			GroupsFinished++;
		}
	//  std::unique_lock<std::mutex> lk(UpdateLock);
	//	std::atomic_fetch_add(&GroupsFinished, 1);
	//	if (CVready.wait_for(*&std::unique_lock<std::mutex>(UpdateLock), 1000ms, [this] {return GroupsFinished == 0; }));
		while (GroupsFinished._My_val != 0) { UpdaterLocked = true; }
	//  UpdaterLocked = false;
	}
}
void CreatureManager::Scheduler()
{
	while (GroupsFinished < (NUMBER_OF_GROUPS )) {}
	GroupsFinished = 0;
}	
OPTIMIZATION_ON()
int IDD = 0;

void CreatureManager::UpdateOrganisms()
{/// This is a Secondary worker thread that constantly updates the creatures while the rest of the program does its thing.
///while (true)
///{
///	BenchMark Performance;
///	{
///		for_loop(I, Springs.size())
///		{
///			auto submit = [&]() {Springs[I].Update(); };   
///			ThreadPool::Instance().Async((Worker_Function&)submit);
///		}
///		for_loop(I, Cells.size())
///		{
///	//		ThreadPool::Instance().Async(); //std::bind(&Springs[I].Update, Edge);
///		}
///	}
///}
 for_loop(I, NUMBER_OF_GROUPS)
 {
 	Print(I);
 	std::thread UpThread(&CreatureManager::GroupUpdater, this, I);
 	UpThread.detach();
 }
 UpdateLoop = new std::thread
 ([]()
 {
 	BenchMark UpdateBench("Update Creatures");
 	while (true)
 	{
 		BenchMark Performance;
 		{
 			//isWriting = true;
 			//Organism::Manager.UpdateSprings();
 			Organism::Manager.Scheduler();
 			Organism::Manager.UpdateCells();
 		 
 			//isWriting = false;
 		}
 	}
 });
 //UpdateLoop.detach();
}
void CreatureManager::UpdateVertices()
{/// THIS IS CALLED FROM MAIN THREAD
	isReading = true;
	Sphere::BatchVAO.VAO.Bind();
	Sphere::BatchVAO.VAO.Buffers[0]->Bind();
	glBufferData(GL_ARRAY_BUFFER, VertexBuffer.size() * sizeof(Vec3), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, VertexBuffer.size() * sizeof(Vec3), &VertexBuffer[0]);
	isReading = false;
}
void CreatureManager::UpdateSprings()
{/// CALLED IN THE SECOND THREAD UPDATE LOOP
	__debugbreak();/// DEPRECIATED: Not currently in use and may be removed
	for_loop(I, Springs.size())
	{
		Edge *S = &Springs[I];
	}
}
void CreatureManager::Updater(int &_trigger)
{/// DEPRECIATED: Not currently in use and may be removed
	__debugbreak();
	IDD++;
	int TID = IDD;
	while (true)
	{
		while (_trigger == 0) {}
		Springs[_trigger].Update();
		_trigger = 0;
		//	Print("Thread: " << TID << " : Trigger: " << _trigger);
	}
}


//
//void(*_Func)();
//
//class A
//{
//	A()
//	{
//		All.push_back(this);
//	}
//
//	void Update()
//	{ ...Update single Object... }
//
//	static void UpdateAll()
//	{
//		for (int i = 0; i < All.size(); ++i)
//		{
//			ThreadPool.Submit(All[i].Update);//<- Illegal on bound member function
//		}
//	}
//
//	static std::vector<A> All;
//}
//
//



///=======================================================================================================================================
/// Organism Class
///=======================================================================================================================================

Organism::Organism(Mesh *_mesh)
	:
	ID(NumberofCreatures++)
{
	Position = _mesh->Position;
	CellStart = Manager.Cells.size();
	std::unordered_map<GLuint, std::vector<GLuint>> MapVertices;
	std::vector<Vec3> Vertices;

	int IndicesCount = _mesh->VAO.ElementCount / 3;
	int VerticeCount = _mesh->VAO.Buffers[0]->ElementCount;

	///----------------------Deleted at the end of the function-------------------------
	/// Yep it causes a problem if I change the buffer, I tried to add colors to it and boom, it failed DUH/. It was a weird hard to diagnose error as well, I got lucky finding it
	iVec3 *Data = (iVec3*)_mesh->VAO.Buffers[4]->Map(GL_READ_ONLY); /// 4 is the Indices, need to fix this whole fucking Buffer Attributes system
	glUnmapBuffer(GL_ARRAY_BUFFER);

	Vec3 *Verts = (Vec3*)_mesh->VAO.Buffers[0]->Map(GL_READ_ONLY);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	///---------------------------------------------------------------------------------


	//for_loop(I, IndicesCount)
    for(int I = 0; I < IndicesCount; I++)
	{
		MapVertices[Data[I].x].push_back(Data[I].y);
		MapVertices[Data[I].x].push_back(Data[I].z);
        MapVertices[Data[I].y].push_back(Data[I].x);
        MapVertices[Data[I].y].push_back(Data[I].z);
		MapVertices[Data[I].z].push_back(Data[I].y);
		MapVertices[Data[I].z].push_back(Data[I].x);
	}

	for_loop(I, VerticeCount)
	{// Make All The Cells for This Model
		MakeCell(Verts[I] + Position);
	}
	CellEnd = Manager.Cells.size();

	for (int i = CellStart; i < CellEnd; i++)
	{// Get the first and the End of the Cells that make up this Organism from the Cell List
		int OffsetIndex = i - CellStart;
		Manager.Cells[i].EdgeStart = Manager.Springs.size();
		for (auto& E : MapVertices[OffsetIndex])
		{
			Manager.Cells[i].MakeEdge(i, E + CellStart);
		}
		Manager.Cells[i].EdgeEnd = Manager.Springs.size();
	}


	Manager.Creatures.push_back(*this);
}

void Organism::MakeCell(Vec3 _pos)
{
	unsigned int Start = Organism::Manager.Cells.size();
	Organism::Manager.Cells.push_back(Node(ID, _pos));
	unsigned int End = Organism::Manager.Cells.size();
}
void Organism::Update()
{
	for (int i = CellStart; i < CellEnd; i++)
	{
		Organism::Manager.Cells[i].Update();
	}
}


///=======================================================================================================================================
/// Edge Class
///=======================================================================================================================================
Edge::Edge(unsigned int a, unsigned int b, float _tension)
	:
	Parent(a),
	Other(b),
	Tension(_tension),
	Damper(1.f),
	RestLength(0),
	Displacement(0)
{
	RestLength = GetDisplacement(Organism::Manager.Cells[Parent].Position, Organism::Manager.Cells[Other].Position);
}

void Edge::Update()
{
					// READ THIS ASAP http://www.ryanmwright.com/tag/c/										
	Vec3 P1 = Organism::Manager.Cells[Parent].Position;
	Vec3 P2 = Organism::Manager.Cells[Other].Position;

	Vec3 CurrentDisplacement = GetDisplacement(P1,P2) - RestLength;
	float Dist = Distance(P1, P2);

	constexpr float K = .1f;
	Organism::Manager.Cells[Parent].Force += -(K * CurrentDisplacement) - (Damper * Organism::Manager.Cells[Parent].Velocity);
	Organism::Manager.Cells[Other].Force  += -(K * -CurrentDisplacement) - (Damper * Organism::Manager.Cells[Other].Velocity);
}
Node *GetCell(unsigned int _index)
{
	return  &Organism::Manager.Cells[_index];
}


///=======================================================================================================================================
/// Node Class
///=======================================================================================================================================

Node::Node(unsigned int _parent, Vec3 _pos)
	:
	Position(_pos),
	Parent(_parent),
	Velocity(0),
	Force(0),
	Acceleration(0),
	EdgeEnd(0),
	EdgeStart(0)
{
	VertexIndex = Organism::Manager.Vertis.size();
	EdgeStart = Organism::Manager.Springs.size();
	Organism::Manager.Vertis.push_back(_pos);
	Organism::Manager.VertexBuffer.push_back(_pos);
}

void Node::Update()
{
	__debugbreak();
	for (int i = EdgeStart; i < EdgeEnd; i++)
	{
		Organism::Manager.Springs[i].Update();
	}
	Velocity.x = RANDOM_SPREAD(.010);
	Velocity.y = RANDOM_SPREAD(.010);
	Velocity.z = RANDOM_SPREAD(.010);

	float Timestep = .1f;
	Velocity = Velocity * .95f;// Friction velocity loss is .95 or 95% of velocity is retained

	Vec3  Last_Acceleration = Acceleration;
	Position += Velocity * Timestep + (Last_Acceleration * 0.5f * Squared(Timestep));
	Acceleration = Force / 10.0f;// Mass = 10;

	Force = Vec3(0.0f);

	Vec3 Avg_Acceleration = (Last_Acceleration + Acceleration) *.5f; // 2.0f;
	Velocity += Avg_Acceleration * Timestep;
	Organism::Manager.VertexBuffer[VertexIndex] = Position;
}
void Node::MakeEdge(unsigned int a, unsigned int b)
{
	Organism::Manager.Springs.push_back(Edge(a, b, RANDOM(.5)));
	EdgeEnd = Organism::Manager.Springs.size();
}


#endif






