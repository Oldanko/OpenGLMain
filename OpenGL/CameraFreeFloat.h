#pragma once
#include "Camera.h"
class CameraFreeFloat :
	public Camera
{
public:
	void update() override;
	CameraFreeFloat();
	~CameraFreeFloat();
};

