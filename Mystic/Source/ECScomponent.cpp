#include"ECScomponent.h"
  /// COMPLETE
std::vector<BaseComponent::Properties> BaseComponent::ComponentTypes;



TagID BaseComponent::RegisterComponentType
(
    ComponentCreateFunc _createfunc,
    ComponentDeleteFunc _deletefunc,
    size_t _size
)
{
    TagID results = ComponentTypes.size();
    ComponentTypes.push_back({ _createfunc ,_deletefunc ,_size });
    ComponentTypes.back().DebugTypeID = ComponentTypes.size();
    return results;
}     
 