
#pragma once
#ifndef __WORLDOBJECT_LIGHT_H__
#define __WORLDOBJECT_LIGHT_H__

#include <cstdint>
#include <vector>
#include <glm/vec3.hpp>
#include "Object3D.h"
#include <GL/glew.h>

namespace worldObject
{
	class Light : public Object3D
	{

	/* ***************************************************** */
	/* ********************** MEMBERS ********************** */
	/* ***************************************************** */
	protected:
		float		m_fDiffuseIntensity;
		float		m_fSpecularIntensity;
		glm::vec3	m_vPosition;
		glm::vec3	m_vDirection;
		glm::vec3	m_vColor;

	/* ***************************************************** */
	/* ************* CONSTUCTION AND DESTUCTION ************ */
	/* ***************************************************** */
	public:
		static Light * create_ptr(void);
		static void release_ptr(Light * ptr);

	protected:
		Light(void);
		virtual ~Light(void);

		virtual void init(void);
		virtual void release(void);

	/* ***************************************************** */
	/* ********************** METHODS ********************** */
	/* ***************************************************** */
	public:
		virtual void draw(const utils::ShaderProgram * p_pShaderProgram, const glm::mat4& p_mObjectToWorld) const {};
		
	/* ***************************************************** */
	/* ****************** GETTER AND SETTER **************** */
	/* ***************************************************** */
	public:
		float * const getPositionAxisValue(int axis);
		const glm::vec3& getPosition(void) const;
		const glm::vec3& getDirection(void) const;
		const glm::vec3& getColor(void) const;
		const float getSpecularIntensity(void) const;
		float * const getSpecularIntensityPtr(void);
		const float getDiffuseIntensity(void) const;
		float * const getDiffuseIntensityPtr(void);

	}; // class Light

	inline float * const Light::getPositionAxisValue(int axis) { return &m_vPosition[axis]; }
	inline const glm::vec3& Light::getPosition(void) const { return m_vPosition; }
	inline const glm::vec3& Light::getDirection(void) const { return m_vDirection; }
	inline const glm::vec3& Light::getColor(void) const { return m_vColor; }
	inline const float Light::getDiffuseIntensity(void) const { return m_fDiffuseIntensity; }
	inline float * const Light::getDiffuseIntensityPtr(void) { return &m_fDiffuseIntensity; }
	inline const float Light::getSpecularIntensity(void) const { return m_fSpecularIntensity; }
	inline float * const Light::getSpecularIntensityPtr(void) { return &m_fSpecularIntensity; }

} //namespace worldObject

#endif // __WORLDOBJECT_LIGHT_H__
