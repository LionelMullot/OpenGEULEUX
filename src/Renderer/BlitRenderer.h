
#pragma once
#ifndef __RENDERER_BLIT_H__
#define __RENDERER_BLIT_H__

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
	class BlitRenderer
	{
		/* ***************************************************** */
		/* ********************** MEMBERS ********************** */
		/* ***************************************************** */
	public:
		const static int nbFBO = 4;
	
	protected:
		GLuint					m_tTexture[nbFBO];
		utils::ShaderProgram *	m_pShaderProgram;

		// Uniforms
		GLuint	m_glTextureLocation;

		/* ***************************************************** */
		/* ************* CONSTUCTION AND DESTUCTION ************ */
		/* ***************************************************** */
	public:
		static BlitRenderer * create_ptr(void);
		static void release_ptr(BlitRenderer * ptr);

	protected:
		BlitRenderer(void);
		~BlitRenderer(void);

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

	}; // class BlitRenderer

}; // namespace renderer

#endif //__RENDERER_BLIT_H__
