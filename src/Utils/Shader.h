
#pragma once
#ifndef __UTILS_SHADER_H__
#define __UTILS_SHADER_H__

#include <GL/glew.h>
#include <string>

#define GLIMAC_SHADER_SRC(str) #str

namespace utils {

	class Shader 
	{

	/* ***************************************************** */
	/* ********************** MEMBERS ********************** */
	/* ***************************************************** */
	protected:
		GLuint				m_GLId;
		std::string			m_sName; // Simplify debug

	/* ***************************************************** */
	/* ************* CONSTUCTION AND DESTUCTION ************ */
	/* ***************************************************** */
	public:
		static Shader * create_ptr(GLenum type, const std::string name);
		static void release_ptr(Shader * ptr);

	protected:
		Shader(void);
		~Shader(void);

		void init(GLenum type, const std::string name);
		void release(void);

	public:
		Shader(Shader&& rvalue);
		Shader& operator =(Shader&& rvalue);

	private:
		// A Shader cannot be copied
		Shader(const Shader&);
		Shader& operator =(const Shader&);
	
	/* ***************************************************** */
	/* ********************** METHODS ********************** */
	/* ***************************************************** */
	public:
		bool compile(std::string& logInfo);

	/* ***************************************************** */
	/* *************** GETTER / SETTER ********************* */
	/* ***************************************************** */
		const std::string getInfoLog(void) const;
		GLuint getGLId(void) const;

		void setSource(const char* src);

	}; // class Shader

	// Load a shader (but does not compile it)
	Shader * loadShader(GLenum type, const char* filename);

} // namespace utils

#endif // __UTILS_SHADER_H__