#pragma once
#include<vector>
#include<string>
#include<unordered_map>

#include"Common.h"



/*
Death to this Useful abomination. I used it in a previous project with great succes however
my attempts to implement it here have been filled with problems mainly due to the fact that
I attempted to make all my objects derive from the asset class if it needed the functionality
instead of just composing the objects and adding a static manager to any class viewed as an Asset. 
*/
template<typename T>
class Asset
{
public:
	Asset() = default;
	~Asset()
	{
//		AssetMap.clear();
	//	Manager.clear();
	}

	Asset(std::string file)
		:
		Name("Asset" + std::to_string(ID)),
		Filepath(file),
		ID(ElementCount++), 
		Complete(false)
	{
	}

	Asset(std::string name, std::string file)
		:
		Name(name),
		Filepath(file),
		ID(ElementCount++),
		Complete(true)
	{
	}
	Asset(std::string name, std::string file, T* _object)
		:
		Name(name),
		Filepath(file),
		ID(ElementCount++),
		Complete(true)
	{
		Add(_object);
	}

	TagID ID = 0; // Change the order of these two 
	std::string Name = ""; // And shit can break/// Fucking broke anyway who really cares now bitch
    std::string Filepath = "";
	bool Complete = false;

    static std::vector<T*> Manager;
    static std::unordered_map<std::string, TagID> AssetMap;
   
  //  virtual bool Load() = pure_virtual;

	void NameIt(std::string _name)
	{
		Name = _name;
	}

	void Add(T* _object)
	{
		Manager.push_back(_object);
		AssetMap.insert({ Name, _object->ID }); //		AssetMap[Name] = _object->ID;
	}
	static T* Get(std::string _name)
	{
		return Manager[AssetMap[_name]];
	}

	static TagID ElementCount;
}; 

template<typename T>    std::vector<T*> Asset<T>::Manager;
template<typename T>    TagID Asset<T>::ElementCount = 0;
template<typename T>    std::unordered_map<std::string, TagID> Asset<T>::AssetMap;


///============================================================================================
/// SORT OF WORKED, DID RETURN THE BASE ASSET CLASS POINTER BUT FAILED TO RETURN THE OBJECT
///============================================================================================
//  static std::vector<Asset<T>> Manager;
/// T* Get()
/// { 
/// 	return *Manager[ID];
/// }
/// static T* Get(std::string _name);
/// static T* Get(TagID _id);
/// template<typename T>
/// T* Asset<T>::Get(std::string name)
/// {
/// 	
/// 	return (T*)(&Manager[AssetMap[name]]);
/// }
/// template<typename T> T* Asset<T>::Get(TagID _id)
/// {
/// 	return (T*)(&Manager[_id]);
/// 
/// }
/// template<typename T>
/// std::vector<Asset<T>> Asset<T>::Manager;
///============================================================================================

template<typename T>
class AssetManager
{
public:
	AssetManager(){}

	void push_back(T* _object, std::string _name)
	{
		ObjectID = Objects.size();
		Objects.push_back(_object);
		ObjectMap[_name] = ObjectID;
	}

	T& GetAsset(std::string _name)
	{
		return Objects[ObjectMap[_name]];
	}

	static void LoadAsset(std::string _name)
	{
 	}

	TagID ObjectID; 
	static std::vector<T> Objects;
	static std::unordered_map<std::string, TagID> ObjectMap;
	static TagID ObjectCount;
};

template<typename T> TagID AssetManager<T>::ObjectCount = 0;
template<typename T> std::vector<T> AssetManager<T>::Objects;
template<typename T> std::unordered_map<std::string, TagID> AssetManager<T>::ObjectMap;

/* 
             THEORY
           ==========

		   Any Asset is Subclass of Asset,
		   If name not given one Generated.
		   All Types stored in Vector of Objects T
	       ID also Generated as Index to Vector
		   Recoverable Via name or ID


		   Image:Asset<Image>
		   All Images are Assets.
		   Assets want a Image::Manager
*/
//(std::find(vec.begin(), vec.end(), item) != vec.end()

// 












//
//
//template<typename T>
//class Asset
//{
//public:
//	Asset(std::string name, std::string file, T* _object)
//		:
//		Name(name),
//		Filepath(file),
//		ID(ElementCount++),
//		Complete(true)
//	{
//		Add(_object);
//	}
//
//	TagID ID = 0;
//	std::string Name = ""; 
//	std::string Filepath = "";
//	bool Complete = false;
//
//	static std::vector<T*> Manager;
//	static std::unordered_map<std::string, TagID> AssetMap;
//
//	void Add(T* _object)
//	{
//		Manager.push_back(_object);
//		AssetMap.insert({ Name, _object->ID }); //		AssetMap[Name] = _object->ID;
//	}
//
//	static T* Get(std::string _name)
//	{
//		return Manager[AssetMap[_name]];
//	}
//
//	void NameIt(std::string _name)
//	{
//		Name = _name;
//	}
//	static TagID ElementCount;
