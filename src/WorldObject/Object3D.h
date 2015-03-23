
#pragma once
#ifndef __WORLDOBJECT_OBJECT3D_H__
#define __WORLDOBJECT_OBJECT3D_H__

#include <cstdint>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

#define M_PI        3.14159265358979323846264338327950288f   /* pi */
#define M_PI_2      1.57079632679489661923132169163975144f  /* pi/2 */
#define M_PI_4      0.785398163397448309615660845819875721f  /* pi/4 */

namespace utils
{
	class ShaderProgram;
}

namespace worldObject
{
	class Object3D
	{
	
	/* ***************************************************** */
	/* ********************** MEMBERS ********************** */
	/* ***************************************************** */
	protected:
		// Translation / rotation / scale
		glm::mat4   m_mRotation;
		glm::mat4   m_mTranslation;
		glm::mat4   m_mScale;
		glm::mat4	m_mObjectModel;
		// Position / size / angle
		glm::vec3	m_vPosition;
		glm::vec3	m_vSize;
		float		m_fAngleX;
		float		m_fAngleY;
		float		m_fAngleZ;

		// Datas 
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
		virtual void initData(void) = 0;
		virtual void release(void) = 0;

	/* ***************************************************** */
	/* ********************** METHODS ********************** */
	/* ***************************************************** */
	public:
		virtual void draw() const = 0;
		virtual void updateMatrix(void);

	/* ***************************************************** */
	/* ****************** GETTER AND SETTER **************** */
	/* ***************************************************** */
		float * const getPositionAxisValue(int axis);
		const glm::vec3& getPosition(void) const;
		glm::vec3& getPosition(void);
		void setPosition(const glm::vec3& p_vPosition);

		const glm::mat4& getModelMatrix(void) const;

		const glm::vec2 getSize(void) const;
		void setSize(const glm::vec3& p_vSize);
		float * const getSizeX(void);
		float * const getSizeY(void);

		float * const getAngleX(void);
		void setAngleX(float p_fAngle);
		float * const getAngleY(void);
		void setAngleY(float p_fAngle);
		float * const getAngleZ(void);
		void setAngleZ(float p_fAngle);

	}; // class Object3D

	inline float * const Object3D::getPositionAxisValue(int axis) { return &m_vPosition[axis]; }
	inline const glm::vec3& Object3D::getPosition(void) const { return m_vPosition; }
	inline glm::vec3& Object3D::getPosition(void) { return m_vPosition; }
	inline void Object3D::setPosition(const glm::vec3& p_vPosition) { m_vPosition = p_vPosition; updateMatrix(); }

	inline const glm::mat4& Object3D::getModelMatrix(void) const { return m_mObjectModel; }

	inline const glm::vec2 Object3D::getSize(void) const { return glm::vec2(m_vSize.x * .5, m_vSize.y * .5); }
	inline void Object3D::setSize(const glm::vec3& p_vSize) { m_vSize = p_vSize; updateMatrix(); }
	inline float * const Object3D::getSizeX(void) { return &m_vSize.x; }
	inline float * const Object3D::getSizeY(void) { return &m_vSize.y; }

	inline float * const Object3D::getAngleX(void) { return &m_fAngleX; }
	inline void Object3D::setAngleX(float p_fAngle) { m_fAngleX = p_fAngle; updateMatrix(); }
	inline float * const Object3D::getAngleY(void) { return &m_fAngleY; }
	inline void Object3D::setAngleY(float p_fAngle) { m_fAngleY = p_fAngle; updateMatrix(); }
	inline float * const Object3D::getAngleZ(void) { return &m_fAngleZ; }
	inline void Object3D::setAngleZ(float p_fAngle) { m_fAngleZ = p_fAngle; updateMatrix(); }


} //namespace worldObject

#endif // __WORLDOBJECT_OBJECT3D_H__
