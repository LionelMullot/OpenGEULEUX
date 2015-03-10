
#include "ShadowRenderer.h"

#include "Utils/ShaderProgram.h"
#include "Utils/QuadBlit.h"
#include "Core/Config.h"
#include "WorldObject/Scene.h"
#include "WorldObject/AreaLight.h"

#include <cstdio>
#include <cassert>
#include <iostream>

namespace renderer
{
	ShadowRenderer * ShadowRenderer::create_ptr(void)
	{
		ShadowRenderer * ptr = new ShadowRenderer();
		ptr->init();
		return ptr;
	}

	void ShadowRenderer::release_ptr(ShadowRenderer * ptr)
	{
#ifndef NDEBUG
		assert(ptr != NULL);
#endif
		ptr->release();
		delete ptr;
	}

	ShadowRenderer::ShadowRenderer(void)
		: m_glBufferObject(0)
		, m_pShaderProgram(NULL)
		, m_glRenderBuffer(0)
		, m_glTexture(0)
		, m_glProjectionMatrixLocation(0)
		, m_glWorldToViewMatrixLocation(0)
	{}


	ShadowRenderer::~ShadowRenderer(void)
	{}

	void ShadowRenderer::init(void)
	{
		m_pShaderProgram = utils::ShaderProgram::create_ptr();
		m_pShaderProgram->addShader(GL_VERTEX_SHADER, "../../glsl/gbuffer.vert");
		m_pShaderProgram->addShader(GL_FRAGMENT_SHADER, "../../glsl/shadow.frag");

		std::string logInfo;
		if (!m_pShaderProgram->compileAndLinkShaders(logInfo))
		{
			throw std::runtime_error(logInfo);
		}

		m_glProjectionMatrixLocation = m_pShaderProgram->getUniformIndex("Projection");
		m_glWorldToViewMatrixLocation = m_pShaderProgram->getUniformIndex("WorldToView");

		glGenFramebuffers(1, &m_glBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, m_glBufferObject);
		
		// Create a render buffer since we don't need to read shadow color 
		// in a texture
		glGenRenderbuffers(1, &m_glRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_glRenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, RESOLUTION, RESOLUTION);

		// Attach the renderBuffer
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_glRenderBuffer);
		
		glGenTextures(1, &m_glTexture);

		glBindTexture(GL_TEXTURE_2D, m_glTexture);
		// Create empty texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, RESOLUTION, RESOLUTION, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		// Bilinear filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Color needs to be 0 outside of texture coordinates
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		// Attach the shadow texture to the depth attachment
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_glTexture, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			fprintf(stderr, "Error on building shadow framebuffer\n");
			exit(EXIT_FAILURE);
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		// Fall back to default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void ShadowRenderer::release(void)
	{
		if (m_pShaderProgram)
		{
			utils::ShaderProgram::release_ptr(m_pShaderProgram);
		}
	}

	void ShadowRenderer::render(const worldObject::Scene * p_pScene, const glm::mat4& p_mProjection, const glm::mat4& p_mWorldToView, glm::mat4& p_mObjectToWorld)
	{
		if (m_pShaderProgram)
		{
			// Bind ShadowBufferObject
			glBindFramebuffer(GL_FRAMEBUFFER, m_glBufferObject);
			// Set the viewport corresponding to shadow texture resolution
			glViewport(0, 0, RESOLUTION, RESOLUTION);
			// Clear only the depth buffer
			glClear(GL_DEPTH_BUFFER_BIT);
			// Use shader program 
			m_pShaderProgram->use();

			// Upload uniforms
			m_pShaderProgram->setUniform(m_glProjectionMatrixLocation, p_mProjection);
			m_pShaderProgram->setUniform(m_glWorldToViewMatrixLocation, p_mWorldToView);

			p_pScene->draw(m_pShaderProgram, p_mObjectToWorld);

			// Fallback to default framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			// Revert to window size viewport
			glViewport(0, 0, core::Config::WINDOW_WIDTH, core::Config::WINDOW_HEIGHT);
		}
	}
}
