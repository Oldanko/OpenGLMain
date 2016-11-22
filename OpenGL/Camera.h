#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

#include "Controls.h"


class Camera
{
	static const glm::vec2 TILT_LIMITS;
	float m_speed = 5;
	glm::mat4 m_matrix;
	glm::vec3 m_position;
	glm::vec3 m_cameraPosition;
	glm::vec2 m_angle;
	float m_shoulder;
	float m_rotationSpeed;
	bool m_matrixShouldBeCalculated;
public:
	glm::mat4 ViewMatrix();
	void calculateViewMatrix();
	const glm::vec3 & position();
	const glm::vec2 & angles();
	const glm::vec3 & cameraLocalPosition();
	const glm::vec3 & cameraGlobalPosition();
	const glm::vec3 & cameraDirection();
	glm::vec3 clipCoordsToVector(glm::vec2 coords);

	void rotateUp(float a);
	void rotateLeft(float a);

	void moveForward(float a);
	void moveLeft(float a);
	
	void zoom(float a);
	void zoomStep(float a);

	void update();
	
	Camera();
	~Camera();
};

