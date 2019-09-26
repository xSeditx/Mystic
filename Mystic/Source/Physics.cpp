
#include"Physics.h"
#include"Mesh.h"
#include"Primitive.h"
#include"Shader.h"
#include"Camera.h"

// Set the default for the Static variables
float Mass::Current_Timestep = 2.0f;
float Mass::Gravity = 0.1f;



Simulation& Simulation::Instance()
{
	static Simulation _instance;
	return _instance;
}
void Simulation::Add(Collider& _object)
{
	Objects.push_back(&_object);
}
void Simulation::Start(Scene *world)
{
	for (auto &M : world->Meshes)
	{
		Objects.push_back(Collider::CreateColliderFromMesh(Collider::BOX, &M));
	}

	ProcessingThread = new std::thread(&Simulation::Run, this);
}
void Simulation::End()
{
	Running = false;
	if( ProcessingThread != nullptr && ProcessingThread->joinable())
	{
		ProcessingThread->join();
	}
	Objects.clear();
}
void Simulation::Run()
{

	while (Running)
	{
		int Size = Objects.size();

		for_loop(O, Size)
		{
			for_loop(I, Size)
			{
				if (O == I)
				{
					continue;
				}
				if (Objects[O]->isCollision(Objects[I]))
				{
					CollisionPairs.push_back(CollisionPair(*Objects[O], *Objects[I]));
				}
			}
		}

		Running = Window::ActiveWindow()->Alive();
		Resolve();
	}
}
void Simulation::Resolve()
{

	for (auto& P : CollisionPairs)
	{
		
		P.M1->Body.Force += glm::normalize(P.M1->Body.Position - P.M2->Body.Position);
		//P.M2->Body.Force += Vec3(-10);

		P.M1->Body.Update();
	//	P.M2->Body.Update();

		P.M1->Position = P.M1->Body.Position;
	//	P.M2->Position = P.M2->Body.Position;
		
    P.M1->_object_ptr->Transform = glm::mat4(1.0f); // Set Identity and Rotate all axis followed with the Translation.
    P.M1->_object_ptr->Transform = glm::rotate(P.M1->Transform, glm::radians(P.M1->Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
    P.M1->_object_ptr->Transform = glm::rotate(P.M1->Transform, glm::radians(P.M1->Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
    P.M1->_object_ptr->Transform = glm::rotate(P.M1->Transform, glm::radians(P.M1->Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
    P.M1->_object_ptr->Transform = glm::translate(P.M1->_object_ptr->Transform, P.M1->Position);
	//P.M1->DebugMesh->Transform = P.M1->_object_ptr->Transform;
//
//	P.M2->_object_ptr->Transform = glm::mat4(1.0f); // Set Identity and Rotate all axis followed with the Translation.
//	P.M2->_object_ptr->Transform = glm::rotate(P.M1->Transform, glm::radians(P.M1->Rotation.x), Vec3(1.0f, 0.0f, 0.0f));
//	P.M2->_object_ptr->Transform = glm::rotate(P.M1->Transform, glm::radians(P.M1->Rotation.y), Vec3(0.0f, 1.0f, 0.0f));
//	P.M2->_object_ptr->Transform = glm::rotate(P.M1->Transform, glm::radians(P.M1->Rotation.z), Vec3(0.0f, 0.0f, 1.0f));
//	P.M2->_object_ptr->Transform = glm::translate(P.M1->Transform, P.M1->Position);
    }
	CollisionPairs.clear();
}

///=============================================================================================================================================
///=============================================================================================================================================
///=============================================================================================================================================


Mass::Mass() 
	:
	Force(0, 0, 0),
	Acceleration(0, 0, 0),
	Velocity(0, 0, 0),
	Weight(0),
    Surface_Friction(0)
{
}
Mass::Mass(float weight, Vec3 pos)
	:
	Force(0, 0, 0),
	Acceleration(0, 0, 0),
	Velocity(0, 0, 0),
	Weight(weight),
	Angular(0,0,0),
    Surface_Friction(0)
{
	Position = pos;
}
Mass::Mass(float weight, float x, float y, float z)
{
	Mass(weight, Vec3(x, y, z));
}
void Mass::Apply_Gravity() {}
void Mass::Apply_Physics() {}
void Mass::Set_Force(float x, float y, float z)
{
	Force = Vec3(x, y, z);
}
void Mass::Set_Position(float x, float y, float z)
{
	Position = Vec3(x, y, z);
}
Vec3 Mass::Get_Position()
{
	return Position;
}



///=============================================================================================================================================
///=============================================================================================================================================
///=============================================================================================================================================

Collider::Collider(int _type, Mesh* _object, Vec3 _position)
	:
	GameObject(_position, Vec3(0), Vec3(1)),
	Body(10.0, _position),
	Type((Collider_t)_type),
	_object_ptr(_object)
{}
Collider* Collider::CreateColliderFromMesh(Collider_t _type, Mesh *_mesh)
{
	Collider *results = nullptr;

	switch (_type)
	{
	case SPHERE:
		results = &Simulation::Instance().CreateCollisionSphere(_mesh);
		break;
	case BOX:
		results = &Simulation::Instance().CreateAABBCollider(_mesh);
		break;
	};
	results->Type = _type;// Be really fucking careful and also remove this in the future. 
	return results;
}

CollisionSphere& Simulation::CreateCollisionSphere(Mesh* _mesh)
{
	int VerticesIndex = -1;
	{// Find the Location of the Vertex Buffer in the VAO of Mesh 
		for_loop(I, _mesh->VAO->Buffers.size())
		{
			if (_mesh->VAO->Buffers[0]->AttributeType == VERTEX)
			{
				VerticesIndex = I;
				break;
			}
		}
		if (VerticesIndex == -1)
		{
			Print("Error: Vertex Buffer not found. CollisionSphere unable to be generated.");
			__debugbreak();
		}
	}
	Vec3 *Verts = (Vec3*)_mesh->VAO->Buffers[VerticesIndex]->Map(GL_READ_ONLY);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	Vec3 Average = Vec3(0);
	Vec3 Position = Vec3(0);

	float SphereRadius = 0;
	{ // Finds the Vertex Furthest from the Models Center 

		int VerticeCount = _mesh->VAO->Buffers[VerticesIndex]->ElementCount;

		Vec3 *Verts = (Vec3*)_mesh->VAO->Buffers[0]->Map(GL_READ_ONLY);
		glUnmapBuffer(GL_ARRAY_BUFFER);

		///---------------------------------------------------------------------------------


		for_loop(I, VerticeCount)
		{
			Average += Verts[I];
		}

		Position = Vec3(Average.x / VerticeCount, Average.y / VerticeCount, Average.z / VerticeCount);

		for_loop(I, VerticeCount)
		{
			float X = Verts[I].x;
			float Y = Verts[I].y;
			float Z = Verts[I].z;
			float Distance = std::sqrt
			(
				Squared(X - Position.x) +
				Squared(Y - Position.y) +
				Squared(Z - Position.z)
			);
			if (Distance > SphereRadius)
			{
				SphereRadius = Distance;
			}

		}
	}

	CollisionSphere *results = new CollisionSphere(_mesh, Position, SphereRadius);
	return *results;
}
AABBCollider& Simulation::CreateAABBCollider(Mesh* _mesh)
{
	Vec3 Min(0xFFFFFF), Max(-0xFFFFFF);
	float AvgX = 0, AvgY = 0, AvgZ = 0;
	Vec3 Average = Vec3(0);
	Vec3 Position = Vec3(0);

	int VerticesIndex = -1;
	{// Find the Location of the Vertex Buffer in the VAO of Mesh 
		for_loop(I, _mesh->VAO->Buffers.size())
		{
			if (_mesh->VAO->Buffers[0]->AttributeType == VERTEX)
			{
				VerticesIndex = I;
				break;
			}
		}
		if (VerticesIndex == -1)
		{
			Print("Error: Vertex Buffer not found. CollisionSphere unable to be generated.");
			__debugbreak();
		}
	}
	{ // Finds the Length of each axis from the Models Center 

		int VerticeCount = _mesh->VAO->Buffers[VerticesIndex]->ElementCount;

		Vec3 *Verts = (Vec3*)_mesh->VAO->Buffers[0]->Map(GL_READ_ONLY);
		glUnmapBuffer(GL_ARRAY_BUFFER);

		///---------------------------------------------------------------------------------
		for_loop(I, VerticeCount)
		{
			Average += Verts[I];
		}
		Position = Vec3(Average.x / VerticeCount, Average.y / VerticeCount, Average.z / VerticeCount);


		for_loop(I, VerticeCount)
		{
			float X = Verts[I].x;
			float Y = Verts[I].y;
			float Z = Verts[I].z;
			if (X < Min.x) Min.x = X;
			if (X > Max.x) Max.x = X;

			if (Y < Min.y) Min.y = Y;
			if (Y > Max.y) Max.y = Y;

			if (Z < Min.z) Min.z = Z;
			if (Z > Max.z) Max.z = Z;
		}
	}

	//Vec3 Position = (Min + Max) * .5f;// Vec3(AvgX, AvgY, AvgZ);
	AABBCollider *results = new AABBCollider(_mesh, Position, Min, Max);
	return *results;
}

///=============================================================================================================================================
///=============================================================================================================================================

CollisionSphere::CollisionSphere(Mesh* _obj, Vec3 _pos, float _radius)
	:
	Collider(SPHERE, _obj, _pos),
	Radius(_radius)
{
	DebugMesh = new Sphere(_pos, _radius, 3);
}
void CollisionSphere::Update()
{// Manage All the Physics for the Collision Object
}
void CollisionSphere::Render() 
{/// This will be where the Collision Object is rendered, generally in Wireframe mode in order to accurately debug the object

	GetRenderer().Enable();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	GetRenderer().SetUniform("ProjectionMatrix", Viewport::Camera->ProjectionMatrix);
	GetRenderer().SetUniform("ViewMatrix", Viewport::Camera->ViewMatrix);
	DebugMesh->Render(GetRenderer());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GetRenderer().Disable();
}

bool CollisionSphere::isCollision(Collider *other) { return false; }
bool CollisionSphere::isCollision(AABBCollider *other) { return false; }
bool CollisionSphere::isCollision(CollisionSphere *other) { return false; }

///=============================================================================================================================================
///=============================================================================================================================================

AABBCollider::AABBCollider(Mesh* _obj, Vec3 _pos, Vec3 _min, Vec3 _max)
	:
	Collider(BOX, _obj, _pos),
	Min(_min),
	Max(_max)
{
	Dimensions.x = (_max.x - _min.x),
	Dimensions.y = (_max.y - _min.y),
	Dimensions.z = (_max.z - _min.z);
	/// Position is Correct here.... Has to be in Box
	DebugMesh = new Box(_pos, Dimensions.x, Dimensions.y, Dimensions.z);
	Dimensions *= .5;
}

void AABBCollider::Render()
{
	GetRenderer().Enable();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	GetRenderer().SetUniform("ProjectionMatrix", Viewport::Camera->ProjectionMatrix);
	GetRenderer().SetUniform("ViewMatrix", Viewport::Camera->ViewMatrix);
	DebugMesh->Render(GetRenderer());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GetRenderer().Disable();
}
bool AABBCollider::isCollision(Collider *other) { return other->isCollision(this); }
bool AABBCollider::isCollision(AABBCollider *other)
{
	return
		(std::min(Min.x, Max.x) <= std::max(other->Min.x, other->Max.x) && std::max(Min.x, Max.x) >= std::min(other->Min.x, other->Max.x)) &&
		(std::min(Min.y, Max.y) <= std::max(other->Min.y, other->Max.y) && std::max(Min.y, Max.y) >= std::min(other->Min.y, other->Max.y)) &&
		(std::min(Min.z, Max.z) <= std::max(other->Min.z, other->Max.z) && std::max(Min.z, Max.z) >= std::min(other->Min.z, other->Max.z));
}
bool AABBCollider::isCollision(CollisionSphere *other) { return false; }
///=============================================================================================================================================



