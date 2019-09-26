#include"Common.h"

#include"ECSsystem.h"



bool BaseSystem::is_valid()
{
    for_loop(i , ComponentFlags.size())
    {
        if ((ComponentFlags[i] & BaseSystem::FLAG_OPTIONAL) == 0)
        {
            return true;
        }
    }
    return false;
}

bool SystemList::RemoveSystem(BaseSystem& _system)
{
    for_loop (i, Systems.size())
    {
        if (&_system == Systems[i])
        {
            Systems.erase(Systems.begin() + i);
            return true;
        }
    }
    return false;
}