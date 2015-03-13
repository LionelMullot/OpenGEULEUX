
#pragma once
#ifndef __WORLDOBJECT_AREALIGHT_H__
#define __WORLDOBJECT_AREALIGHT_H__

#include "Light.h"

namespace worldObject
{
	// Create a unit cube center on the world origin
	class AreaLight : public Light
	{

	/* ***************************************************** */
	/* ********************** MEMBERS ********************** */
	/* ***************************************************** */
	protected:
		glm::vec3	m_vUp;
		glm::vec3	m_vRight; 
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
		virtual void initData(void);
		virtual void release(void);

	/* ***************************************************** */
	/* ********************** METHODS ********************** */
	/* ***************************************************** */
	public:
		virtual void draw() const;
		virtual void updateMatrix(void);

	/* ***************************************************** */
	/* ****************** GETTER AND SETTER **************** */
	/* ***************************************************** */
	public:
		const glm::vec3& getUp(void) const;
		const glm::vec3& getRight(void) const;
		float * const getDistance(void);
		const GLuint getTexture(void) const;

	}; // class AreaLight

	inline const glm::vec3& AreaLight::getUp(void) const { return m_vUp; }
	inline const glm::vec3& AreaLight::getRight(void) const { return m_vRight; }
	inline float * const AreaLight::getDistance(void) { return &m_fDistance; }

	inline const GLuint AreaLight::getTexture(void) const { return m_Textures[0]; }

} //namespace worldObject

#endif // __WORLDOBJECT_AREALIGHT_H__
