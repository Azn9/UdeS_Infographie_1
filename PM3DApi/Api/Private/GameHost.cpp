#include "Api/Public/GameHost.h"

PM3D_API::GameHost::~GameHost()
{
    delete baseComponent;
}

void PM3D_API::GameHost::SetBaseComponent(ContainerComponent* component)
{
    this->baseComponent = component;
}

void PM3D_API::GameHost::Draw()
{
    // TODO
}
