#include "Camera.h"



Camera::Camera()
{
}


Camera::~Camera()
{
}

const POV& Camera::pov() const
{
	return m_pov;
}

const mat4& Camera::matrix() const
{
	return m_pov.matrix;
}

const vec3& Camera::position() const
{
	return m_pov.position;
}
