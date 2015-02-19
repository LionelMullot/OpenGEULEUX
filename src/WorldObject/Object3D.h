
#pragma once
#ifndef __WORLDOBJECT_OBJECT3D_H__
#define __WORLDOBJECT_OBJECT3D_H__

#include <cstdint>
#include <vector>

#include <GL/glew.h>

namespace worldObject
{
	class Object3D
	{
	
	/* ***************************************************** */
	/* ********************** MEMBERS ********************** */
	/* ***************************************************** */
	protected:
		int32_t					m_iTriangleCount;
		std::vector<int32_t>	m_pTriangleList;
		std::vector<float>		m_pUVS;
		std::vector<float>		m_pVertices;
		std::vector<float>		m_pNormals;
		
		// OpenGL Attributes
		GLuint	m_idVao;
		GLuint	m_tVbo[4];

	/* ***************************************************** */
	/* ************* CONSTUCTION AND DESTUCTION ************ */
	/* ***************************************************** */
	public:
		static void release_ptr(Object3D * ptr);

	protected:
		Object3D(void);
		virtual ~Object3D(void);

		virtual void init(void) = 0;
		virtual void release(void) = 0;

	/* ***************************************************** */
	/* ********************** METHODS ********************** */
	/* ***************************************************** */
	public:
		virtual void draw(void) const = 0;

	}; // class Object3D

} //namespace worldObject

#endif // __WORLDOBJECT_OBJECT3D_H__
