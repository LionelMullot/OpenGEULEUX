
#include "App.h"

#include <cstdio>
#include <cassert>
#include <memory>

#include "Config.h"
#include "Utils/Camera.h"
#include "Utils/QuadBlit.h"
#include "Utils/ShaderProgram.h"
#include "Utils/imgui/imgui.h"
#include "Utils/imgui/imguiRenderGL3.h"
#include "GUIState.h"
#include "WorldObject/Cube.h"
#include "WorldObject/Plane.h"
#include "WorldObject/Scene.h"
#include "WorldObject/AreaLight.h"
#include "Renderer/GBufferRenderer.h"
#include "Renderer/BlitRenderer.h"
#include "Renderer/IlluminationRenderer.h"
#include "Renderer/ShadowRenderer.h"

#include "glm/glm.hpp"
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4, glm::ivec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective

#define M_PI        3.14159265358979323846264338327950288f   /* pi */
#define UI 1
#define BLITRENDER 0

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
		, m_pAreaLight(NULL)
		, m_pGbuffer(NULL)
		, m_pBlit(NULL)
		, m_pIllumination(NULL)
		, m_pShadowBuffer(NULL)
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
		initUI();

		// Ensure we can capture the escape key being pressed below
		glfwSetInputMode(m_pWindow, GLFW_STICKY_KEYS, GL_TRUE);
		// Enable vertical sync (on cards that support it)
		glfwSwapInterval(1);

		initCamera();
		initGUIState();
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

	void App::initUI(void)
	{
		// Init UI
		if (!imguiRenderGLInit("../../font/DroidSans.ttf"))
		{
			fprintf(stderr, "Could not init GUI renderer.\n");
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
		m_pIllumination = renderer::IlluminationRenderer::create_ptr(m_pAreaLight);
		m_pBlit = renderer::BlitRenderer::create_ptr();
		m_pShadowBuffer = renderer::ShadowRenderer::create_ptr();
	}

	void App::initScene(void)
	{
		m_pScene = worldObject::Scene::create_ptr();

		worldObject::Cube * cube = worldObject::Cube::create_ptr();
		worldObject::Plane * plane = worldObject::Plane::create_ptr();
		m_pAreaLight = worldObject::AreaLight::create_ptr();

		m_pScene->addObject(cube);
		m_pScene->addObject(plane);

		m_pQuadBlit = utils::QuadBlit::create_ptr();

		initRenderers();
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
			glm::mat4 projection = glm::perspective(45.0f, (float)core::Config::WINDOW_WIDTH / (float)core::Config::WINDOW_HEIGHT, 0.1f, 1000.f);
			glm::mat4 worldToView = glm::lookAt(
				m_pCamera->getWhereILook(),
				m_pCamera->getOrigin(), 
				m_pCamera->getUp()
			); // Cam inv Model View
			glm::mat4 objectToWorld; // Model Matrix for RTS (Rotation / Translation / Scale)
			glm::mat4 mvp = projection * worldToView * objectToWorld; 
			glm::vec3 vCameraPosition = m_pCamera->getWhereILook();
			// Compute the inverse worldToScreen matrix
			glm::mat4 screenToWorld = glm::transpose(glm::inverse(mvp));

			// Light space matrices
			// From ligh	t space to shadow map screen space
			float SizeX = *m_pAreaLight->getSizeX() * 0.5f;
			float SizeY = *m_pAreaLight->getSizeY() * 0.5f;

			glm::mat4 projectionLight = glm::frustum(-SizeX, SizeX, -SizeY, SizeY, 0.1f, 1000.0f);
			// From world to light
			glm::mat4 worldToLight = glm::lookAt(m_pAreaLight->getPosition(), m_pAreaLight->getPosition() + m_pAreaLight->getDirection(), glm::vec3(0.f, 0.f, -1.f));
			// From world to shadow map screen space 
			glm::mat4 worldToLightScreen = projectionLight * worldToLight;

			// Clear the front buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Default states
			glEnable(GL_DEPTH_TEST);

			// RENDER GBUFFEROBJECT
			m_pScene->addObject(m_pAreaLight);
			m_pGbuffer->render(m_pScene, projection, worldToView, objectToWorld);

			// RENDER SHADOW
			m_pScene->removeLastObjectAdd();
			m_pShadowBuffer->render(m_pScene, projectionLight, worldToLight, objectToWorld);

			// RENDER ILLUMINATION
			m_pIllumination->addTextureToDraw(m_pGbuffer->getTextureId(0), 0);
			m_pIllumination->addTextureToDraw(m_pGbuffer->getTextureId(1), 1);
			m_pIllumination->addTextureToDraw(m_pGbuffer->getTextureId(2), 2);
			m_pIllumination->addTextureToDraw(m_pShadowBuffer->getTexture(), 3);
			m_pIllumination->addTextureToDraw(m_pAreaLight->getTexture(), 4);
			m_pIllumination->render(m_pQuadBlit, vCameraPosition, screenToWorld, worldToLightScreen);

#if BLITRENDER
			// RENDER BLIT QUADS
			m_pBlit->addTextureToDraw(m_pGbuffer->getTextureId(0), 0);
			m_pBlit->addTextureToDraw(m_pGbuffer->getTextureId(1), 1);
			m_pBlit->addTextureToDraw(m_pGbuffer->getTextureId(2), 2);
			m_pBlit->render(m_pQuadBlit);
#endif

#if UI
			// Draw UI
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glViewport(0, 0, core::Config::WINDOW_WIDTH, core::Config::WINDOW_HEIGHT);

			unsigned char mbut = 0;
			int mscroll = 0;
			double mousex; double mousey;
			glfwGetCursorPos(m_pWindow, &mousex, &mousey);
			mousey = core::Config::WINDOW_HEIGHT - mousey;

			if (leftButton == GLFW_PRESS)
				mbut |= IMGUI_MBUT_LEFT;

			imguiBeginFrame((int)mousex, (int)mousey, mbut, mscroll);
			int logScroll = 0;
			imguiBeginScrollArea("Light", core::Config::WINDOW_WIDTH - 210, core::Config::WINDOW_HEIGHT - 310, 200, 300, &logScroll);
			
			imguiSlider("Light diffuse", m_pAreaLight->getDiffuseIntensityPtr(), 0.0f, 1000.0f, 1.0f);
			imguiSlider("Light specular", m_pAreaLight->getSpecularIntensityPtr(), 0.0f, 100.0f, 1.0f);
			imguiSlider("Light Size X", m_pAreaLight->getSizeX(), 0.0f, 100.0f, 1.0f);
			imguiSlider("Light Size Y", m_pAreaLight->getSizeY(), 0.0f, 100.0f, 1.0f);
			imguiSlider("Light Position X", m_pAreaLight->getPositionAxisValue(0), -15.0f, 15.0f, 0.1f);
			imguiSlider("Light Position Y", m_pAreaLight->getPositionAxisValue(1), -15.0f, 15.0f, 0.1f);
			imguiSlider("Light Position Z", m_pAreaLight->getPositionAxisValue(2), -15.0f, 15.0f, 0.1f);
			imguiSlider("Light Angle X", m_pAreaLight->getAngleX(), -M_PI, M_PI, 0.1f);
			imguiSlider("Light Angle Y", m_pAreaLight->getAngleY(), -M_PI, M_PI, 0.1f);
			imguiSlider("Light Angle Z", m_pAreaLight->getAngleZ(), -M_PI, M_PI, 0.1f);
			imguiSlider("Light Distance", m_pAreaLight->getDistance(), 0.0f, 150.f, 0.1f);
			imguiEndScrollArea();
			imguiEndFrame();
			imguiRenderGLDraw(core::Config::WINDOW_WIDTH, core::Config::WINDOW_HEIGHT);

			glDisable(GL_BLEND);
#endif

			glfwSwapBuffers(m_pWindow);
			glfwPollEvents();

			m_pAreaLight->updateMatrix();

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
