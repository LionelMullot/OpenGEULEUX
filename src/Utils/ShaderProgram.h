
#pragma once
#ifndef __UTILS__SHADERPROGRAM_H__
#define __UTILS__SHADERPROGRAM_H__

#include <GL/glew.h>
#include <Utils/Shader.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace utils
{

	class ShaderProgram
	{
	
	/* ***************************************************** */
	/* ********************** MEMBERS ********************** */
	/* ***************************************************** */
	protected:
		GLuint					m_programId;
		std::vector<Shader* >	m_vShadersList;

	/* ***************************************************** */
	/* ************* CONSTUCTION AND DESTUCTION ************ */
	/* ***************************************************** */
	public:
		static ShaderProgram * create_ptr(void);
		static void release_ptr(ShaderProgram * ptr);

	protected:
		ShaderProgram(void);
		~ShaderProgram(void);

		void init(void);
		void release(void);

	public:
		ShaderProgram(ShaderProgram&& rvalue);
		ShaderProgram& operator =(ShaderProgram&& rvalue);

	private:
		// A ShaderProgram cannot be copied
		ShaderProgram(const ShaderProgram&);
		ShaderProgram& operator =(const ShaderProgram&);

	/* ***************************************************** */
	/* ********************** METHODS ********************** */
	/* ***************************************************** */
	public:
		static ShaderProgram loadProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
		void addShader(GLenum shaderType, const std::string& shaderFilePath);
	  
		bool compileAndLinkShaders(std::string& logInfo) const;
		void use() const;

	/* ***************************************************** */
	/* *************** GETTER / SETTER ********************* */
	/* ***************************************************** */
	public:
		GLuint getUniformIndex(const std::string& uniformName) const;

		void setUniform(GLuint uniformIndex, const glm::mat3& matrix) const;
		void setUniform(GLuint uniformIndex, const glm::mat4& matrix) const;
		void setUniform(GLuint uniformIndex, const glm::vec4& vector) const;
		void setUniform(GLuint uniformIndex, const glm::vec3& vector) const;
		void setUniform(GLuint uniformIndex, const glm::vec2& vector) const;
		void setUniform(GLuint uniformIndex, float scalar) const;
		void setUniform(GLuint uniformIndex, int scalar) const;
		void setUniform(GLuint uniformIndex, bool value) const;

		GLuint getId() const;

	}; // class ShaderProgram

	inline GLuint ShaderProgram::getId(void) const { return m_programId; }

} //namespace utils

#endif // __UTILS__SHADERPROGRAM_H__
