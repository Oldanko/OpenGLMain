#include "GUI.h"

float clamp(float x, float a, float b)
{
	return
		a < b ?
		x < a ? a :
		x > b ? b : x
		:
		x > a ? a :
		x < b ? b : x;
}


GUI::GUI()
{
}


GUI::~GUI()
{
}
