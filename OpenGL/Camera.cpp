#include "Camera.h"

const glm::vec2 Camera::TILT_LIMITS = glm::vec2(glm::radians(5.0f), glm::radians(89.0f));

/*glm::vec3 rotateY(glm::vec3 v, GLfloat a)
{
	return glm::vec3(
		cos(a)*v.x + sin(a)*v.z,
		v.y,
		-sin(a)*v.x + cos(a)*v.z
		);
}*/

glm::mat4 Camera::ViewMatrix()
{
	return m_matrix;
}

void Camera::calculateViewMatrix()
{
	if (m_matrixShouldBeCalculated)
	{
		m_cameraPosition= rotateY(glm::vec3(0, m_shoulder*sin(m_angle.x), m_shoulder*cos(m_angle.x)), m_angle.y);
		m_matrix = glm::lookAt(m_position + m_cameraPosition, m_position, glm::vec3(0.0, 1.0, 0.0));
		m_matrixShouldBeCalculated = false;
	}
}

const glm::vec3 Camera::cameraLocalPosition()
{
	return m_cameraPosition;
}

const glm::vec3 Camera::cameraGlobalPosition()
{
	return m_cameraPosition + m_position;
}

const glm::vec3 Camera::cameraDirection()
{
	return normalize(-m_cameraPosition);
}

void Camera::rotateUp(float a)
{
	m_angle.x += a * m_rotationSpeed;
	if (m_angle.x < TILT_LIMITS.x)
		m_angle.x = TILT_LIMITS.x;
	if (m_angle.x > TILT_LIMITS.y)
		m_angle.x = TILT_LIMITS.y;
	m_matrixShouldBeCalculated = true;
}


void Camera::rotateLeft(float a)
{
	m_angle.y -= a* m_rotationSpeed;
	m_matrixShouldBeCalculated = true;
}


void Camera::moveForward(float a)
{
	glm::vec2 direction = glm::normalize(/*glm::vec2(0)*/ -glm::vec2(m_cameraPosition.x, m_cameraPosition.z));
	m_position.x += a * m_speed * direction.x* sqrt(m_shoulder);
	m_position.z += a * m_speed * direction.y* sqrt(m_shoulder);
	m_matrixShouldBeCalculated = true;
}


void Camera::moveLeft(float a)
{
	glm::vec2 direction = glm::normalize(/*glm::vec2(0)*/ - glm::vec2(m_cameraPosition.x, m_cameraPosition.z));
	m_position.z -= a * m_speed * direction.x* sqrt(m_shoulder);
	m_position.x += a * m_speed * direction.y* sqrt(m_shoulder);
	m_matrixShouldBeCalculated = true;
}


void Camera::zoom(float a)
{
	m_shoulder -= a * sqrt(m_shoulder) * m_speed;
	if (m_shoulder < 5.0f)
		m_shoulder = 5.0f;
	if (m_shoulder > 200.0f)
		m_shoulder = 200.0f;
	m_matrixShouldBeCalculated = true;
}

void Camera::zoomStep(float a)
{
	m_shoulder -= a * sqrt(m_shoulder);
	if (m_shoulder < 5.0f)
		m_shoulder = 5.0f;
	if (m_shoulder > 200.0f)
		m_shoulder = 200.0f;
	m_matrixShouldBeCalculated = true;
}

void Camera::update()
{
	ushort cameraControls = Controls::cameraControls();
	if (cameraControls & CAMERA_MOVE_FORWARD)
		moveForward(WindowManager::timeDelta());
	if (cameraControls & CAMERA_MOVE_BACK)
		moveForward(-WindowManager::timeDelta());
	if (cameraControls & CAMERA_MOVE_LEFT)
		moveLeft(WindowManager::timeDelta());
	if (cameraControls & CAMERA_MOVE_RIGHT)
		moveLeft(-WindowManager::timeDelta());
	if (cameraControls & CAMERA_ROTATE_UP)
		rotateUp(WindowManager::timeDelta());
	if (cameraControls & CAMERA_ROTATE_DOWN)
		rotateUp(-WindowManager::timeDelta());
	if (cameraControls & CAMERA_ROTATE_LEFT)
		rotateLeft(WindowManager::timeDelta());
	if (cameraControls & CAMERA_ROTATE_RIGHT)
		rotateLeft(-WindowManager::timeDelta());
	if (cameraControls & CAMERA_ZOOM_IN)
		zoom(WindowManager::timeDelta());
	if (cameraControls & CAMERA_ZOOM_OUT)
		zoom(-WindowManager::timeDelta());
	if (cameraControls & CAMERA_ROTATE_MOUSE)
	{
		glm::vec2 delta = Controls::mouseMMBCoordsDelta();

		rotateUp(WindowManager::timeDelta() * delta.y / WindowManager::height() * 500);
		rotateLeft(WindowManager::timeDelta() * delta.x / WindowManager::width() * 500);
	}
	if (cameraControls & CAMERA_MOVE_MOUSE)
	{
		glm::vec2 delta = Controls::mouseCoords() - Controls::mouseMMBClickCoords();

		moveForward(-WindowManager::timeDelta() * delta.y / WindowManager::height() * 5);
		moveLeft(-WindowManager::timeDelta() * delta.x / WindowManager::width() * 5);
	}
	if (cameraControls & CAMERA_ZOOM_IN_STEP)
		zoomStep(0.4f);
	if (cameraControls & CAMERA_ZOOM_OUT_STEP)
		zoomStep(-0.4f);
	

	calculateViewMatrix();
}


Camera::Camera()
{
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_shoulder = 150.0f;
	m_angle = glm::vec2(glm::radians(30.0f), 0.0f);
	m_rotationSpeed = 3.0f;
	m_matrixShouldBeCalculated = true;
	calculateViewMatrix();
}

Camera::~Camera()
{
}
