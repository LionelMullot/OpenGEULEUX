
#pragma once
#ifndef __WORLDOBJECT_PLANE_H__
#define __WORLDOBJECT_PLANE_H__

#include <cstdint>
#include <vector>

#include "Object3D.h"
#include <GL/glew.h>

namespace worldObject
{
	class Plane : public Object3D
	{

	/* ***************************************************** */
	/* ************* CONSTUCTION AND DESTUCTION ************ */
	/* ***************************************************** */
	public:
		static Plane * create_ptr(void);
		static void release_ptr(Plane * ptr);

	protected:
		Plane(void);
		virtual ~Plane(void);

		virtual void init(void);
		virtual void release(void);

	/* ***************************************************** */
	/* ********************** METHODS ********************** */
	/* ***************************************************** */
	public:
		virtual void draw(const utils::ShaderProgram * p_pShaderProgram, const glm::mat4& p_mObjectToWorld) const;

	}; // class Plane

} //namespace worldObject

#endif // __WORLDOBJECT_PLANE_H__
