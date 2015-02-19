
#pragma once
#ifndef __RENDERER_GBUFFER_H__
#define __RENDERER_GBUFFER_H__

#include <GL/glew.h>
#include <glm/matrix.hpp>

namespace utils
{
	class ShaderProgram;
}

namespace worldObject
{
	class Scene;
}

namespace renderer
{
	class GBufferRenderer
	{

		/* ***************************************************** */
		/* ********************** MEMBERS ********************** */
		/* ***************************************************** */
	protected:
		GLuint					m_glGBufferObject;
		GLuint					m_glGBufferObjectTextures[3];
		GLuint					m_glGBufferObjectDrawTextures[2];
		utils::ShaderProgram *	m_pShaderProgram;

		// Uniforms
		GLuint	m_glMvpMatrixLocation;
		GLuint	m_glDiffuseTextureLocation;
		GLuint	m_glSpecularTextureLocation;
		GLuint	m_glSpecularPowerLocation;

		/* ***************************************************** */
		/* ************* CONSTUCTION AND DESTUCTION ************ */
		/* ***************************************************** */
	public:
		static GBufferRenderer * create_ptr(void);
		static void release_ptr(GBufferRenderer * ptr);

	protected:
		GBufferRenderer(void);
		~GBufferRenderer(void);

		void init(void);
		void release(void);

		/* ***************************************************** */
		/* ********************** METHODS ********************** */
		/* ***************************************************** */
	public:
		void render(const worldObject::Scene * p_pScene, const glm::mat4& p_mvp);

		/* ***************************************************** */
		/* *************** GETTER / SETTER ********************* */
		/* ***************************************************** */
		GLuint getTextureId(int index);

	}; // class GBufferRenderer

	inline GLuint GBufferRenderer::getTextureId(int index){ return m_glGBufferObjectTextures[index]; }

}; // namespace renderer

#endif //__RENDERER_GBUFFER_H__
