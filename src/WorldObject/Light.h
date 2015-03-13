
#pragma once
#ifndef __WORLDOBJECT_LIGHT_H__
#define __WORLDOBJECT_LIGHT_H__

#include "Object3D.h"

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
		virtual void initData(void);
		virtual void release(void);

	/* ***************************************************** */
	/* ********************** METHODS ********************** */
	/* ***************************************************** */
	public:
		virtual void draw() const {}; 
		virtual void updateMatrix(void);
		
	/* ***************************************************** */
	/* ****************** GETTER AND SETTER **************** */
	/* ***************************************************** */
	public:
		const glm::vec3& getDirection(void) const;
		const glm::vec3& getColor(void) const;
		const float getSpecularIntensity(void) const;
		float * const getSpecularIntensityPtr(void);
		const float getDiffuseIntensity(void) const;
		float * const getDiffuseIntensityPtr(void);

	}; // class Light

	inline const glm::vec3& Light::getDirection(void) const { return m_vDirection; }
	inline const glm::vec3& Light::getColor(void) const { return m_vColor; }
	inline const float Light::getDiffuseIntensity(void) const { return m_fDiffuseIntensity; }
	inline float * const Light::getDiffuseIntensityPtr(void) { return &m_fDiffuseIntensity; }
	inline const float Light::getSpecularIntensity(void) const { return m_fSpecularIntensity; }
	inline float * const Light::getSpecularIntensityPtr(void) { return &m_fSpecularIntensity; }

} //namespace worldObject

#endif // __WORLDOBJECT_LIGHT_H__
