#include "Controls.h"

KeyifPressed<ubyte> cameraControls_default[] = {
	KeyifPressed<ubyte>(GLFW_KEY_W,			CAMERA_MOVE_FORWARD),
	KeyifPressed<ubyte>(GLFW_KEY_S,			CAMERA_MOVE_BACK),
	KeyifPressed<ubyte>(GLFW_KEY_A,			CAMERA_MOVE_LEFT),
	KeyifPressed<ubyte>(GLFW_KEY_D,			CAMERA_MOVE_RIGHT)
};

KeyifPressed<ubyte>* Controls::m_cameraControlsKeys = cameraControls_default;

ubyte Controls::m_camera = 0;

double Controls::m_x;
double Controls::m_y;


void Controls::init()
{	
}
ubyte Controls::cameraControls()
{
	return m_camera;
}
void Controls::update()
{
	// =================Keyboard Input=================

	//Camera Controls
	m_camera = 0;

	for (int i = 0; i < 4; i++) // 
		if (glfwGetKey(WindowManager::window(), m_cameraControlsKeys[i].m_id) == GLFW_PRESS)
			m_camera |= m_cameraControlsKeys[i].m_action;

	// =================Mouse Input====================
	glfwGetCursorPos(WindowManager::window(), &m_x, &m_y);

	m_x = WindowManager::width() / 2 - m_x;
	m_y = WindowManager::height() / 2 - m_y;

	glfwSetCursorPos(WindowManager::window(), WindowManager::width()/2, WindowManager::height()/2);	
}

float Controls::x()
{
	return m_x;
}

float Controls::y()
{
	return m_y;
}