#pragma once

#include "ECScomponent.h"
using System_ID = size_t;

/// COMPLETED
class BaseSystem
{
public:
    enum
    {/// Replace this and make this std::optional
        FLAG_OPTIONAL = 1,
    };
    BaseSystem() = default;
    virtual void UpdateComponents(float delta, BaseComponent** components) {};

    const std::vector<unsigned int>& g_ComponentTypes()
    {
        return ComponentTypes;
    }
    const std::vector<unsigned int>& g_ComponentFlags()
    {
        return ComponentFlags;
    }
    bool is_valid();


    std::string SystemName;
protected:
    void AddComponentType(TagID componentType, TagID componentFlag = 0)
    {
        ComponentTypes.push_back(componentType);
        ComponentFlags.push_back(componentFlag);
    }
private:
    std::vector<unsigned int> ComponentTypes;
    std::vector<unsigned int> ComponentFlags;
};

class SystemList
{
public:
    inline bool AddSystem(BaseSystem& _system)
    {
        if (!_system.is_valid())
        {
            return false;
        }
        Systems.push_back(&_system);
        return true;
    }
    inline bool RemoveSystem(BaseSystem& _system);


    inline size_t size()
    {
        return Systems.size();
    }
    inline BaseSystem* operator[](size_t _index)
    {
        return Systems[_index];
    }

private:
    std::vector<BaseSystem*> Systems;
};