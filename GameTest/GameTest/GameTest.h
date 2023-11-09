#pragma once

#include "../../PM3DApi/Api/Public/GameHost.h"

class GameTest : public PM3D_API::GameHost
{
public:
	GameTest() = default;
	~GameTest() override = default;

	// Inherited via GameHost
	void Initialize() override;

protected:

private:

};
