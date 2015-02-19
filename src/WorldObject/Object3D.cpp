
#include "Object3D.h"

#include <cassert>
#include <iostream>

namespace worldObject
{

	void Object3D::release_ptr(Object3D * ptr)
	{
#ifndef NDEBUG
		assert(ptr != NULL);
#endif
		ptr->release();
		delete ptr;
	}

	Object3D::Object3D(void)
		: m_iTriangleCount(0)
		, m_pTriangleList(NULL)
		, m_pUVS(NULL)
		, m_pVertices(NULL)
		, m_pNormals(NULL)
		, m_idVao(0)
	{}

	Object3D::~Object3D(void)
	{}

} // namespace worldObject
