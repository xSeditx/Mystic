#pragma once
#pragma message("Start of ECS.h")

#include<vector>
#include<utility>
#include<unordered_map>

#include"Common.h"

#define NULL_ENTITY nullptr
#define COMPONENT(x) struct x : Component<x>

// TODO: 
///Fix All these Typedefs to more accurately reflect what they accomplish in the ECS as a whole

using Component_Handle = uint32_t;
struct BaseComponent;


//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef void *EntityPTR;
using Entity_ID = unsigned int; /// Fix this shit where do I need it at Maybe just put it all in Common and forget about it

typedef Component_Handle(*ComponentCreateFunc)(std::vector<uint8_t>&memory, EntityPTR entity, BaseComponent *comp);
typedef void(*ComponentDeleteFunc)(BaseComponent* comp);


typedef std::vector<std::pair<unsigned int, Entity_ID>> EntityHandle_t; // First is Index of the Pair in the Vector second is the Entities ID into the Component Entities
typedef std::pair<Entity_ID, EntityHandle_t> Entity_t;/// A pair that is the Entities ID and the Entities Structure which is a vector of pairs of unsigned ints
typedef std::unordered_map <Component_Handle, std::vector<uint8_t>> Component_map; /// A Map that has the Component ID as the key and a Vector of bytes for the Data 
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------


struct BaseComponent
{
private:


public:

    struct Properties
    {/// Might Make this Public| It neededs to be if I wish to debug the values.....
        Properties
        (
            ComponentCreateFunc _createf,
            ComponentDeleteFunc _deletef,
            size_t _sz
        )
            :
            Create_Function(_createf),
            Delete_Function(_deletef),
            SIZE(_sz)
        {
#ifdef _DEBUG
            ComponentName = (typeid(this).name());
#endif
        }
        ComponentCreateFunc Create_Function;
        ComponentDeleteFunc Delete_Function;
        size_t SIZE;

        std::string ComponentName;
        int DebugTypeID = 0;
    };



    /// --------------- FINISHED ----------------------------
    static TagID RegisterComponentType(ComponentCreateFunc _createfunc, ComponentDeleteFunc _deletefunc, size_t _size);

    static ComponentCreateFunc g_TypeCreateFunction(Component_Handle _id)
    {
        return ComponentTypes[_id].Create_Function;
    }
    static ComponentDeleteFunc g_TypeDeleteFunction(Component_Handle _id)
    {
        return ComponentTypes[_id].Delete_Function;
    }
    static size_t g_TypeSize(Component_Handle _id)
    {
        return ComponentTypes[_id].SIZE;
    }

    inline static bool is_type_valid(Component_Handle _id)
    {
        return _id < ComponentTypes.size();
    }
    ///-----------------------------------------------

    static std::vector<Properties> ComponentTypes;
    EntityPTR Entity = NULL_ENTITY;


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



//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

template<typename _C_Ty>
unsigned int CreateComponent(std::vector<uint8_t>& memory, EntityPTR _entity, BaseComponent *comp)
{
    unsigned int Index = memory.size();
    memory.resize(Index + _C_Ty::SIZE);

    _C_Ty *component = new (&memory[Index])_C_Ty(*(_C_Ty*)comp);

    component->Entity = _entity;
    return Index;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

template<typename _C_Ty>
void DeleteComponent(BaseComponent* _comp)
{
    _C_Ty* component = (_C_Ty*)_comp;
    component->~_C_Ty();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
const TagID Component<T>::ID(BaseComponent::RegisterComponentType(CreateComponent<T>, DeleteComponent<T>, sizeof(T)));
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
const size_t Component<T>::SIZE(sizeof(T));
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
const ComponentCreateFunc Component<T>::Create_Function(CreateComponent<T>);
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
const ComponentDeleteFunc Component<T>::Delete_Function(DeleteComponent<T>);
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------- 




