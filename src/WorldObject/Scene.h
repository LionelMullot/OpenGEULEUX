
#pragma once
#ifndef __WORLDOBJECT_SCENE_H__
#define __WORLDOBJECT_SCENE_H__

#include <cstdint>
#include <vector>

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

namespace utils
{
	class ShaderProgram;
}

namespace worldObject
{
	class Object3D;

	class Scene
	{
	
	/* ***************************************************** */
	/* ********************** MEMBERS ********************** */
	/* ***************************************************** */
	protected:
		std::vector<Object3D *>	m_pObject3D;

	/* ***************************************************** */
	/* ************* CONSTUCTION AND DESTUCTION ************ */
	/* ***************************************************** */
	public:
		static Scene * create_ptr(void);
		static void release_ptr(Scene * ptr);

	protected:
		Scene(void);
		~Scene(void);

		void init(void);
		void release(void);

	/* ***************************************************** */
	/* ********************** METHODS ********************** */
	/* ***************************************************** */
	public:
		void draw(const utils::ShaderProgram * p_pShaderProgram, const glm::mat4& p_mObjectToWorld) const;

	/* ***************************************************** */
	/* *************** GETTER / SETTER ********************* */
	/* ***************************************************** */
	public:
		void addObject(Object3D * p_pObject);

	}; // class Scene

} //namespace worldObject

#endif // __WORLDOBJECT_SCENE_H__
