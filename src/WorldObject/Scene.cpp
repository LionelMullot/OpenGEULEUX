
#include "Scene.h"
#include "Object3D.h"
#include "Utils/ShaderProgram.h"

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

	void Scene::draw(const utils::ShaderProgram * p_pShaderProgram, const glm::mat4& p_mObjectToWorld) const
	{
		for (Object3D* p_object : m_pObject3D)
		{
			p_pShaderProgram->setUniform(p_pShaderProgram->getUniformIndex("ObjectToWorld"), p_mObjectToWorld);
			p_object->draw(p_pShaderProgram, p_mObjectToWorld);
		}
	}

	void Scene::addObject(Object3D * p_pObject)
	{
		m_pObject3D.push_back(p_pObject);
	}

	void Scene::removeLastObjectAdd()
	{
		m_pObject3D.pop_back();
	}
} // namespace worldObject
