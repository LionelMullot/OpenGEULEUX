
#pragma once
#ifndef __WORLDOBJECT_CUBE_H__
#define __WORLDOBJECT_CUBE_H__

#include <cstdint>
#include <vector>

#include "Object3D.h"
#include <GL/glew.h>

namespace worldObject
{
	// Create a unit cube center on the world origin
	class Cube : public Object3D
	{

	/* ***************************************************** */
	/* ********************** MEMBERS ********************** */
	/* ***************************************************** */
	protected:
		GLuint	m_Textures[2];

	/* ***************************************************** */
	/* ************* CONSTUCTION AND DESTUCTION ************ */
	/* ***************************************************** */
	public:
		static Cube * create_ptr(void);
		static void release_ptr(Cube * ptr);

	protected:
		Cube(void);
		virtual ~Cube(void);

		virtual void init(void);
		virtual void release(void);

	/* ***************************************************** */
	/* ********************** METHODS ********************** */
	/* ***************************************************** */
	public:
		virtual void draw(const utils::ShaderProgram * p_pShaderProgram, const glm::mat4& p_mObjectToWorld) const;

	}; // class Cube

} //namespace worldObject

#endif // __WORLDOBJECT_CUBE_H__
