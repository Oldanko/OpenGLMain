#include "WindowManager.h"


GLFWwindow *WindowManager::m_window = nullptr;
int WindowManager::m_height = 0;
int WindowManager::m_width = 0;

float WindowManager::m_FoV = 0.0f;

float WindowManager::m_lastTime = 0;
float WindowManager::m_timeDelta = 0;

glm::mat4 WindowManager::m_projectionMatrix = glm::perspective(glm::radians(70.0f), 1280.0f / 720.0f, 1.0f, 100000.0f);


void WindowManager::init()
{

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_width = 1280;
	m_height = 720;
	m_FoV = 70.0f;

	m_projectionMatrix = glm::perspective(glm::radians(m_FoV), (float)m_width / m_height, 1.0f, 100000.0f);

	m_window = glfwCreateWindow(m_width, m_height, "OpenGL Engine", NULL, NULL);
	if (m_window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSetMouseButtonCallback(m_window, GLFW_MOUSE_BUTTON_1);

	m_lastTime = glfwGetTime();
}

bool WindowManager::update()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();

	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		close();


	float time = glfwGetTime();
	m_timeDelta = time - m_lastTime;
	m_lastTime = time;

	return !glfwWindowShouldClose(m_window);
}

void WindowManager::close()
{
	glfwSetWindowShouldClose(m_window, GL_TRUE);
}

const int& WindowManager::width() { return m_width; }
const int& WindowManager::height() { return m_height; }
const float & WindowManager::FoV()
{
	return m_FoV;
}
GLFWwindow *WindowManager::window() { return m_window; }
const float& WindowManager::timeDelta() { return m_timeDelta; }

const glm::mat4 WindowManager::projectionMatrix()
{
	return m_projectionMatrix;
}
