#include "CameraFreeFloat.h"

#include <glm\gtx\transform.hpp>
#include "Controls.h"
#include "WindowManager.h"


void CameraFreeFloat::update()
{
	const float angleMax = 3.14f / 2;
	angleX += Controls::x()* 0.005f;
	angleY += Controls::y() * -0.005f;
	if (angleY > angleMax)
		angleY = angleMax;
	if (angleY < -angleMax)
		angleY = -angleMax;

	m_pov.direction = mat3(rotate(angleX, vec3(0, 1, 0))) * vec3(1, 0, 1);
	m_pov.direction = mat3(rotate(angleY, normalize(vec3(m_pov.direction.z, 0, -m_pov.direction.x)))) * m_pov.direction;

	if (Controls::cameraControls() & CAMERA_MOVE_FORWARD)
		m_pov.position += m_pov.direction;
	if (Controls::cameraControls() & CAMERA_MOVE_BACK)
		m_pov.position -= m_pov.direction;

	auto forward = normalize(vec2(m_pov.direction.x, m_pov.direction.z));

	if (Controls::cameraControls() & CAMERA_MOVE_LEFT)
	{
		m_pov.position.x += forward.y;
		m_pov.position.z -= forward.x;
	}
	if (Controls::cameraControls() & CAMERA_MOVE_RIGHT)
	{
		m_pov.position.x -= forward.y;
		m_pov.position.z += forward.x;
	}
	m_pov.matrix = lookAt(m_pov.position, m_pov.position + m_pov.direction, glm::vec3(0, 1, 0));
}

CameraFreeFloat::CameraFreeFloat()
{
	m_pov.position = vec3(0, 0, 0);
	angleX = 0;
	angleY = 0;
}


CameraFreeFloat::~CameraFreeFloat()
{
}
