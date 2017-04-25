#pragma once
#include <glm\glm.hpp>
using namespace glm;

struct POV
{
	mat4 matrix;
	vec3 position;
	vec3 direction;
};

class Camera
{
protected:
	POV m_pov;
	float angleX;
	float angleY;
public:
	Camera();
	~Camera();
	virtual void update() = 0;
	const POV& pov() const;
	const mat4& matrix() const;
	const vec3& position() const;
};

