
#pragma once
#ifndef __UTILS_CAMERA_H__
#define __UTILS_CAMERA_H__

#include <glm/glm.hpp>

namespace utils
{
	class Camera
	{

		/* ***************************************************** */
		/* ********************** MEMBERS ********************** */
		/* ***************************************************** */
	protected:
		float		m_fRadius;
		float		m_fTheta;
		float		m_fPhi;
		glm::vec3	m_vOrigin;
		glm::vec3	m_vEye;
		glm::vec3	m_vUp;

		/* ***************************************************** */
		/* ************* CONSTUCTION AND DESTUCTION ************ */
		/* ***************************************************** */
	public:
		static Camera * create_ptr(void);
		static void release_ptr(Camera * ptr);

	protected:
		Camera(void);
		~Camera(void);

		void init(void);
		void release(void);

		/* ***************************************************** */
		/* ********************** METHODS ********************** */
		/* ***************************************************** */
	public:
		void defaults(void);
		void zoom(float p_factor);
		void turn(float p_phi, float p_theta);
		void pan(float p_x, float p_y);

	protected:

	private:
		void compute(void);

		/* ***************************************************** */
		/* *********************** GETTER ********************** */
		/* ***************************************************** */
	public:
		const glm::vec3& getWhereILook(void) const;
		const glm::vec3& getOrigin(void) const;
		const glm::vec3& getUp(void) const;

	}; // class Camera
	
	inline const glm::vec3& Camera::getWhereILook(void) const { return m_vEye; }
	inline const glm::vec3& Camera::getOrigin(void) const { return m_vOrigin; }
	inline const glm::vec3& Camera::getUp(void) const { return m_vUp; }

}; // namespace Utils

#endif //__UTILS_CAMERA_H__
