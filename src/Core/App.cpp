
#include <GL/glew.h>

#include "App.h"

#include <cstdio>
#include <cassert>
#include <memory>

#include "Utils/Camera.h"
#include "Utils\ShaderProgram.h"
#include "GUIState.h"

#include "glm/glm.hpp"
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4, glm::ivec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "glm/gtc/type_ptr.hpp" // glm::value_ptr

namespace core
{
	// Static initialization
	App * App::m_p_instance = NULL;

	App::App(void)
		//Members init
		: m_t(0)
		, m_fps(0)
		, m_pWindow(NULL)
		, m_pCamera(NULL)
		, m_pGUIState(NULL)
		, m_pShaderProgram(NULL)
	{}

	App::~App(void)
	{}

	void App::init_app(int argc, char** argv)
	{
#ifndef NDEBUG
		assert(m_p_instance == NULL);
#endif
		m_p_instance = new App();
		m_p_instance->initApp(argc, argv);
	}

	void App::initApp(int argc, char** argv)
	{
		initGLFW();
		initWindow();
		initGlew();

		// Ensure we can capture the escape key being pressed below
		glfwSetInputMode(m_pWindow, GLFW_STICKY_KEYS, GL_TRUE);
		// Enable vertical sync (on cards that support it)
		glfwSwapInterval(1);

		initCamera();
		initGUIState();
		initShaders();
	}

	void App::initGLFW(void)
	{
		// Initialize GLFW
		if (!glfwInit())
		{
			fprintf(stderr, "Failed to initialize GLFW\n");
			exit(EXIT_FAILURE);
		}
		glfwInit();
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
		glfwWindowHint(GLFW_DECORATED, GL_TRUE);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	}

	void App::initWindow(void)
	{
		// Open a window and create its OpenGL context
		m_pWindow = glfwCreateWindow(1024, 768, "Projet", 0, 0);
		if (!m_pWindow)
		{
			fprintf(stderr, "Failed to open GLFW window\n");
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(m_pWindow);
	}

	void App::initGlew(void)
	{
		// Init glew
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			exit(EXIT_FAILURE);
		}
	}

	void App::initCamera(void)
	{
		m_pCamera = utils::Camera::create_ptr();
		m_pCamera->defaults();
	}

	void App::initGUIState(void)
	{
		m_pGUIState = core::GUIState::create_ptr();
	}

	void App::initShaders(void)
	{
		m_pShaderProgram = utils::ShaderProgram::create_ptr();
		m_pShaderProgram->addShader(GL_VERTEX_SHADER, "../../glsl/simple.vert");
		m_pShaderProgram->addShader(GL_FRAGMENT_SHADER, "../../glsl/simple.frag");
	
		std::string logInfo;
		if (!m_pShaderProgram->compileAndLinkShaders(logInfo))
		{
			throw std::runtime_error(logInfo);
		}
	}

	int32_t App::run_app(void)
	{
		int32_t ret = m_p_instance->runApp();

		return ret;
	}

	int32_t App::runApp(void)
	{
		// First object
		int32_t plane_triangleCount = 2;
		int32_t plane_triangleList[] = { 0, 1, 2, 2, 1, 3 };
		float plane_uvs[] = { 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f };
		float plane_vertices[] = { 
			-2.0, -1.0, 1.0,
			0.50, -1.0, 2.0,
			-0.50, -1.0, -1.0,
			0.50, -1.0, -1.0
		};
		float plane_normals[] = { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 };

		// Second object
		int cube_triangleCount = 12;
		int cube_triangleList[] = { 0, 1, 2, 2, 1, 3, 4, 5, 6, 6, 5, 7, 8, 9, 10, 10, 9, 11, 12, 13, 14, 14, 13, 15, 16, 17, 18, 19, 17, 20, 21, 22, 23, 24, 25, 26, };
		float cube_uvs[] = { 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, };
		float cube_vertices[] = { -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5 };
		float cube_normals[] = { 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, };

		
		// Create a Vertex Array Object
		GLuint vaoPlane;
		glGenVertexArrays(1, &vaoPlane);

		// Create a VBO for each array
		GLuint vboPlane[4];
		glGenBuffers(4, vboPlane);

		// Bind the VAO
		glBindVertexArray(vaoPlane);

		// Bind indices and upload data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboPlane[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane_triangleList), plane_triangleList, GL_STATIC_DRAW);

		// Bind vertices and upload data
		glBindBuffer(GL_ARRAY_BUFFER, vboPlane[1]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 3, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);

		// Bind normals and upload data
		glBindBuffer(GL_ARRAY_BUFFER, vboPlane[2]);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 3, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(plane_normals), plane_normals, GL_STATIC_DRAW);

		// Bind uv coords and upload data
		glBindBuffer(GL_ARRAY_BUFFER, vboPlane[3]);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 2, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(plane_uvs), plane_uvs, GL_STATIC_DRAW);

		// Unbind everything
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Create a Vertex Array Object
		GLuint vao;
		glGenVertexArrays(1, &vao);

		// Create a VBO for each array
		GLuint vbo[4];
		glGenBuffers(4, vbo);

		// Bind the VAO
		glBindVertexArray(vao);

		// Bind indices and upload data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_triangleList), cube_triangleList, GL_STATIC_DRAW);

		// Bind vertices and upload data
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 3, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

		// Bind normals and upload data
		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 3, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), cube_normals, GL_STATIC_DRAW);

		// Bind uv coords and upload data
		glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 2, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_uvs), cube_uvs, GL_STATIC_DRAW);

		// Unbind everything
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		// Viewport 
		glViewport(0, 0, 1024, 768);

		// Upload uniforms
		GLuint mvpLocation = m_pShaderProgram->getUniformIndex("MVP");

		do
		{
			m_t = glfwGetTime();

			// Mouse states
			int leftButton = glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT);
			int rightButton = glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_RIGHT);
			int middleButton = glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_MIDDLE);

			if (leftButton == GLFW_PRESS)
				m_pGUIState->setTurnLock(true);
			else
				m_pGUIState->setTurnLock(false);

			if (rightButton == GLFW_PRESS)
				m_pGUIState->setZoomLock(true);
			else
				m_pGUIState->setZoomLock(false);

			if (middleButton == GLFW_PRESS)
				m_pGUIState->setPanLock(true);
			else
				m_pGUIState->setPanLock(false);

			// Camera movements
			int altPressed = glfwGetKey(m_pWindow, GLFW_KEY_LEFT_SHIFT);
			if (!altPressed && (leftButton == GLFW_PRESS || rightButton == GLFW_PRESS || middleButton == GLFW_PRESS))
			{
				double x; double y;
				glfwGetCursorPos(m_pWindow, &x, &y);
				m_pGUIState->setLockPosition(x, y);
			}
			if (altPressed == GLFW_PRESS)
			{
				double mousex; double mousey;
				glfwGetCursorPos(m_pWindow, &mousex, &mousey);
				double diffLockPositionX = mousex - m_pGUIState->getLockPosition(0);
				double diffLockPositionY = mousey - m_pGUIState->getLockPosition(1);
				if (m_pGUIState->isZoomLock())
				{
					float zoomDir = 0.0;
					if (diffLockPositionX > 0)
					{
						zoomDir = -1.f;
					}
					else if (diffLockPositionX < 0)
					{
						zoomDir = 1.f;
					}
					m_pCamera->zoom(float(zoomDir * GUIState::get_mouse_zoom_speed()));
				}
				else if (m_pGUIState->isTurnLock())
				{
					m_pCamera->turn(float(diffLockPositionY * GUIState::get_mouse_turn_speed()),
						float(diffLockPositionX * GUIState::get_mouse_turn_speed()));

				}
				else if (m_pGUIState->isPanLock())
				{
					m_pCamera->pan(float(diffLockPositionX * GUIState::get_mouse_pan_speed()),
						float(diffLockPositionY * GUIState::get_mouse_pan_speed()));
				}
				m_pGUIState->setLockPosition(mousex, mousey);
			}

			// Default states
			glEnable(GL_DEPTH_TEST);

			// Clear the front buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Get camera matrices
			glm::mat4 projection = glm::perspective(45.0f, 1024.f / 728.f, 0.1f, 100.f);
			glm::mat4 worldToView = glm::lookAt(
				m_pCamera->getWhereILook(), 
				m_pCamera->getOrigin(), 
				m_pCamera->getUp()
			);
			glm::mat4 objectToWorld;
			glm::mat4 mvp = projection * worldToView * objectToWorld;

			// Select shader
			m_pShaderProgram->use();

			// Upload uniforms
			m_pShaderProgram->setUniform(mvpLocation, mvp);
			
			// Render here

			// Render cube
			glBindVertexArray(vao);
			glDrawElementsInstanced(GL_TRIANGLES, cube_triangleCount * 3, GL_UNSIGNED_INT, (void*)0, 1);


			// Render plane
			glBindVertexArray(vaoPlane);
			glDrawElementsInstanced(GL_TRIANGLES, plane_triangleCount * 3, GL_UNSIGNED_INT, (void*)0, 1);

			glfwSwapBuffers(m_pWindow);
			glfwPollEvents();

			double newTime = glfwGetTime();
			m_fps = 1.f / (newTime - m_t);
		} // Check if the ESC key was pressed
		while (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS);

		return 0;
	}

	void App::exit_app(void)
	{
		m_p_instance->exitApp();
		delete m_p_instance;

		// Close OpenGL window and terminate GLFW
		glfwTerminate();

		exit(EXIT_SUCCESS);
	}

	void App::exitApp(void)
	{
		//Window::release_ptr(m_pWindow);
		//utils::Camera::release_ptr(m_pCamera);
	}

}; // namespace Core
