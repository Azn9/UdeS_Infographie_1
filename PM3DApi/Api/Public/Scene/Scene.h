#pragma once
#include "../Component/Component.h"

namespace PM3D_API
{
class Scene : public Component
{
public:
	Scene() : Component({0, 0, 0}, {0, 0, 0}, {0, 0, 0}) {}
	~Scene() override;
	virtual void Initialize();
	
protected:
	
private:

};
}
