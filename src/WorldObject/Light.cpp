
#include "Light.h"

#include <cassert>
#include <iostream>
	
namespace worldObject
{

	Light * Light::create_ptr(void)
	{
		Light * ptr = new Light();
		ptr->init();
		return ptr;
	}

	void Light::release_ptr(Light * ptr)
	{
#ifndef NDEBUG
		assert(ptr != NULL);
#endif
		ptr->release();
		delete ptr;
	}

	Light::Light() 
		: m_fDiffuseIntensity(0)
		, m_fSpecularIntensity(0)
		, m_vDirection(glm::vec3(0))
		, m_vColor(glm::vec3(0))
	{}

	Light::~Light() {}

	void Light::init(void)
	{
		m_fDiffuseIntensity = 50;
		m_fSpecularIntensity = 50;
		m_vColor = glm::vec3(1, 1, 1);
		
		initData();
		updateMatrix();
	}

	void Light::initData(void)
	{

	}

	void Light::updateMatrix(void)
	{
		Object3D::updateMatrix();
	}

	void Light::release(void)
	{
	}

} // namespace worldObject
