
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
		virtual void draw(const utils::ShaderProgram * p_pShaderProgram, glm::mat4& p_mObjectToWorld) const {};
		
	/* ***************************************************** */
	/* ****************** GETTER AND SETTER **************** */
	/* ***************************************************** */
	public:
		glm::vec3 getPosition(void);
		glm::vec3 getDirection(void);
		glm::vec3 getColor(void);
		float getSpecularIntensity(void);
		float getDiffuseIntensity(void);

	}; // class Light

	inline glm::vec3 Light::getPosition(void) { return m_vPosition; }
	inline glm::vec3 Light::getDirection(void) { return m_vDirection; }
	inline glm::vec3 Light::getColor(void) { return m_vColor; }
	inline float Light::getDiffuseIntensity(void) { return m_fDiffuseIntensity; }
	inline float Light::getSpecularIntensity(void) { return m_fSpecularIntensity; }

} //namespace worldObject

#endif // __WORLDOBJECT_LIGHT_H__
