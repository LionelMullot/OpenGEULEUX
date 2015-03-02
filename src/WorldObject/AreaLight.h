
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
		float		m_fDistance;
		float		m_fAngleX;
		float		m_fAngleY;
		float		m_fAngleZ;

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
		virtual void draw(const utils::ShaderProgram * p_pShaderProgram, glm::mat4& p_mObjectToWorld) const;

		void initData(void);
		void updateMatrix(void);

	/* ***************************************************** */
	/* ****************** GETTER AND SETTER **************** */
	/* ***************************************************** */
	public:
		glm::vec3 getUp(void);
		glm::vec3 getRight(void);
		glm::vec2 getSize(void);
		float * getSizeX(void);
		float * getSizeY(void);
		float getDistance(void);
		float * getDistancePtr(void);
		float * getAngleX(void);
		float * getAngleY(void);
		float * getAngleZ(void);

	}; // class AreaLight

	inline glm::vec3 AreaLight::getUp(void) { return m_vUp; }
	inline glm::vec3 AreaLight::getRight(void) { return m_vRight; }
	inline glm::vec2 AreaLight::getSize(void) { return glm::vec2(m_vSize.x * .5, m_vSize.y * .5); }
	inline float * AreaLight::getSizeX(void) { return &m_vSize.x; }
	inline float * AreaLight::getSizeY(void) { return &m_vSize.y; }
	inline float AreaLight::getDistance(void) { return m_fDistance; }
	inline float * AreaLight::getDistancePtr(void) { return &m_fDistance; }
	inline float * AreaLight::getAngleX(void) { return &m_fAngleX; }
	inline float * AreaLight::getAngleY(void) { return &m_fAngleY; }
	inline float * AreaLight::getAngleZ(void) { return &m_fAngleZ; }

} //namespace worldObject

#endif // __WORLDOBJECT_AREALIGHT_H__
