
#pragma once
#ifndef __CORE_APP_H__
#define __CORE_APP_H__

#include <cstdint>

#include <GLFW\glfw3.h>

namespace utils
{
	class Camera;
}

namespace core
{
	class GUIState;

	// Singleton class
	class App
	{

	static const int MAX_NB_SHADERS = 3;

	private:
		static App *		m_p_instance;
		double				m_t;
		double				m_fps;
		GLFWwindow *		m_pWindow;
		utils::Camera *		m_pCamera;
		core::GUIState *	m_pGUIState;
		GLuint				m_pVertexShadersID[MAX_NB_SHADERS];
		GLuint				m_pFragmentShadersID[MAX_NB_SHADERS];
		GLuint				m_programObject;
	
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

		/* Shaders utils */
		static int check_link_error(GLuint program);
		static int check_compile_error(GLuint shader, const char ** sourceBuffer);
		static GLuint compile_shader(GLenum shaderType, const char * sourceBuffer, int bufferSize);
		static GLuint compile_shader_from_file(GLenum shaderType, const char * fileName);

	}; // class App

} // namespace core

#endif // __CORE_APP_H__
