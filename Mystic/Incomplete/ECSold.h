#pragma once
/*
#include<vector>
#include<unordered_map>
#include<utility>
 
#include"Common.h"

struct BaseComponent; // Forward Declaration
typedef void *EntityHandle;

typedef TagID(*ComponentCreateFunc)(std::vector<uint8_t>&memory, EntityHandle entity, BaseComponent *comp);
typedef void(*ComponentDeleteFunc)(BaseComponent* comp);

using EntityHandle_t = std::vector<std::pair<unsigned int, unsigned int>>;
using Entity_t = std::pair<TagID, EntityHandle_t>;


#define NULL_ENTITY nullptr

//-----------------------------------------------------------------------------------
// NOTE: I Dislike Tuple. A struct called Handlers or something, an exact copy of Component could have a vector 
// of said type in base component, Component could be a templated wrapper to the type
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
struct BaseComponent
{
private:
	struct Properties
	{/// Might Make this Public
		Properties
		(
			ComponentCreateFunc _cf,
			ComponentDeleteFunc _df,
			size_t _sz
		)
			:
			Create_Function(_cf),
			Delete_Function(_df),
			SIZE(_sz)
		{}
		ComponentCreateFunc Create_Function;
		ComponentDeleteFunc Delete_Function;
		size_t SIZE;
	};

public:
	static TagID RegisterComponentType(ComponentCreateFunc _createfunc, ComponentDeleteFunc _deletefunc, size_t _size)
	{
		ComponentTypes.push_back({ _createfunc ,_deletefunc ,_size });
	}
	static ComponentCreateFunc getTypeCreateFunction(TagID _id)
	{
		return ComponentTypes[_id].Create_Function;
	}
	static ComponentDeleteFunc getTypeDeleteFunction(TagID _id)
	{
		return ComponentTypes[_id].Delete_Function;
	}
	static size_t getTypeSize(TagID _id)
	{
		return ComponentTypes[_id].SIZE;
	}

	static std::vector<Properties> ComponentTypes;
	EntityHandle Entity = NULL_ENTITY;
};

//-----------------------------------------------------------------------------------

template<typename T>
struct Component : public BaseComponent
{
	static const ComponentCreateFunc Create_Function;
	static const ComponentDeleteFunc Delete_Function;
	static const TagID ID;
	static const size_t SIZE;
};

// Component
// Raw data

// Entity
// List of Components or ID to that specific component

// System
// takes the information and processes the data, likely via a memory manager


class BaseSystem
{
public:
	BaseSystem(std::vector<unsigned int>& comptypeIn)
		:
		ComponentTypes(comptypeIn)
	{}

	std::vector<TagID>& GetComponentTypes()
	{// I might just directly access without a getter, I hate Getters
		return ComponentTypes;
	}

	virtual void UpdateComponents(float _delta, BaseComponent **comp)
	{
	}

	const std::vector<TagID>& GetTypes()
	{
		return ComponentTypes;
	}
private:
	std::vector<TagID> ComponentTypes;
};

class TestSystem : public BaseSystem
{
	TestSystem(std::vector<TagID>& comptypeIn)
		:BaseSystem(comptypeIn)
	{}

	void UpdateComponents(float _delta, BaseComponent **comp)
	{
		
	}
};

typedef unsigned int Component_t;
//-----------------------------------------------------------------------------------
class EntityComponentSystem
{
public:
	 EntityComponentSystem();
	~EntityComponentSystem();

	//Entity Methods
	void RemoveEntity(EntityHandle _handle);
	// Components
	void AddComponent(EntityHandle _ent);
	void RemoveComponent(EntityHandle _ent);

	template<class _component>
	void GetComponent(EntityHandle _ent);


	//System Methods
	void AddSystem(BaseSystem &_sys)
	{
		Systems.push_back(&_sys);/// HMMMM 
	}
//https://www.youtube.com/watch?v=4-LRor0UG6o&list=PLEETnX-uPtBUrfzE3Dxy3PWyApnW6YEMm&index=7
	void UpdateSystem(float _delta)
	{}
	static unsigned int EntityCount;
private:
	bool RemoveComponentInternal(EntityHandle _handle, TagID _componentID);
	std::vector<BaseSystem*> Systems;
	std::unordered_map<uint32_t, std::vector<uint8_t>> Components;
	
    // Vector of TagID and Vector of Pairs of TagID and TagID
	std::vector<TagID, 
		std::vector< // Index of Pair in this Vector, so element 1 will have 1 here etc..
		std::pair<Component_t, unsigned int>>* // Pointer to the first pair
		>Entities;

	Entity_t* RawTypeHandle(EntityHandle _handle)
	{
		(Entity_t*)_handle;
	}
	unsigned int HandleToEntityIndex(EntityHandle _handle)
	{
		return RawTypeHandle(_handle)->first;
	}
	EntityHandle_t& HandleToEntity(EntityHandle _handle)
	{
		return RawTypeHandle(_handle)->second;
	}

	EntityHandle MakeEntity(BaseComponent *_entitycomp, const TagID* _componentIDs, size_t numComponents)
	{
		Entity_t* NewEntity = new Entity_t(); 
	}
};

using EntityHandle_t = std::vector<std::pair<unsigned int, unsigned int>>;

//-----------------------------------------------------------------------------------

template<typename T>
unsigned int CreateComponent(std::vector<uint8_t>& memory, EntityHandle entity, BaseComponent *comp)
{
	unsigned int Index = memory.size();
	memory.resize(Index + T::SIZE);

	T *component = new (&memory[Index])T(*(T*)comp);
	component->Entity();
}
//-----------------------------------------------------------------------------------

template<typename T>
void DeleteComponent(BaseComponent* comp)
{
	T* component = (T*)comp;
	component->~Component();
}


















///HMMMMM  I THINK I AM GETTING THERE UNDERSTANDING THIS. THIS IS WHAT I HAVE CREATED SO FAR
///
///typedef void(*CreateComp)();
///typedef void(*DeleteComp)();
///
///template <typename T>
///class Component
///{
///	Component()
///	{
///
///	}
///	CreateComp Create();
///	DeleteComp Delete();
///
///	virtual ~Component() = pure_virtual;
///	void AddComponent(T* _component);
///	static std::vector<T> List;
///	static TagID ID;
///};
///
///
///class System
///{
///	virtual void Update() = pure_virtual;
///	std::vector<Component *> ComponentList;
///	static TagID ID;
///};


*/