
#pragma once
#ifndef __UTILS_QUADBLIT_H__
#define __UTILS_QUADBLIT_H__

#include <cstdint>
#include <vector>

#include <GL/glew.h>

namespace utils
{
	class QuadBlit
	{
		/* ***************************************************** */
		/* ********************** MEMBERS ********************** */
		/* ***************************************************** */
	protected:
		int		m_iTriangleCount;
		GLuint	m_idVao;
		GLuint	m_tVbo[2];
		
		/* ***************************************************** */
		/* ************* CONSTUCTION AND DESTUCTION ************ */
		/* ***************************************************** */
	public:
		static QuadBlit * create_ptr(void);
		static void release_ptr(QuadBlit * ptr);

	protected:
		QuadBlit(void);
		virtual ~QuadBlit(void);

		void init(void);
		void release(void);

		/* ***************************************************** */
		/* ********************** METHODS ********************** */
		/* ***************************************************** */
	public:
		void draw(void) const;

	}; // class QuadBlit

} //namespace utils

#endif // __UTILS_QUADBLIT_H__
