
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
}

namespace renderer
{
	class IlluminationRenderer
	{

		/* ***************************************************** */
		/* ********************** MEMBERS ********************** */
		/* ***************************************************** */
	protected:
		utils::ShaderProgram *	m_pShaderProgram;
		GLuint					m_tTexture[3];
		// Uniforms
		GLuint	m_glColorBufferLocation;
		GLuint	m_glNormalBufferLocation;
		GLuint	m_glDepthBufferLocation;
		GLuint	m_glScreenToWorldLocation;
		GLuint	m_glCameraViewLocation;

		/* ***************************************************** */
		/* ************* CONSTUCTION AND DESTUCTION ************ */
		/* ***************************************************** */
	public:
		static IlluminationRenderer * create_ptr(void);
		static void release_ptr(IlluminationRenderer * ptr);

	protected:
		IlluminationRenderer(void);
		~IlluminationRenderer(void);

		void init(void);
		void release(void);

		/* ***************************************************** */
		/* ********************** METHODS ********************** */
		/* ***************************************************** */
	public:
		void render(const utils::QuadBlit * p_pQuadBlit);

		/* ***************************************************** */
		/* *************** GETTER / SETTER ********************* */
		/* ***************************************************** */
		void addTextureToDraw(GLuint p_idTextureToDraw, int index);

	}; // class IlluminationRenderer

}; // namespace renderer

#endif //__RENDERER_ILLUMINATION_H__
