#include "Shader.h"

#include <fstream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include <cassert>

namespace utils 
{

	Shader * Shader::create_ptr(GLenum type, const std::string name)
	{
		Shader * ptr = new Shader();
		ptr->init(type, name);
		return ptr;
	}

	void Shader::release_ptr(Shader * ptr)
	{
#ifndef NDEBUG
		assert(ptr != NULL);
#endif
		ptr->release();
		delete ptr;
	}

	Shader::Shader(void)
	  : m_GLId(0), m_sName("")
	{}

	Shader::~Shader(void) {}

	Shader::Shader(Shader&& rvalue) 
		: m_GLId(rvalue.m_GLId) 
	{
		rvalue.m_GLId = 0;
	}

	Shader& Shader::operator=(Shader&& rvalue)
	{
		m_GLId = rvalue.m_GLId;
		rvalue.m_GLId = 0;
		return *this;
	}

	void Shader::init(GLenum type, const std::string name)
	{
		m_GLId = glCreateShader(type);
		m_sName = name;

		std::cout << "Shader id" << m_GLId << " create" << std::endl;
	}

	void Shader::release(void)
	{
		glDeleteShader(m_GLId);

		std::cout << "Shader id" << m_GLId << " destroy" << std::endl;
	}

	bool Shader::compile(std::string& infoLog) 
	{
		glCompileShader(m_GLId);
		GLint status;
		glGetShaderiv(m_GLId, GL_COMPILE_STATUS, &status);
		GLint length;
		glGetShaderiv(m_GLId, GL_INFO_LOG_LENGTH, &length);
		if (length > 1)
		{
			char* log = new char[length];
			glGetShaderInfoLog(m_GLId, length, 0, log);
			infoLog.append("Error in " + m_sName + + "\n");
			infoLog.append(log);

			delete [] log;
		}
		return status == GL_TRUE;
	}

	const std::string Shader::getInfoLog(void) const 
	{
		GLint length;
		glGetShaderiv(m_GLId, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetShaderInfoLog(m_GLId, length, 0, log);
		std::string logString("Error in " + m_sName + "\n" + log);
		delete [] log;
		return logString;
	}

	GLuint Shader::getGLId(void) const
	{
		return m_GLId;
	}

	void Shader::setSource(const char* src)
	{
		glShaderSource(m_GLId, 1, &src, 0);
	}

	Shader * loadShader(GLenum type, const char* filename) {
		std::ifstream input(filename);
		if(!input) {
			throw std::runtime_error("Unable to load the file " + std::string(filename));
		}
    
		std::stringstream buffer;
		buffer << input.rdbuf();
    
		Shader * shader = Shader::create_ptr(type, filename);
		shader->setSource(buffer.str().c_str());

		return shader;
	}

} // namespace utils

