﻿#pragma once

#include "../GameObject/GameObject.h"

class Light : public PM3D_API::GameObject
{
public:
	Light(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation);
	Light(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, float intensity);
	Light(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, float intensity, DirectX::XMFLOAT3 color);

protected:
	
private:
	float Intensity;
	DirectX::XMFLOAT3 Color;
};
