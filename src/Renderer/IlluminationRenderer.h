
#pragma once
#ifndef __RENDERER_ILLUMINATION_H__
#define __RENDERER_ILLUMINATION_H__

#include <GL/glew.h>
#include <glm/matrix.hpp>

namespace utils
{
	class ShaderProgram;
	class QuadBlit;
}

namespace worldObject
{
	class Scene;
	class AreaLight;
}

namespace renderer
{
	class IlluminationRenderer
	{
		/* ***************************************************** */
		/* ********************** MEMBERS ********************** */
		/* ***************************************************** */
	protected:
		utils::ShaderProgram *		m_pShaderProgram;
		GLuint						m_tTexture[5];
		worldObject::AreaLight *	m_pAreaLight;
		// Uniforms
		GLuint	m_glColorBufferLocation;
		GLuint	m_glNormalBufferLocation;
		GLuint	m_glDepthBufferLocation;
		GLuint	m_glShadowTextureLocation;
		GLuint  m_glLightTextureLocation;
		GLuint	m_glScreenToWorldLocation;
		GLuint	m_glWorldToLightScreenLocation;
		GLuint	m_glCameraViewLocation;
		// Area Light Uniforms
		GLuint	m_glAreaLightPositionLocation;
		GLuint	m_glAreaLightDirectionLocation;
		GLuint	m_glAreaLightUpLocation;
		GLuint	m_glAreaLightRightLocation;
		GLuint	m_glAreaLightSizeLocation;
		GLuint	m_glAreaLightColorLocation;
		GLuint	m_glAreaLightDiffuseIntensityLocation;
		GLuint	m_glAreaLightSpecularIntensityLocation;

		/* ***************************************************** */
		/* ************* CONSTUCTION AND DESTUCTION ************ */
		/* ***************************************************** */
	public:
		static IlluminationRenderer * create_ptr(worldObject::AreaLight * p_pAreaLight);
		static void release_ptr(IlluminationRenderer * ptr);

	protected:
		IlluminationRenderer(void);
		~IlluminationRenderer(void);

		void init(worldObject::AreaLight * p_pAreaLight);
		void release(void);

		/* ***************************************************** */
		/* ********************** METHODS ********************** */
		/* ***************************************************** */
	public:
		void render(const utils::QuadBlit * p_pQuadBlit, const glm::vec3& vCameraPosition, const glm::mat4& screenToWorld, const glm::mat4& worldToLightScreen);

		/* ***************************************************** */
		/* *************** GETTER / SETTER ********************* */
		/* ***************************************************** */
		void addTextureToDraw(GLuint p_idTextureToDraw, int index);

	}; // class IlluminationRenderer

}; // namespace renderer

#endif //__RENDERER_ILLUMINATION_H__
