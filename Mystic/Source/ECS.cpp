#include<algorithm>

#include"ECS.h"
using Entity_ID = unsigned int;

//-----------------------------------------------------------------------------------
typedef std::unordered_map <Component_Handle, std::vector<uint8_t>> Component_map;

EntityComponentSystem::~EntityComponentSystem()
{
    for (Component_map::iterator it = Components.begin(); it != Components.end(); ++it)
    {
        size_t typeSize = BaseComponent::g_TypeSize(it->first);
        ComponentDeleteFunc freefn = BaseComponent::g_TypeDeleteFunction(it->first);
        for (uint32_t i = 0; i < it->second.size(); i += typeSize)
        {
            freefn((BaseComponent*)&it->second[i]);
        }
    }

    for_loop(i , Entities.size())
    {
        delete Entities[i];
    }
}

//Entity Methods
EntityPTR EntityComponentSystem::MakeEntity(BaseComponent ** _comp, const Component_Handle * _componentIDs, size_t _size)
{
    Entity_t *newEntity = new Entity_t();
    EntityPTR results = (EntityPTR)newEntity;
    
    for_loop(i, _size)
    {
        if (!BaseComponent::is_type_valid(_componentIDs[i]))
        {
            __debugbreak();
          //  DEBUG_LOG("ECS", LOG_ERROR, "'%u' is not a valid component type.", componentIDs[i]);
          //  delete newEntity;
          //  return NULL_ENTITY_HANDLE;
        }

        AddComponentInternal(results, newEntity->second, _componentIDs[i], _comp[i]);
    }

    newEntity->first = Entities.size();
    Entities.push_back(newEntity);

    for_loop(i, Listeners.size())
    {
        bool isValid = true;
        if (Listeners[i]->shouldNotifyOnAllEntityOperations()) 
        {
            Listeners[i]->onMakeEntity(results);
        }
        else
        {
            for_loop(j , Listeners[i]->g_ComponentIDs().size())
            {
                bool hasComponent = false;
                for_loop (k , _size)
                {
                    if (Listeners[i]->g_ComponentIDs()[j] == _componentIDs[k])
                    {
                        hasComponent = true;
                        break;
                    }
                }
                if (!hasComponent)
                {
                    isValid = false;
                    break;
                }
            }
            if (isValid)
            {
                Listeners[i]->onMakeEntity(results);
            }
        }
    }

    return results;
}


void EntityComponentSystem::RemoveEntity(EntityPTR _handle)
{
    EntityHandle_t& entity = HandleToEntity(_handle);
  
    for_loop ( i, Listeners.size())
    {
        const std::vector<Component_Handle>& componentIDs = Listeners[i]->g_ComponentIDs();
        bool isValid = true;
        if (Listeners[i]->shouldNotifyOnAllEntityOperations()) {
            Listeners[i]->onRemoveEntity(_handle);
        }
        else {
            for_loop (j, componentIDs.size())
            {
                bool hasComponent = false;
                for_loop (k ,  entity.size())
                {
                    if (componentIDs[j] == entity[k].first)
                    {
                        hasComponent = true;
                        break;
                    }
                }
                if (!hasComponent)
                {
                    isValid = false;
                    break;
                }
            }
            if (isValid) {
                Listeners[i]->onRemoveEntity(_handle);
            }
        }
    }

    for_loop (i, entity.size())
    {
        ComponentDelete(entity[i].first, entity[i].second);
    }

    TagID destIndex = HandleToEntityIndex(_handle);
    TagID srcIndex = Entities.size() - 1;
    delete Entities[destIndex];
    Entities[destIndex] = Entities[srcIndex];
    Entities[destIndex]->first = destIndex;
    Entities.pop_back();
}

void EntityComponentSystem::AddComponentInternal(EntityPTR handle, EntityHandle_t & entity, Component_Handle componentID, BaseComponent* component)
{
    ComponentCreateFunc createfn = BaseComponent::g_TypeCreateFunction(componentID);
    std::pair<uint32_t, uint32_t> newPair;
    newPair.first = componentID;
    newPair.second = createfn(Components[componentID], handle, component);
    entity.push_back(newPair);
}
bool EntityComponentSystem::RemoveComponentInternal(EntityPTR _handle, Component_Handle _componentID)
{
	EntityHandle_t& EntityComponents = HandleToEntity(_handle);
	for_loop(I, EntityComponents.size())
	{
		if (_componentID == EntityComponents[I].first)
		{
		///IMPLEMENT THIS	DeleteComponent(EntityComponents[I].first, EntityComponents[I].second);
			unsigned int SourceIndex = EntityComponents.size() - 1;
			unsigned int DestinationIndex = I;
			EntityComponents[DestinationIndex] = EntityComponents[SourceIndex];
			EntityComponents.pop_back();
			return true;
		}
	}
	return false;
}

BaseComponent * EntityComponentSystem::g_ComponentInternal(EntityHandle_t & entityComponents, std::vector<uint8_t>& array, Component_Handle componentID)
{
    for_loop(i, entityComponents.size())
    {
        if (componentID == entityComponents[i].first)
        {
            return (BaseComponent*)&array[entityComponents[i].second];
        }
    }
    return nullptr;
}


void EntityComponentSystem::ComponentDelete(Component_Handle _componentID, unsigned int index)
{
    std::vector<unsigned char>& ComponentArray = Components[_componentID];
    ComponentDeleteFunc FreeFunction = BaseComponent::g_TypeDeleteFunction(_componentID);
    size_t typeSize = BaseComponent::g_TypeSize(_componentID);

    unsigned int SourceIndex = ComponentArray.size() - typeSize;

    BaseComponent* destComponent = (BaseComponent*)&ComponentArray[index];
    BaseComponent* srcComponent = (BaseComponent*)&ComponentArray[SourceIndex];
    FreeFunction(destComponent);

    if (index == SourceIndex)
    {
        ComponentArray.resize(SourceIndex);
        return;
    }
    std::memcpy(destComponent, srcComponent, typeSize);

    EntityHandle_t& srcComponents = HandleToEntity(srcComponent->Entity);
    for_loop(i, srcComponents.size())
    {
        if (_componentID == srcComponents[i].first && SourceIndex == srcComponents[i].second)
        {
            srcComponents[i].second = index;
            break;
        }
    }
    ComponentArray.resize(SourceIndex);
}


TagID EntityComponentSystem::FindLeastCommonComponent(const std::vector<TagID>& _componentTypes, const std::vector<TagID>& _componentFlags)
{
    uint32_t minSize = (uint32_t)-1;
    uint32_t minIndex = (uint32_t)-1;
    for_loop(i , _componentTypes.size())
    {
        if ((_componentFlags[i] & BaseSystem::FLAG_OPTIONAL) != 0)
        {
            continue;
        }
        size_t typeSize = BaseComponent::g_TypeSize(_componentTypes[i]);
        uint32_t size = Components[_componentTypes[i]].size() / typeSize;
        if (size <= minSize)
        {
            minSize = size;
            minIndex = i;
        }
    }
    return minIndex;
}

void EntityComponentSystem::UpdateSystems(SystemList& _systems, float _delta)
{
    std::vector<BaseComponent*> componentParam;
    std::vector<std::vector<unsigned char>*> componentArrays;

    for(int i = 0;i < _systems.size(); ++i)
    {
        const std::vector<unsigned int>& componentTypes = _systems[i]->g_ComponentTypes();

        if (componentTypes.size() == 1)
        {
            size_t typeSize = BaseComponent::g_TypeSize(componentTypes[0]);
            std::vector<unsigned char>& array = Components[componentTypes[0]];
            for (unsigned int j = 0; j < array.size(); j += typeSize)
            {
                BaseComponent* component = (BaseComponent*)&array[j];
                _systems[i]->UpdateComponents(_delta, &component);
            }
        }
        else
        {
            UpdateSystemWithMultipleComponents(i, _systems, componentTypes, componentParam, componentArrays, _delta);
        }
    }
}
void EntityComponentSystem::UpdateSystemWithMultipleComponents
(/// I despise this function and its signature
    uint32_t _index,
    SystemList& _systems,
    const std::vector<uint32_t>& _componentTypes,
    std::vector<BaseComponent*>& _componentParam,
    std::vector<std::vector<uint8_t>*>& _componentArrays,
    float _delta
)
{

    _componentParam.resize(std::max(_componentParam.size(), _componentTypes.size()));
    _componentArrays.resize(std::max(_componentArrays.size(), _componentTypes.size()));

    const std::vector<uint32_t>& componentFlags = _systems[_index]->g_ComponentFlags();

    for_loop(i , _componentTypes.size()) 
    {
        _componentArrays[i] = &Components[_componentTypes[i]];
    }
    uint32_t minSizeIndex = FindLeastCommonComponent(_componentTypes, componentFlags);

    size_t typeSize = BaseComponent::g_TypeSize(_componentTypes[minSizeIndex]);
    std::vector<uint8_t>& array = *_componentArrays[minSizeIndex];

    for (uint32_t i = 0; i < array.size(); i += typeSize)
    {
        _componentParam[minSizeIndex] = (BaseComponent*)&array[i];
        EntityHandle_t& entityComponents = HandleToEntity(_componentParam[minSizeIndex]->Entity);

        bool isValid = true;
        for_loop (j, _componentTypes.size())
        {
            if (j == minSizeIndex)
            {
                continue;
            }

            _componentParam[j] =
                g_ComponentInternal
                (
                    entityComponents,
                    *_componentArrays[j],
                    _componentTypes[j]
                );

            if (_componentParam[j] == nullptr && (componentFlags[j] & BaseSystem::FLAG_OPTIONAL) == 0)
            {
                isValid = false;
                break;
            }
        }

        if (isValid)
        {
            _systems[_index]->UpdateComponents(_delta, &_componentParam[0]);
        }
    }
}


    

// https://www.youtube.com/watch?v=51qSGUtaJwc
// BENNY: https://www.youtube.com/watch?v=4-LRor0UG6o&list=PLEETnX-uPtBUrfzE3Dxy3PWyApnW6YEMm&index=7
// https://gamedev.stackexchange.com/questions/4898/are-there-existing-foss-component-based-frameworks/4966#4966
// OTHERDUDE: https://www.youtube.com/watch?v=WbwXxms80Z4
// Springs: https://www.myphysicslab.com/springs/single-spring-en.html








