#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

#pragma once
class PointOfView
{
	glm::mat4 _matrix;
	glm::vec3 _position;
	glm::vec3 _direction;
public:
	PointOfView();
	void update();
	void setPosition(glm::vec3 position);
	void setDirection(glm::vec3 direction);
	glm::mat4 matrix();
	glm::vec3 position();
	glm::vec3 direction();

	~PointOfView();
};

