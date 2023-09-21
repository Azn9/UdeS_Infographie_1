#pragma once

#include "../../Component/Component.h"

#define FAST_OBJ_IMPLEMENTATION
#include <string>

#include "../../Util/fast_obj.h"

namespace PM3D_API
{
class MeshRenderer final : public Component
{
public:
	MeshRenderer(std::string meshName);
	MeshRenderer(fastObjMesh* mesh);

	~MeshRenderer() override;
	void DrawSelf() const override;

private:
	fastObjMesh* mesh;

};
}
