#include "PointOfView.h"



PointOfView::PointOfView()
{
}

void PointOfView::update()
{
	_matrix = glm::lookAt(_position, _position + _direction, glm::vec3(0, 1, 0));
}

void PointOfView::setPosition(glm::vec3 position)
{
	_position = position;
}

void PointOfView::setDirection(glm::vec3 direction)
{
	_direction = direction;
}

glm::mat4 PointOfView::matrix()
{
	return _matrix;
}

glm::vec3 PointOfView::position()
{
	return _position;
}

glm::vec3 PointOfView::direction()
{
	return _direction;
}


PointOfView::~PointOfView()
{

}
