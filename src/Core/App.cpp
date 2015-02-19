
#include <GL/glew.h>

#include "App.h"

#include <cstdio>
#include <cassert>
#include <memory>

#include "Config.h"
#include "Utils/Camera.h"
#include "Utils/QuadBlit.h"
#include "Utils/ShaderProgram.h"
#include "GUIState.h"
#include "WorldObject/Cube.h"
#include "WorldObject/Plane.h"
#include "WorldObject/Scene.h"
#include "Renderer/GBufferRenderer.h"
#include "Renderer/BlitRenderer.h"
#include "Renderer/IlluminationRenderer.h"

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
		, m_pScene(NULL)
		, m_pQuadBlit(NULL)
		, m_pGbuffer(NULL)
		, m_pBlit(NULL)
		, m_pIllumination(NULL)
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
		initRenderers();
		initScene();
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
		m_pWindow = glfwCreateWindow(core::Config::WINDOW_WIDTH, core::Config::WINDOW_HEIGHT, "Projet", 0, 0);
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

	void App::initRenderers(void)
	{
		m_pGbuffer = renderer::GBufferRenderer::create_ptr();
		m_pIllumination = renderer::IlluminationRenderer::create_ptr();
		m_pBlit = renderer::BlitRenderer::create_ptr();
	}

	void App::initScene(void)
	{
		m_pScene = worldObject::Scene::create_ptr();

		worldObject::Cube * cube = worldObject::Cube::create_ptr();
		worldObject::Plane * plane = worldObject::Plane::create_ptr();

		m_pScene->addObject(cube);
		m_pScene->addObject(plane);

		m_pQuadBlit = utils::QuadBlit::create_ptr();
	}

	int32_t App::run_app(void)
	{
		int32_t ret = m_p_instance->runApp();

		return ret;
	}

	int32_t App::runApp(void)
	{
		// Viewport 
		glViewport(0, 0, core::Config::WINDOW_WIDTH, core::Config::WINDOW_HEIGHT);

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
			
			// Get camera matrices
			glm::mat4 projection = glm::perspective(45.0f, (float)core::Config::WINDOW_WIDTH / (float)core::Config::WINDOW_HEIGHT, 0.1f, 100.f);
			glm::mat4 worldToView = glm::lookAt(
				m_pCamera->getWhereILook(),
				m_pCamera->getOrigin(), 
				m_pCamera->getUp()
			);
			glm::mat4 objectToWorld;
			glm::mat4 mvp = projection * worldToView * objectToWorld;

			// Clear the front buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Default states
			glEnable(GL_DEPTH_TEST);

			// RENDER GBUFFEROBJECT
			m_pGbuffer->render(m_pScene, mvp);

			// RENDER ILLUMINATION
			m_pIllumination->addTextureToDraw(m_pGbuffer->getTextureId(0), 0);
			m_pIllumination->addTextureToDraw(m_pGbuffer->getTextureId(1), 1);
			m_pIllumination->addTextureToDraw(m_pGbuffer->getTextureId(2), 2);
			m_pIllumination->render(m_pQuadBlit);
	
			// RENDER BLIT QUADS
			m_pBlit->addTextureToDraw(m_pGbuffer->getTextureId(0), 0);
			m_pBlit->addTextureToDraw(m_pGbuffer->getTextureId(1), 1);
			m_pBlit->addTextureToDraw(m_pGbuffer->getTextureId(2), 2);
			m_pBlit->render(m_pQuadBlit);

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

		exit(EXIT_SUCCESS);
	}

	void App::exitApp(void)
	{
		utils::Camera::release_ptr(m_pCamera);
		core::GUIState::release_ptr(m_pGUIState);
		worldObject::Scene::release_ptr(m_pScene);
		renderer::BlitRenderer::release_ptr(m_pBlit);
		renderer::GBufferRenderer::release_ptr(m_pGbuffer);
		renderer::IlluminationRenderer::release_ptr(m_pIllumination);

		// Close OpenGL window and terminate GLFW
		glfwTerminate();
	}

}; // namespace Core
