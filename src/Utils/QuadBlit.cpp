
#include "QuadBlit.h"

#include <cassert>
#include <iostream>

namespace utils
{
	QuadBlit * QuadBlit::create_ptr(void)
	{
		QuadBlit * ptr = new QuadBlit();
		ptr->init();
		return ptr;
	}

	void QuadBlit::release_ptr(QuadBlit * ptr)
	{
#ifndef NDEBUG
		assert(ptr != NULL);
#endif
		ptr->release();
		delete ptr;
	}

	QuadBlit::QuadBlit() {}

	QuadBlit::~QuadBlit() {}

	void QuadBlit::init(void)
	{
		// Blit texture object 
		m_iTriangleCount = 2;

		int   quad_triangleList[] = { 0, 1, 2, 2, 1, 3 };
		float quad_vertices[] = { -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0 };

		// Create VAO
		glGenVertexArrays(1, &m_idVao);
		// Create VBO
		glGenBuffers(2, m_tVbo);

		// Bind VAO
		glBindVertexArray(m_idVao);
		// Bind indices and upload data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_tVbo[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_triangleList), quad_triangleList, GL_STATIC_DRAW);
		// Bind vertices and upload data
		glBindBuffer(GL_ARRAY_BUFFER, m_tVbo[1]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 2, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
	}

	void QuadBlit::release(void)
	{
	}

	void QuadBlit::draw(void) const
	{
		glBindVertexArray(m_idVao);
		glDrawElementsInstanced(GL_TRIANGLES, m_iTriangleCount * 3, GL_UNSIGNED_INT, (void*)0, 1);
		glBindVertexArray(0);
	}

} // namespace worldObject
