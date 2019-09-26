#pragma once
#include<vector>
#include<utility>
#include<unordered_map>

#include"Common.h"

#include"ECScomponent.h"
#include"ECSsystem.h"

#define NULL_ENTITY     nullptr

/// Component
// Raw data

/// Entity
// List of Components or ID to that specific component

/// System
// takes the information and processes the data, likely via a memory manager
using Entity_ID = unsigned int;


class ECSListener
{
public:
    virtual void onMakeEntity(EntityPTR handle) {}
    virtual void onRemoveEntity(EntityPTR handle) {}
    virtual void onAddComponent(EntityPTR handle, Component_Handle id) {}
    virtual void onRemoveComponent(EntityPTR handle, Component_Handle id) {}

    const std::vector<Component_Handle>& g_ComponentIDs()
    {
        return ComponentIDs;
    }

    inline bool shouldNotifyOnAllComponentOperations()
    {
        return notifyOnAllComponentOperations;
    }
    inline bool shouldNotifyOnAllEntityOperations()
    {
        return notifyOnAllEntityOperations;
    }

protected:
    void setNotificationSettings( bool shouldNotifyOnAllComponentOperations, bool shouldNotifyOnAllEntityOperations)
    {
        notifyOnAllComponentOperations = shouldNotifyOnAllComponentOperations;
        notifyOnAllEntityOperations = shouldNotifyOnAllEntityOperations;
    }
    void addComponentID(Component_Handle id) {
        ComponentIDs.push_back(id);
    }
private:
    std::vector<Component_Handle> ComponentIDs;
    bool notifyOnAllComponentOperations = false;
    bool notifyOnAllEntityOperations = false;
};


//-----------------------------------------------------------------------------------
class EntityComponentSystem
{
public:
    NO_COPY_OR_ASSIGNMENT(EntityComponentSystem);

    EntityComponentSystem() = default;
	~EntityComponentSystem();


    // Component Methods
    template<class T_component> inline void AddComponent(EntityPTR _entity, T_component* component)
    {
        AddComponentInternal(_entity, HandleToEntity(_entity), T_component::ID, component);
        for_loop(i, Listeners.size())
        {
            const std::vector<TagID>& componentIDs = Listeners[i]->g_ComponentIDs();
            if (Listeners[i]->shouldNotifyOnAllComponentOperations())
            {
                Listeners[i]->onAddComponent(_entity, T_component::ID);
            }
            else 
            {
                for_loop (j , componentIDs.size())
                {
                    if (componentIDs[j] == T_component::ID)
                    {
                        Listeners[i]->onAddComponent(_entity, T_component::ID);
                        break;
                    }
                }
            }
        }
    }
    template<class T_component> void RemoveComponent(EntityPTR _entity)
    {
        for_loop(i, Listeners.size())
        {
            const std::vector<TagID>& ComponentIDs = Listeners[i]->getComponentIDs();

            for_loop(j, ComponentIDs.size())
            {
                if (Listeners[i]->shouldNotifyOnAllComponentOperations()) {
                    Listeners[i]->onRemoveComponent(_entity, T_component::ID);
                }
                else
                {
                    if (ComponentIDs[j] == Component::ID)
                    {
                        Listeners[i]->onRemoveComponent(_entity, T_component::ID);
                        break;
                    }
                }
            }
        }
        return RemoveComponentInternal(_entity, Component::ID);
    }   
	template<class T_component> T_component* g_Component(EntityPTR _entity)
    {
        return (T_component*)g_ComponentInternal(HandleToEntity(_entity), Components[T_component::ID], T_component::ID);
    }
    void ComponentDelete(Component_Handle _componentID, unsigned int index);

	//System Methods
    void UpdateSystems(SystemList& systems, float delta);


    // Entity Methods
    EntityPTR MakeEntity(BaseComponent **_comp, const Component_Handle *_componentIDs, size_t _size);
    template<class ...ARGS>  EntityPTR MakeEntity(ARGS&... _args)
    {
        BaseComponent* components[] = {&_args... };
        unsigned int componentIDs[] = { ARGS::ID...};
        return MakeEntity(components, componentIDs, sizeof...(ARGS));
    }

    void RemoveEntity(EntityPTR _handle);


private:
    BaseComponent* g_ComponentInternal(EntityHandle_t& entityComponents, std::vector<uint8_t>& array, Component_Handle componentID);



    void AddComponentInternal(EntityPTR handle, EntityHandle_t& entity, Component_Handle componentID, BaseComponent* component);
	bool RemoveComponentInternal(EntityPTR _handle, Component_Handle _componentID);

    ///TODO: <WARNING!> I believe this is correct as it adds the pointer to the type here
    std::vector<Entity_t* >  Entities;
	Component_map            Components;
	std::vector<BaseSystem*> Systems;
    std::vector<ECSListener*> Listeners;





	uint32_t HandleToEntityIndex(EntityPTR _handle)
	{
		return RawTypeHandle(_handle)->first;
	}
	Entity_t* RawTypeHandle(EntityPTR _handle)
	{
		return (Entity_t*)_handle;
	}
	EntityHandle_t& HandleToEntity(EntityPTR _handle)
	{
		return RawTypeHandle(_handle)->second;
	}



    void UpdateSystemWithMultipleComponents
    (//TODO: Fix this Clusterfuck
        uint32_t index,
        SystemList& systems,
        const std::vector<uint32_t>& _componentTypes,
        std::vector<BaseComponent*>& _componentParam,
        std::vector<std::vector<uint8_t>*>& _componentArrays, 
        float delta
    );

    TagID FindLeastCommonComponent(const std::vector<Component_Handle>& componentTypes, const std::vector<Component_Handle>& componentFlags);
    static unsigned int EntityCount;

};




class TestSystem : public BaseSystem
{
	TestSystem(std::vector<Component_Handle>& comptypeIn)
		:
        BaseSystem()
	{}

	void UpdateComponents(float _delta, BaseComponent **comp)
	{}
};
