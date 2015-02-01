#include "ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <iostream>

#include <fstream>
#include <sstream>
#include <string>

namespace utils
{

	ShaderProgram * ShaderProgram::create_ptr(void)
	{
		ShaderProgram * ptr = new ShaderProgram();
		ptr->init();
		return ptr;
	}

	void ShaderProgram::release_ptr(ShaderProgram * ptr)
	{
#ifndef NDEBUG
		assert(ptr != NULL);
#endif
		ptr->release();
		delete ptr;
	}

	ShaderProgram::ShaderProgram()
		: m_programId(0)
	{}

	ShaderProgram::~ShaderProgram() {}

	ShaderProgram::ShaderProgram(ShaderProgram&& rvalue) 
		: m_programId(rvalue.m_programId) 
	{
		rvalue.m_programId = 0;
		for (size_t i = 0; i < rvalue.m_vShadersList.size(); ++i)
		{
			m_vShadersList.push_back(rvalue.m_vShadersList[i]);
			rvalue.m_vShadersList[i] = nullptr;
		}
	}

	ShaderProgram& ShaderProgram::operator =(ShaderProgram&& rvalue) 
	{
		m_programId = rvalue.m_programId;
		rvalue.m_programId = 0;
		for (size_t i = 0; i < rvalue.m_vShadersList.size(); ++i)
		{
			m_vShadersList.push_back(rvalue.m_vShadersList[i]);
			rvalue.m_vShadersList[i] = nullptr;
		}
		return *this;
	}

	void ShaderProgram::init(void)
	{
		m_programId = glCreateProgram();
		std::cout << "ShaderProgram id " << m_programId << "create" << std::endl;
	}

	void ShaderProgram::release(void)
	{
		//Properly delete shaders hosted
		std::vector<Shader* >::iterator currentShader;
		for (currentShader = m_vShadersList.begin(); currentShader != m_vShadersList.end(); ++currentShader)
		{
			Shader::release_ptr(*currentShader);
		}

		glDeleteProgram(m_programId);
		std::cout << "ShaderProgram id " << m_programId << "destroy" << std::endl;
	}

	void ShaderProgram::addShader(GLenum shaderType, const std::string& shaderFilePath)
	{
		std::string shaderSource;
		//RAII
		{
			std::ifstream fileStream;
			std::stringstream buffer;

			fileStream.open(shaderFilePath.c_str(), std::ifstream::in);
			if (!fileStream)
			{
				throw std::runtime_error("Cannot find : " + shaderFilePath);
			}

			buffer << fileStream.rdbuf();
			shaderSource = buffer.str();
		}

		Shader* newShader = Shader::create_ptr(shaderType, shaderFilePath);

		newShader->setSource(shaderSource.c_str());
		m_vShadersList.push_back(newShader);
	}

	bool ShaderProgram::compileAndLinkShaders(std::string &logInfo) const
	{
		std::vector<Shader* >::const_iterator currentShader;
		for (currentShader = m_vShadersList.begin(); currentShader != m_vShadersList.end(); ++currentShader)
		{
			if (!(*currentShader)->compile(logInfo))
				return false;
			glAttachShader(m_programId, (*currentShader)->getGLId());
		}

		glLinkProgram(m_programId);

		GLint linkStatus;
		glGetProgramiv(m_programId, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == GL_FALSE) {
			GLint logLength;
			glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &logLength);

			char log[800];

			glGetProgramInfoLog(m_programId, logLength, 0, log);
			logInfo.append(log);
			return false;
		}
		for (currentShader = m_vShadersList.begin(); currentShader
			!= m_vShadersList.end(); ++currentShader)
			glDetachShader(m_programId, (*currentShader)->getGLId());
		return true;
	}

	void ShaderProgram::use() const
	{
		glUseProgram(m_programId);
	}

	GLint ShaderProgram::getUniformIndex(const std::string &uniformName) const
	{
		return glGetUniformLocation(m_programId, uniformName.c_str());
	}

	void ShaderProgram::setUniform(GLint uniformIndex, const glm::mat3 &matrix) const
	{
		glUniformMatrix3fv(uniformIndex, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void ShaderProgram::setUniform(GLint uniformIndex, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(uniformIndex, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void ShaderProgram::setUniform(GLint uniformIndex, const glm::vec4& vector) const
	{
		glUniform4f(uniformIndex, vector.x, vector.y, vector.z, vector.w);
	}

	void ShaderProgram::setUniform(GLint uniformIndex, const glm::vec3& vector) const
	{
		glUniform3fv(uniformIndex, 1, glm::value_ptr(vector));
	}

	void ShaderProgram::setUniform(GLint uniformIndex, float scalar) const
	{
		glUniform1f(uniformIndex, scalar);
	}

	void ShaderProgram::setUniform(GLint uniformIndex, int scalar) const
	{
		glUniform1i(uniformIndex, scalar);
	}

	void ShaderProgram::setUniform(GLint uniformIndex, bool value) const
	{
		glUniform1i(uniformIndex, value);
	}

	ShaderProgram ShaderProgram::loadProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	{
		std::string vertexShaderSource;
		std::string fragmentShaderSource;
		{
			std::ifstream fileStreamVertexShader, fileStreamFragmentShader;
			std::stringstream bufferVertexShader, bufferFragmentShader;

			fileStreamVertexShader.open(vertexShaderFilePath.c_str(), std::ifstream::in);
			fileStreamFragmentShader.open(fragmentShaderFilePath.c_str(), std::ifstream::in);

			if (!fileStreamVertexShader || !fileStreamFragmentShader)
			{
				throw std::runtime_error("Cannont find : " + vertexShaderFilePath + "; " + fragmentShaderFilePath);
			}

			bufferVertexShader << fileStreamVertexShader.rdbuf();
			bufferFragmentShader << fileStreamFragmentShader.rdbuf();

			vertexShaderSource = bufferVertexShader.str();
			fragmentShaderSource = bufferFragmentShader.str();
		}

		ShaderProgram program;
		program.addShader(GL_VERTEX_SHADER, vertexShaderSource);
		program.addShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

		return program;
	}
} // namespace utils
