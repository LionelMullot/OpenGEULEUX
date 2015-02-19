
#include "GBufferRenderer.h"

#include "utils/ShaderProgram.h"
#include "Core/Config.h"
#include "WorldObject/Scene.h"

#include <cstdio>
#include <cassert>
#include <iostream>

namespace renderer
{
	GBufferRenderer * GBufferRenderer::create_ptr(void)
	{
		GBufferRenderer * ptr = new GBufferRenderer();
		ptr->init();
		return ptr;
	}

	void GBufferRenderer::release_ptr(GBufferRenderer * ptr)
	{
#ifndef NDEBUG
		assert(ptr != NULL);
#endif
		ptr->release();
		delete ptr;
	}

	GBufferRenderer::GBufferRenderer(void)
		: m_glGBufferObject(0)
		, m_pShaderProgram(NULL)
		, m_glMvpMatrixLocation(0)
		, m_glDiffuseTextureLocation(0)
		, m_glSpecularTextureLocation(0)
		, m_glSpecularPowerLocation(0)
	{}


	GBufferRenderer::~GBufferRenderer(void)
	{}

	void GBufferRenderer::init(void)
	{
		m_pShaderProgram = utils::ShaderProgram::create_ptr();
		m_pShaderProgram->addShader(GL_VERTEX_SHADER, "../../glsl/gBuffer.vert");
		m_pShaderProgram->addShader(GL_FRAGMENT_SHADER, "../../glsl/gBuffer.frag");

		std::string logInfo;
		if (!m_pShaderProgram->compileAndLinkShaders(logInfo))
		{
			throw std::runtime_error(logInfo);
		}

		m_glMvpMatrixLocation = m_pShaderProgram->getUniformIndex("MVP");
		m_glDiffuseTextureLocation = m_pShaderProgram->getUniformIndex("Diffuse");
		m_glSpecularTextureLocation = m_pShaderProgram->getUniformIndex("Specular");
		m_glSpecularPowerLocation = m_pShaderProgram->getUniformIndex("SpecularPower");

		// Generate 3 textures for GBufferObject
		glGenTextures(3, m_glGBufferObjectTextures);

		// Create material texture
		glBindTexture(GL_TEXTURE_2D, m_glGBufferObjectTextures[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, core::Config::WINDOW_WIDTH, core::Config::WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Create normal texture
		glBindTexture(GL_TEXTURE_2D, m_glGBufferObjectTextures[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, core::Config::WINDOW_WIDTH, core::Config::WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Create depth texture
		glBindTexture(GL_TEXTURE_2D, m_glGBufferObjectTextures[2]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, core::Config::WINDOW_WIDTH, core::Config::WINDOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Create FrameBufferObject
		glGenFramebuffers(1, &m_glGBufferObject);
		// Bind FrameBufferObject
		glBindFramebuffer(GL_FRAMEBUFFER, m_glGBufferObject);

		// Initialize DrawBuffers
		m_glGBufferObjectDrawTextures[0] = GL_COLOR_ATTACHMENT0;
		m_glGBufferObjectDrawTextures[1] = GL_COLOR_ATTACHMENT1;
		glDrawBuffers(2, m_glGBufferObjectDrawTextures);
		
		// Attach textures to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_glGBufferObjectTextures[0], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_glGBufferObjectTextures[1], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_glGBufferObjectTextures[2], 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			fprintf(stderr, "Error on building framebuffer\n");
			exit(EXIT_FAILURE);
		}

		// Back to the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GBufferRenderer::release(void)
	{
		if (m_pShaderProgram)
		{
			utils::ShaderProgram::release_ptr(m_pShaderProgram);	
		}
	}

	void GBufferRenderer::render(const worldObject::Scene * p_pScene, const glm::mat4& p_MVP)
	{
		if (m_pShaderProgram)
		{
			// Bind GBufferObject
			glBindFramebuffer(GL_FRAMEBUFFER, m_glGBufferObject);
			// Clear the GBufferObject
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Use ShaderProgram
			m_pShaderProgram->use();

			// Upload uniforms
			m_pShaderProgram->setUniform(m_glMvpMatrixLocation, p_MVP);
			m_pShaderProgram->setUniform(m_glDiffuseTextureLocation, 0);
			m_pShaderProgram->setUniform(m_glSpecularTextureLocation, 1);
			m_pShaderProgram->setUniform(m_glSpecularPowerLocation, 30.f);
			
			// Render scene 
			p_pScene->draw();

			// Get back to the default FrameBufferObject
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}