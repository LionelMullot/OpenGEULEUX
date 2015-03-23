
#include "Cube.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cassert>
#include <iostream>
	
namespace worldObject
{

	Cube * Cube::create_ptr(void)
	{
		Cube * ptr = new Cube();
		ptr->init();
		return ptr;
	}

	void Cube::release_ptr(Cube * ptr)
	{
#ifndef NDEBUG
		assert(ptr != NULL);
#endif
		ptr->release();
		delete ptr;
	}

	Cube::Cube() {}

	Cube::~Cube() {}

	void Cube::init(void)
	{
		m_vSize = glm::vec3(1, 1, 1);

		initData();
		updateMatrix();
	}

	void Cube::release(void)
	{
	}

	void Cube::initData(void)
	{
		// Init shape
		m_iTriangleCount = 12;
		m_pTriangleList = { 0, 1, 2, 2, 1, 3, 4, 5, 6, 6, 5, 7, 8, 9, 10, 10, 9, 11, 12, 13, 14, 14, 13, 15, 16, 17, 18, 19, 17, 20, 21, 22, 23, 24, 25, 26, };
		m_pUVS = { 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, };
		m_pVertices = { -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5 };
		m_pNormals = { 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, };

		// Init VAO & VBO 
		glGenVertexArrays(1, &m_idVao);

		// Create a VBO for each array
		glGenBuffers(4, m_tVbo);

		// Bind the VAO
		glBindVertexArray(m_idVao);

		// Bind indices and upload data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_tVbo[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_pTriangleList.size() * sizeof(int32_t), &(m_pTriangleList[0]), GL_STATIC_DRAW);

		// Bind vertices and upload data
		glBindBuffer(GL_ARRAY_BUFFER, m_tVbo[1]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 3, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, m_pVertices.size() * sizeof(float), &(m_pVertices[0]), GL_STATIC_DRAW);

		// Bind normals and upload data
		glBindBuffer(GL_ARRAY_BUFFER, m_tVbo[2]);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 3, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, m_pNormals.size() * sizeof(float), &(m_pNormals[0]), GL_STATIC_DRAW);

		// Bind uv coords and upload data
		glBindBuffer(GL_ARRAY_BUFFER, m_tVbo[3]);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 2, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, m_pUVS.size() * sizeof(float), &(m_pUVS[0]), GL_STATIC_DRAW);

		// Unbind everything
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Load image
		int x;
		int y;
		int comp;
		unsigned char * diffuse = stbi_load("../../textures/spnza_bricks_a_diff.tga", &x, &y, &comp, 4);
		int x1;
		int y1;
		int comp1;
		unsigned char * specular = stbi_load("../../textures/spnza_bricks_a_spec.tga", &x1, &y1, &comp1, 4);

		// Create Texture for the cube
		glGenTextures(1, &m_Textures[0]);
		glBindTexture(GL_TEXTURE_2D, m_Textures[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, diffuse);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenTextures(1, &m_Textures[1]);
		glBindTexture(GL_TEXTURE_2D, m_Textures[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x1, y1, 0, GL_RGBA, GL_UNSIGNED_BYTE, specular);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void Cube::updateMatrix(void)
	{
		Object3D::updateMatrix();
	}

	void Cube::draw() const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_Textures[1]);
		glBindVertexArray(m_idVao);
		glDrawElementsInstanced(GL_TRIANGLES, m_iTriangleCount * 3, GL_UNSIGNED_INT, (void*)0, 1);
		glBindVertexArray(0);
	}

} // namespace worldObject
