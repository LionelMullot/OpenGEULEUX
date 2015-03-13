
#pragma once
#ifndef __WORLDOBJECT_CUBE_H__
#define __WORLDOBJECT_CUBE_H__

#include "Object3D.h"

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
		virtual void initData(void);
		virtual void release(void);

	/* ***************************************************** */
	/* ********************** METHODS ********************** */
	/* ***************************************************** */
	public:
		virtual void draw() const;
		virtual void updateMatrix(void);

	}; // class Cube

} //namespace worldObject

#endif // __WORLDOBJECT_CUBE_H__
