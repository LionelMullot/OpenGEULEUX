
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
		/**
		 * @brief loadProgram créé un ShaderProgram constitué des deux shaders décrits par les fichiers
		 * dont le chemin d'accès est spécifié en paramètre.
		 * Attention : cette méthode propage une std::runtime_error si un ou les fichiers n'existent pas.
		 * @param vertexShaderFilePath
		 * @param fragmentShaderFilePath
		 * @return le ShaderProgram contenant les deux shaders
		 */
		static ShaderProgram loadProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		/**
		 * @brief addShader ajoute un shader décrit par le fichier dont le chemin d'accès est
		 * spécifié en paramètre.
		 * Attention : cette méthode propage une std::runtime_error si un ou les fichiers n'existent pas.
		 * au ShaderProgram.
		 * @param shaderType
		 * @param shaderFilePath
		 */
		void addShader(GLenum shaderType, const std::string& shaderFilePath);
	  
		bool compileAndLinkShaders(std::string& logInfo) const;
		void use() const;

	/* ***************************************************** */
	/* *************** GETTER / SETTER ********************* */
	/* ***************************************************** */
	public:
		GLint getUniformIndex(const std::string& uniformName) const;

		/**
		 * @brief setUniform
		 * @param uniformIndex l'index de l'uniform obtenu par la méthode
		 * getUniformIndex(uniformName)
		 * @param matrix
		 * @see getUniformIndex(const std::string& uniformName)
		 */
		void setUniform(GLint uniformIndex, const glm::mat3& matrix) const;
		void setUniform(GLint uniformIndex, const glm::mat4& matrix) const;
		void setUniform(GLint uniformIndex, const glm::vec4& vector) const;
		void setUniform(GLint uniformIndex, const glm::vec3& vector) const;
		void setUniform(GLint uniformIndex, float scalar) const;
		void setUniform(GLint uniformIndex, int scalar) const;
		void setUniform(GLint uniformIndex, bool value) const;

		GLuint getId() const;

	}; // class ShaderProgram

	inline GLuint ShaderProgram::getId(void) const { return m_programId; }

} //namespace utils

#endif // __UTILS__SHADERPROGRAM_H__
