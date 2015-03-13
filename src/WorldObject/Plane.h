
#pragma once
#ifndef __WORLDOBJECT_PLANE_H__
#define __WORLDOBJECT_PLANE_H__

#include "Object3D.h"

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
		virtual void initData(void);
		virtual void release(void);

	/* ***************************************************** */
	/* ********************** METHODS ********************** */
	/* ***************************************************** */
	public:
		virtual void draw() const;
		virtual void updateMatrix(void);

	}; // class Plane

} //namespace worldObject

#endif // __WORLDOBJECT_PLANE_H__
