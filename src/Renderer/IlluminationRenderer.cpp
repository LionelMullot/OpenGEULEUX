
#include "IlluminationRenderer.h"

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
	IlluminationRenderer * IlluminationRenderer::create_ptr(worldObject::AreaLight * p_pAreaLight)
	{
		IlluminationRenderer * ptr = new IlluminationRenderer();
		ptr->init(p_pAreaLight);
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
		, m_pAreaLight(NULL)
		, m_glColorBufferLocation(0)
		, m_glNormalBufferLocation(0)
		, m_glDepthBufferLocation(0)
		, m_glShadowTextureLocation(0)
		, m_glLightTextureLocation(0)
		, m_glScreenToWorldLocation(0)
		, m_glWorldToLightScreenLocation(0)
		, m_glCameraViewLocation(0)
		, m_glAreaLightPositionLocation(0)
		, m_glAreaLightDirectionLocation(0)
		, m_glAreaLightUpLocation(0)
		, m_glAreaLightRightLocation(0)
		, m_glAreaLightSizeLocation(0)
		, m_glAreaLightColorLocation(0)
		, m_glAreaLightDiffuseIntensityLocation(0)
		, m_glAreaLightSpecularIntensityLocation(0)
	{}


	IlluminationRenderer::~IlluminationRenderer(void)
	{}

	void IlluminationRenderer::init(worldObject::AreaLight * p_pAreaLight)
	{
		m_pShaderProgram = utils::ShaderProgram::create_ptr();
		m_pShaderProgram->addShader(GL_VERTEX_SHADER, "../../glsl/blit.vert");
		m_pShaderProgram->addShader(GL_FRAGMENT_SHADER, "../../glsl/light.frag");

		std::string logInfo;
		if (!m_pShaderProgram->compileAndLinkShaders(logInfo))
		{
			throw std::runtime_error(logInfo);
		}

		m_pAreaLight = p_pAreaLight;

		m_glColorBufferLocation = m_pShaderProgram->getUniformIndex("ColorBuffer");
		m_glNormalBufferLocation = m_pShaderProgram->getUniformIndex("NormalBuffer");
		m_glDepthBufferLocation = m_pShaderProgram->getUniformIndex("DepthBuffer");
		m_glShadowTextureLocation = m_pShaderProgram->getUniformIndex("ShadowTexture");
		m_glLightTextureLocation = m_pShaderProgram->getUniformIndex("LightTexture");
		m_glScreenToWorldLocation = m_pShaderProgram->getUniformIndex("ScreenToWorld");
		m_glWorldToLightScreenLocation = m_pShaderProgram->getUniformIndex("WorldToLightScreen");
		m_glCameraViewLocation = m_pShaderProgram->getUniformIndex("CameraView");
		m_glAreaLightPositionLocation = m_pShaderProgram->getUniformIndex("AreaLightPosition");
		m_glAreaLightDirectionLocation = m_pShaderProgram->getUniformIndex("AreaLightDirection");
		m_glAreaLightUpLocation = m_pShaderProgram->getUniformIndex("AreaLightUp");
		m_glAreaLightRightLocation = m_pShaderProgram->getUniformIndex("AreaLightRight");
		m_glAreaLightSizeLocation = m_pShaderProgram->getUniformIndex("AreaLightSize");
		m_glAreaLightColorLocation = m_pShaderProgram->getUniformIndex("AreaLightColor");
		m_glAreaLightDiffuseIntensityLocation = m_pShaderProgram->getUniformIndex("AreaLightDiffuseIntensity");
		m_glAreaLightSpecularIntensityLocation = m_pShaderProgram->getUniformIndex("AreaLightSpecularIntensity");
	}

	void IlluminationRenderer::release(void)
	{
		if (m_pShaderProgram)
		{
			utils::ShaderProgram::release_ptr(m_pShaderProgram);
		}
		if (m_pAreaLight)
		{
			m_pAreaLight = NULL;
		}
	}

	void IlluminationRenderer::render(const utils::QuadBlit * p_pQuadBlit, const glm::vec3& vCameraPosition, const glm::mat4& screenToWorld, const glm::mat4& worldToLightScreen)
	{
		if (m_pShaderProgram)
		{
			// Disable the depth test
			glDisable(GL_DEPTH_TEST);
			// Enable blending
			glEnable(GL_BLEND);
			// Setup additive blending
			glBlendFunc(GL_ONE, GL_ONE);

			glViewport(0, 0, core::Config::WINDOW_WIDTH, core::Config::WINDOW_HEIGHT);

			// Use shader program 
			m_pShaderProgram->use();
			// Upload uniforms
			m_pShaderProgram->setUniform(m_glColorBufferLocation, 0);
			m_pShaderProgram->setUniform(m_glNormalBufferLocation, 1);
			m_pShaderProgram->setUniform(m_glDepthBufferLocation, 2);
			m_pShaderProgram->setUniform(m_glShadowTextureLocation, 3);
			m_pShaderProgram->setUniform(m_glLightTextureLocation, 4);
			m_pShaderProgram->setUniform(m_glCameraViewLocation, vCameraPosition);
			m_pShaderProgram->setUniform(m_glScreenToWorldLocation, screenToWorld);
			m_pShaderProgram->setUniform(m_glWorldToLightScreenLocation, worldToLightScreen);

			// Upload uniforms for area light
			if (m_pAreaLight)
			{
				m_pShaderProgram->setUniform(m_glAreaLightPositionLocation, m_pAreaLight->getPosition());
				m_pShaderProgram->setUniform(m_glAreaLightDirectionLocation, m_pAreaLight->getDirection());
				m_pShaderProgram->setUniform(m_glAreaLightUpLocation, m_pAreaLight->getUp());
				m_pShaderProgram->setUniform(m_glAreaLightRightLocation, m_pAreaLight->getRight());
				m_pShaderProgram->setUniform(m_glAreaLightSizeLocation, m_pAreaLight->getSize());
				m_pShaderProgram->setUniform(m_glAreaLightColorLocation, m_pAreaLight->getColor());
				m_pShaderProgram->setUniform(m_glAreaLightDiffuseIntensityLocation, m_pAreaLight->getDiffuseIntensity());
				m_pShaderProgram->setUniform(m_glAreaLightSpecularIntensityLocation, m_pAreaLight->getSpecularIntensity());
			}

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_tTexture[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_tTexture[1]);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_tTexture[2]);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, m_tTexture[3]);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, m_tTexture[4]);

			p_pQuadBlit->draw();

			// Disable blending
			glDisable(GL_BLEND);
		}
	}

	void IlluminationRenderer::addTextureToDraw(GLuint p_idTextureToDraw, int index)
	{
		m_tTexture[index] = p_idTextureToDraw;
	}
}
