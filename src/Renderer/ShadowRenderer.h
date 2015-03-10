
#pragma once
#ifndef __RENDERER_SHADOW_H__
#define __RENDERER_SHADOW_H__

#include <GL/glew.h>
#include <glm/matrix.hpp>

namespace utils
{
	class ShaderProgram;
}

namespace worldObject
{
	class Scene;
	class AreaLight;
}

namespace renderer
{
	class ShadowRenderer
	{
	const int RESOLUTION = 4096;

		/* ***************************************************** */
		/* ********************** MEMBERS ********************** */
		/* ***************************************************** */
	protected:
		GLuint						m_glBufferObject;
		utils::ShaderProgram *		m_pShaderProgram;
		GLuint						m_glRenderBuffer;
		GLuint						m_glTexture;
		// Uniforms
		GLuint	m_glProjectionMatrixLocation;
		GLuint	m_glWorldToViewMatrixLocation;

		/* ***************************************************** */
		/* ************* CONSTUCTION AND DESTUCTION ************ */
		/* ***************************************************** */
	public:
		static ShadowRenderer * create_ptr(void);
		static void release_ptr(ShadowRenderer * ptr);

	protected:
		ShadowRenderer(void);
		~ShadowRenderer(void);

		void init(void);
		void release(void);

		/* ***************************************************** */
		/* ********************** METHODS ********************** */
		/* ***************************************************** */
	public:
		void render(const worldObject::Scene * p_pScene, const glm::mat4& p_mProjection, const glm::mat4& p_mWorldToView, glm::mat4& p_mObjectToWorld);

		/* ***************************************************** */
		/* *************** GETTER / SETTER ********************* */
		/* ***************************************************** */
		const GLuint getTexture() const;

	}; // class ShadowRenderer

	inline const GLuint ShadowRenderer::getTexture() const { return m_glTexture; }

}; // namespace renderer

#endif //__RENDERER_SHADOW_H__
