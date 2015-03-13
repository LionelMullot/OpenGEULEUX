
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
		: m_mRotation(glm::mat4(0.0f))
		, m_mTranslation(glm::mat4(0.0f))
		, m_mScale(glm::mat4(0.0f))
		, m_mObjectModel(glm::mat4(0.0f))
		, m_vPosition(glm::vec3(0.0f))
		, m_vSize(glm::vec3(0.0f))
		, m_fAngleX(0.0f)
		, m_fAngleY(0.0f)
		, m_fAngleZ(0.0f)
		, m_iTriangleCount(0)
		, m_pTriangleList(NULL)
		, m_pUVS(NULL)
		, m_pVertices(NULL)
		, m_pNormals(NULL)
		, m_idVao(0)
	{}

	Object3D::~Object3D(void)
	{}

	void Object3D::updateMatrix(void)
	{
		// Translation
		m_mTranslation[0] = glm::vec4(1, 0, 0, 0);
		m_mTranslation[1] = glm::vec4(0, 1, 0, 0);
		m_mTranslation[2] = glm::vec4(0, 0, 1, 0);
		m_mTranslation[3] = glm::vec4(m_vPosition[0], m_vPosition[1], m_vPosition[2], 1);
		// Rotation on x axis
		glm::mat4 mRotationX;
		mRotationX[0] = glm::vec4(1, 0, 0, 0);
		mRotationX[1] = glm::vec4(0, glm::cos(m_fAngleX), glm::sin(m_fAngleX), 0);
		mRotationX[2] = glm::vec4(0, -glm::sin(m_fAngleX), glm::cos(m_fAngleX), 0);
		mRotationX[3] = glm::vec4(0, 0, 0, 1);
		// Rotation on y axis
		glm::mat4 mRotationY;
		mRotationY[0] = glm::vec4(glm::cos(m_fAngleY), 0, -glm::sin(m_fAngleY), 0);
		mRotationY[1] = glm::vec4(0, 1, 0, 0);
		mRotationY[2] = glm::vec4(glm::sin(m_fAngleY), 0, glm::cos(m_fAngleY), 0);
		mRotationY[3] = glm::vec4(0, 0, 0, 1);
		// Rotation on z axis
		glm::mat4 mRotationZ;
		mRotationZ[0] = glm::vec4(glm::cos(m_fAngleZ), glm::sin(m_fAngleZ), 0, 0);
		mRotationZ[1] = glm::vec4(-glm::sin(m_fAngleZ), glm::cos(m_fAngleZ), 0, 0);
		mRotationZ[2] = glm::vec4(0, 0, 1, 0);
		mRotationZ[3] = glm::vec4(0, 0, 0, 1);

		// Final rotation
		m_mRotation = mRotationX * mRotationY * mRotationZ;

		// Scaling x and y axis
		m_mScale[0] = glm::vec4(m_vSize.x, 0, 0, 0);
		m_mScale[1] = glm::vec4(0, m_vSize.y, 0, 0);
		m_mScale[2] = glm::vec4(0, 0, m_vSize.z, 0);
		m_mScale[3] = glm::vec4(0, 0, 0, 1);

		m_mObjectModel = m_mTranslation * m_mRotation * m_mScale; // Rotation first then translation
	}

} // namespace worldObject
