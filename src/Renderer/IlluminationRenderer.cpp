
#include "IlluminationRenderer.h"

#include "Utils/ShaderProgram.h"
#include "Utils/QuadBlit.h"
#include "Core/Config.h"
#include "WorldObject/Scene.h"

#include <cstdio>
#include <cassert>
#include <iostream>

namespace renderer
{
	IlluminationRenderer * IlluminationRenderer::create_ptr(void)
	{
		IlluminationRenderer * ptr = new IlluminationRenderer();
		ptr->init();
		return ptr;
	}

	void IlluminationRenderer::release_ptr(IlluminationRenderer * ptr)
	{
#ifndef NDEBUG
		assert(ptr != NULL);
#endif
		ptr->release();
		delete ptr;
	}

	IlluminationRenderer::IlluminationRenderer(void)
		: m_pShaderProgram(NULL)
		, m_glColorBufferLocation(0)
		, m_glNormalBufferLocation(0)
		, m_glDepthBufferLocation(0)
		, m_glScreenToWorldLocation(0)
		, m_glCameraViewLocation(0)
	{}


	IlluminationRenderer::~IlluminationRenderer(void)
	{}

	void IlluminationRenderer::init(void)
	{
		m_pShaderProgram = utils::ShaderProgram::create_ptr();
		m_pShaderProgram->addShader(GL_VERTEX_SHADER, "../../glsl/blit.vert");
		m_pShaderProgram->addShader(GL_FRAGMENT_SHADER, "../../glsl/light.frag");

		std::string logInfo;
		if (!m_pShaderProgram->compileAndLinkShaders(logInfo))
		{
			throw std::runtime_error(logInfo);
		}

		m_glColorBufferLocation = m_pShaderProgram->getUniformIndex("ColorBuffer");
		m_glNormalBufferLocation = m_pShaderProgram->getUniformIndex("NormalBuffer");
		m_glDepthBufferLocation = m_pShaderProgram->getUniformIndex("DepthBuffer");
		//m_glScreenToWorldLocation = m_pShaderProgram->getUniformIndex("ScreenToWorld");
		//m_glCameraViewLocation = m_pShaderProgram->getUniformIndex("CameraView");
	}

	void IlluminationRenderer::release(void)
	{
		if (m_pShaderProgram)
		{
			utils::ShaderProgram::release_ptr(m_pShaderProgram);
		}
	}

	void IlluminationRenderer::render(const utils::QuadBlit * p_pQuadBlit)
	{
		if (m_pShaderProgram)
		{
			// Disable the depth test
			glDisable(GL_DEPTH_TEST);
			// Enable blending
			glEnable(GL_BLEND);

			// Use shader program 
			m_pShaderProgram->use();
			// Upload uniforms
			m_pShaderProgram->setUniform(m_glColorBufferLocation, 0);
			m_pShaderProgram->setUniform(m_glNormalBufferLocation, 1);
			m_pShaderProgram->setUniform(m_glDepthBufferLocation, 2);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_tTexture[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_tTexture[1]);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_tTexture[2]);

			p_pQuadBlit->draw();

			// Disable blending
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		}
	}

	void IlluminationRenderer::addTextureToDraw(GLuint p_idTextureToDraw, int index)
	{
		m_tTexture[index] = p_idTextureToDraw;
	}
}
