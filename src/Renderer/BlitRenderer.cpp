
#include "BlitRenderer.h"

#include "utils/ShaderProgram.h"
#include "Utils/QuadBlit.h"
#include "Core/Config.h"

#include <cstdio>
#include <cassert>
#include <iostream>

namespace renderer
{
	BlitRenderer * BlitRenderer::create_ptr(void)
	{
		BlitRenderer * ptr = new BlitRenderer();
		ptr->init();
		return ptr;
	}

	void BlitRenderer::release_ptr(BlitRenderer * ptr)
	{
#ifndef NDEBUG
		assert(ptr != NULL);
#endif
		ptr->release();
		delete ptr;
	}

	BlitRenderer::BlitRenderer(void)
		: m_pShaderProgram(NULL)
		, m_glTextureLocation(0)
	{}


	BlitRenderer::~BlitRenderer(void) {}

	void BlitRenderer::init(void)
	{
		m_pShaderProgram = utils::ShaderProgram::create_ptr();
		m_pShaderProgram->addShader(GL_VERTEX_SHADER, "../../glsl/blit.vert");
		m_pShaderProgram->addShader(GL_FRAGMENT_SHADER, "../../glsl/blit.frag");

		std::string logInfo;
		if (!m_pShaderProgram->compileAndLinkShaders(logInfo))
		{
			throw std::runtime_error(logInfo);
		}
		std::cout << logInfo << std::endl;

		m_glTextureLocation = m_pShaderProgram->getUniformIndex("Texture");
	}

	void BlitRenderer::release(void)
	{
		if (m_pShaderProgram)
		{
			utils::ShaderProgram::release_ptr(m_pShaderProgram);
		}
	}

	void BlitRenderer::render(const utils::QuadBlit * p_pQuadBlit)
	{
		//Render in default framebuffer
		if (m_pShaderProgram)
		{
			glDisable(GL_DEPTH_TEST);
			// Use ShaderProgram
			m_pShaderProgram->use();
			// Upload uniforms
			m_pShaderProgram->setUniform(m_glTextureLocation, 0);
			
			// Render scene
			glActiveTexture(GL_TEXTURE0);
			
			const int width = core::Config::WINDOW_WIDTH;
			const int height = core::Config::WINDOW_HEIGHT;

			for (int i = 0; i <= nbFBO; ++i)
			{
				glViewport(i * width / nbFBO, 0, width / nbFBO, height / nbFBO);
				glBindTexture(GL_TEXTURE_2D, m_tTexture[i]);
				//Draw Quad 
				p_pQuadBlit->draw();
			}

			// Revert to window size viewport
			glViewport(0, 0, width, height);
		}
	}

	void BlitRenderer::addTextureToDraw(GLuint p_idTextureToDraw, int index)
	{
		assert(index <= nbFBO);
		m_tTexture[index] = p_idTextureToDraw;
	}
}
