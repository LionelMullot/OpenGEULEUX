
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
		void updateData(void);

	/* ***************************************************** */
	/* ****************** GETTER AND SETTER **************** */
	/* ***************************************************** */
	public:
		glm::vec3 getUp(void);
		glm::vec3 getRight(void);
		glm::vec2 getSize(void);
		float getDistance(void);

	}; // class AreaLight

	inline glm::vec3 AreaLight::getUp(void) { return m_vUp; }
	inline glm::vec3 AreaLight::getRight(void) { return m_vRight; }
	inline glm::vec2 AreaLight::getSize(void) { return glm::vec2(m_vSize.x * .5, m_vSize.y * .5); }
	inline float AreaLight::getDistance(void) { return m_fDistance; }

} //namespace worldObject

#endif // __WORLDOBJECT_AREALIGHT_H__
