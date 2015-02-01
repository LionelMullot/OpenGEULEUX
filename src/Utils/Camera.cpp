
#include "Camera.h"

#include <cstdio>
#include <cassert>

#define M_PI 3.14159265358979323846f

namespace utils
{

	Camera * Camera::create_ptr(void)
	{
		Camera * ptr = new Camera();
		ptr->init();
		return ptr;
	}

	void Camera::release_ptr(Camera * ptr)
	{
#ifndef NDEBUG
		assert(ptr != NULL);
#endif
		ptr->release();
		delete ptr;
	}

	Camera::Camera(void)
		: m_vEye(glm::vec3(0))
		, m_vOrigin(glm::vec3(0))
		, m_vUp(glm::vec3(0))
	{}


	Camera::~Camera(void)
	{}

	void Camera::init(void)
	{
	}

	void Camera::release(void)
	{
	}

	void Camera::compute(void)
	{
		m_vEye.x = cos(m_fTheta) * sin(m_fPhi) * m_fRadius + m_vOrigin.x;
		m_vEye.y = cos(m_fPhi) * m_fRadius + m_vOrigin.y;
		m_vEye.z = sin(m_fTheta) * sin(m_fPhi) * m_fRadius + m_vOrigin.z;

		m_vUp = glm::vec3(0.f, m_fPhi < M_PI ? 1.f : -1.f, 0.f);
	}

	void Camera::defaults(void)
	{
		m_fPhi = M_PI / 2.0f;
		m_fTheta = M_PI / 2.0f;
		m_fRadius = 10.f;
		compute();
	}

	void Camera::zoom(float p_factor)
	{
		m_fRadius += p_factor * m_fRadius;
		if (m_fRadius < 0.1)
		{
			m_fRadius = 10.f;
			m_vOrigin = m_vEye + glm::normalize(m_vOrigin - m_vEye) * m_fRadius;
		}
		compute();
	}

	void Camera::turn(float p_phi, float p_theta)
	{
		m_fTheta += 1.f * p_theta;
		m_fPhi -= 1.f * p_phi;
		if (m_fPhi >= (2 * M_PI) - 0.1)
			m_fPhi = 0.00001f;
		else if (m_fPhi <= 0)
			m_fPhi = 2.f * M_PI - 0.1f;
		compute();
	}

	void Camera::pan(float p_x, float p_y)
	{
		glm::vec3 up(0.f, m_fPhi < M_PI ? 1.f : -1.f, 0.f);
		glm::vec3 fwd = glm::normalize(m_vOrigin- - m_vEye);
		glm::vec3 side = glm::normalize(glm::cross(fwd, up));
		m_vUp = glm::normalize(glm::cross(side, fwd));
		m_vOrigin[0] += up[0] * p_y * m_fRadius * 2;
		m_vOrigin[1] += up[1] * p_y * m_fRadius * 2;
		m_vOrigin[2] += up[2] * p_y * m_fRadius * 2;
		m_vOrigin[0] -= side[0] * p_x * m_fRadius * 2;
		m_vOrigin[1] -= side[1] * p_x * m_fRadius * 2;
		m_vOrigin[2] -= side[2] * p_x * m_fRadius * 2;

		compute();
	}


}; // namespace Utils
