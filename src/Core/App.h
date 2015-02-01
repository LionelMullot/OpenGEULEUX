
#pragma once
#ifndef __CORE_APP_H__
#define __CORE_APP_H__

#include <cstdint>

#include <GLFW\glfw3.h>

namespace utils
{
	class Camera;
	class ShaderProgram;
}

namespace core
{
	class GUIState;

	// Singleton class
	class App
	{
	
	private:
		static App *			m_p_instance;
		double					m_t;
		double					m_fps;
		GLFWwindow *			m_pWindow;
		utils::Camera *			m_pCamera;
		core::GUIState *		m_pGUIState;
		utils::ShaderProgram *	m_pShaderProgram;
	
	/* ***************************************************** */
	/* ************* CONSTUCTION AND DESTUCTION ************ */
	/* ***************************************************** */
	protected:
		App(void);
		virtual ~App(void);

	private:
		// Disable copy constructor
		App& operator= (const App& p_app){};
		App(const App& p_app){};

	/* ***************************************************** */
	/* ********************** METHODS ********************** */
	/* ***************************************************** */
	private:
		void initGLFW(void);
		void initWindow(void);
		void initGlew(void);
		void initCamera(void);
		void initGUIState(void);
		void initShaders(void);
		void initApp(int argc, char** argv);

		int32_t runApp(void);
		void exitApp(void);

	/* ***************************************************** */
	/* ********************** STATICS ********************** */
	/* ***************************************************** */
	public:
		static void init_app(int argc, char** argv);
		static int32_t run_app(void);
		static void exit_app(void);

	}; // class App

} // namespace core

#endif // __CORE_APP_H__
