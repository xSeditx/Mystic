#ifndef _COLLIDER_H
#define _COLLIDER_H
#pragma once


/*
Mostly going to be ripped out and replaced with a Collider/ Physics engine I have already written but 
at the time I had figured I would have been better off using BulletPhysics yet I never followed through
with fully implementing it because I am not sure it will be able to perform at my specifications so for
now this section lays here like a Zombie till I decide what to do with it.

*/


// Spring Physics
// https://gafferongames.com/post/spring_physics/

#pragma comment(lib ,"Bullet3Common_Debug.lib")
#pragma comment(lib ,"BulletDynamics_Debug.lib")
#pragma comment(lib ,"LinearMath_Debug.lib")
#pragma comment(lib ,"BulletCollision_Debug.lib")

//#include"btBulletDynamicsCommon.h"

#include"Gameobject.h"
#include"Common.h"
#include<algorithm>

#include"Shader.h"
class Mesh;


class CollisionSphere;
class AABBCollider;

class Sphere; // Primative.h
class Shader;
 


class Mass
{
public:
	Mass();
	Mass(float weight, Vec3 pos);
	Mass(float weight, float x, float y, float z);

	float Surface_Friction;
	float Weight;

	Vec3
		Velocity,
		Position,
		Rotation,
		Acceleration,
		Angular,
		Force;

	void Apply_Gravity();
	void Apply_Physics();
	void Set_Force(float x, float y, float z);
	void Set_Position(float x, float y, float z);

	Vec3 Get_Position();

	inline float Get_X() { return Position.x; }
	inline float Get_Y() { return Position.y; }
	inline float Get_Z() { return Position.z; }

	struct convert
	{
		inline float Pounds_to_Kg(float lb) { return lb * .453; }
		inline float Newtons_to_Kg(float N) { return N / 9.8; }
		inline float Kg_to_Newtons(float Kg) { return Kg * 9.8; }
	}Convert;
	struct volume
	{
		inline float ofBox(Vec3 min, Vec3 max) { return (max.x - min.x) * (max.y - min.y) * (max.z - min.z); }
		inline float ofSphere(float radius) { return (float)(4 / 3)*(3.1415926f * (radius*radius*radius)); }
	}Volume;

	static void  Set_Timestep(float time) { Current_Timestep = time; }
	static void  Set_Gravity(float g) { Gravity = g; }

	static float Get_Timestep() { return Current_Timestep; }
	static float Get_Gravity() { return Gravity; }

	void Update(float _dt)
	{
		Force.y += Weight * 9.80665;
		Velocity = Velocity * Surface_Friction;

		Vec3  Last_Acceleration = Acceleration;
		Position += Velocity * _dt + (Last_Acceleration * 0.5f * Squared(_dt));
		Acceleration = Force / Weight;

		Force = Vec3(0.0f);

		Vec3  Avg_Acceleration = (Last_Acceleration + Acceleration) / 2.0f;
		Velocity += Avg_Acceleration * _dt;
	}
	void Update()
	{// Incase global Timestep has been set, use that
		Update(Mass::Get_Timestep());
	}

private:

	static float Current_Timestep;
	static float Gravity;
};



class Collider : public GameObject
{
public:
	Collider(int _type, Mesh* _object, Vec3 _position);

	virtual bool isCollision(Collider *other) = pure_virtual;
	virtual bool isCollision(CollisionSphere *other) = pure_virtual;
	virtual bool isCollision(AABBCollider *other) = pure_virtual;

	enum Collider_t 
	{
		SPHERE,
		BOX,
		MESH,
		TERRAIN,
		POINT, 
		RAY
	};

	inline int GetType() const { return Type; }
	static Collider* CreateColliderFromMesh(Collider_t _type, Mesh *_mesh);


	void Bind() {} // To remove these from the API because the End user does not need them
	void Bind(Shader &_shader) {} // Only here due to the virtual functionality of GameObject
	void Unbind() {}
	void Update() {}
	void Render(Shader &_shader) {}

	Mass Body;
	GameObject *_object_ptr;// Lock this down private with a getter later. All gets and sets need to be explicitly intentional
	Mesh *DebugMesh;

protected:
	//Mass(float weight, Vec3 pos);
	//Mass(float weight, float x, float y, float z);

	Shader& GetRenderer()
	{
		static Shader *ColliderRenderer = new Shader("Resources\\Collider.sfx");
		return *ColliderRenderer;
	}
	Collider_t Type;
private:
	
};

class CollisionSphere: public Collider
{
public:
	CollisionSphere(Mesh* _obj, Vec3 _pos, float _radius);

	bool isCollision(Collider *other) override;
	bool isCollision(AABBCollider *other);
	bool isCollision(CollisionSphere *other) override;

	void Update();
	void Render();

	float Radius;
private:

	void Bind() {} // To remove these from the API because the End user does not need them
	void Bind(Shader &s) {} // Only here due to the virtual functionality of GameObject
	void Unbind() {}
};

//XYZ = LWH
class AABBCollider : public Collider
{
public:
	AABBCollider(Mesh* _obj, Vec3 _pos, Vec3 _min, Vec3 _max);

	bool isCollision(Collider *other)        override;
	bool isCollision(AABBCollider *other)    override;
	bool isCollision(CollisionSphere *other) override;

	void Update() 
	{
		Body.Update();
		Position = Body.Position;
		Rotation = Body.Rotation;
		Min = Position - Dimensions;
		Max = Position + Dimensions;
	}
	void Render();

//	float Length, Width, Height;
	Vec3 Dimensions;

	bool Intersect(AABBCollider a, AABBCollider b)
	{
		return 
			(
			(std::min(a.Min.x, a.Max.x) <= std::max(b.Min.x, b.Max.x) && std::max(a.Min.x, a.Max.x) >= std::min(b.Min.x, b.Max.x)) &&
			(std::min(a.Min.y, a.Max.y) <= std::max(b.Min.y, b.Max.y) && std::max(a.Min.y, a.Max.y) >= std::min(b.Min.y, b.Max.y)) &&
			(std::min(a.Min.z, a.Max.z) <= std::max(b.Min.z, b.Max.z) && std::max(a.Min.z, a.Max.z) >= std::min(b.Min.z, b.Max.z))
			);
	}
	bool PointInsideAABB(Vec3 point)
	{
		return
			(point.x >= std::min(Min.x , Max.x ) && point.x <= std::max(Min.x , Max.x )) &&
			(point.y >= std::min(Min.y , Max.y ) && point.y <= std::max(Min.y , Max.y )) &&
			(point.z >= std::min(Min.z , Max.z ) && point.z <= std::max(Min.z , Max.z ));
	}

	Vec3 Min;
	Vec3 Max;
private:
	Mesh *DebugMesh;


	void Bind() {} // To remove these from the API because the End user does not need them
	void Bind(Shader &s) {} // Only here due to the virtual functionality of GameObject
	void Unbind() {}
};


#include<thread>
#include"Application.h"
#include"Renderer.h"


class Simulation
{
	Simulation() = default;

public:
	struct CollisionPair
	{
		CollisionPair(Collider& m1, Collider& m2) : M1(&m1), M2(&m2) {}
		Collider *M1;
		Collider *M2;
	};


	bool Running = true;
	std::thread *ProcessingThread;
	std::vector<Collider*> Objects;
	std::vector<CollisionPair> CollisionPairs;



	void Add(Collider& _object);
	void Start(Scene *world);
	void Run();
	void Resolve();
	void End();


	Simulation(Simulation& other) = delete;
	Simulation& operator=(Simulation& other) = delete;

	static Simulation& Instance();
	CollisionSphere& CreateCollisionSphere(Mesh* _data);
	AABBCollider& CreateAABBCollider(Mesh* _data);
};

#endif //_COLLIDER_H