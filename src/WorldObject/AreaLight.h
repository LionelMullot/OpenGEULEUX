
#pragma once
#ifndef __WORLDOBJECT_AREALIGHT_H__
#define __WORLDOBJECT_AREALIGHT_H__

#include <cstdint>
#include <vector>

#include "Light.h"
#include <GL/glew.h>
#include <glm/vec2.hpp>

namespace worldObject
{
	// Create a unit cube center on the world origin
	class AreaLight : public Light
	{

	/* ***************************************************** */
	/* ********************** MEMBERS ********************** */
	/* ***************************************************** */
	protected:
		glm::mat4   m_mRotation;
		glm::mat4   m_mTranslation;
		glm::mat4   m_mScale;
		glm::mat4	m_mObjectModel;
		glm::vec3	m_vUp;
		glm::vec3	m_vRight;
		glm::vec2	m_vSize;
		float		m_fAngleX;
		float		m_fAngleY;
		float		m_fAngleZ;
		float		m_fDistance;
		GLuint		m_Textures[1];

	/* ***************************************************** */
	/* ************* CONSTUCTION AND DESTUCTION ************ */
	/* ***************************************************** */
	public:
		static AreaLight * create_ptr(void);
		static void release_ptr(AreaLight * ptr);

	protected:
		AreaLight(void);
		virtual ~AreaLight(void);

		virtual void init(void);
		virtual void release(void);

	/* ***************************************************** */
	/* ********************** METHODS ********************** */
	/* ***************************************************** */
	public:
		virtual void draw(const utils::ShaderProgram * p_pShaderProgram, const glm::mat4& p_mObjectToWorld) const;

		void initData(void);
		void updateMatrix(void);

	/* ***************************************************** */
	/* ****************** GETTER AND SETTER **************** */
	/* ***************************************************** */
	public:
		const glm::vec3& getUp(void) const;
		const glm::vec3& getRight(void) const;
		const glm::vec2 getSize(void) const;
		float * const getSizeX(void);
		float * const getSizeY(void);
		float * const getAngleX(void);
		float * const getAngleY(void);
		float * const getAngleZ(void);
		float * const getDistance(void);
		const GLuint getTexture(void) const;

	}; // class AreaLight

	inline const glm::vec3& AreaLight::getUp(void) const { return m_vUp; }
	inline const glm::vec3& AreaLight::getRight(void) const { return m_vRight; }
	inline const glm::vec2 AreaLight::getSize(void) const { return glm::vec2(m_vSize.x * .5, m_vSize.y * .5); }
	inline float * const AreaLight::getSizeX(void) { return &m_vSize.x; }
	inline float * const AreaLight::getSizeY(void) { return &m_vSize.y; }
	inline float * const AreaLight::getAngleX(void) { return &m_fAngleX; }
	inline float * const AreaLight::getAngleY(void) { return &m_fAngleY; }
	inline float * const AreaLight::getAngleZ(void) { return &m_fAngleZ; }
	inline float * const AreaLight::getDistance(void) { return &m_fDistance; }

	inline const GLuint AreaLight::getTexture(void) const { return m_Textures[0]; }

} //namespace worldObject

#endif // __WORLDOBJECT_AREALIGHT_H__
