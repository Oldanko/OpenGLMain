#pragma once
#include "WindowManager.h"
#include <glm\glm.hpp>
typedef unsigned short ushort;
typedef unsigned char ubyte;
typedef char byte;


const ubyte CAMERA_MOVE_FORWARD		= 0b00000001;
const ubyte CAMERA_MOVE_BACK		= 0b00000010;
const ubyte CAMERA_MOVE_LEFT		= 0b00000100;
const ubyte CAMERA_MOVE_RIGHT		= 0b00001000;


template<class T>struct KeyifPressed
{
	int m_id;
	T m_action;
	KeyifPressed(int id, T action) : m_id(id), m_action(action) {}
};

template<class T>struct KeyPressed
{
	int m_id;
	T m_action;
	bool isPressed;
	KeyPressed(int id, T action) : m_id(id), m_action(action) {}
};


class Controls
{
	static double m_x, m_y;
	static KeyifPressed<ubyte>* m_cameraControlsKeys;
	static ubyte m_camera;

public:
	static void init(); // read from file
	static ubyte cameraControls();
	static void update();

	static float x();
	static float y();
};

