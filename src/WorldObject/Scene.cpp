
#include "Scene.h"
#include "Object3D.h"

#include <cassert>
#include <iostream>

namespace worldObject
{

	Scene * Scene::create_ptr(void)
	{
		Scene * ptr = new Scene();
		ptr->init();
		return ptr;
	}

	void Scene::release_ptr(Scene * ptr)
	{
#ifndef NDEBUG
		assert(ptr != NULL);
#endif
		ptr->release();
		delete ptr;
	}

	Scene::Scene()
		: m_pObject3D(NULL)
	{}

	Scene::~Scene() {}

	void Scene::init(void)
	{}

	void Scene::release(void)
	{
		for (auto p_object : m_pObject3D)
		{
			Object3D::release_ptr(p_object);
		}
	}

	void Scene::draw(void) const
	{
		for (Object3D* p_object : m_pObject3D)
		{
			p_object->draw();
		}
	}

	void Scene::addObject(Object3D * p_pObject)
	{
		m_pObject3D.push_back(p_pObject);
	}

} // namespace worldObject
