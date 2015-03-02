
#include "AreaLight.h"
#include "Utils/ShaderProgram.h"
#include <cassert>
#include <iostream>

#define M_PI        3.14159265358979323846264338327950288f   /* pi */
#define M_PI_2      1.57079632679489661923132169163975144f  /* pi/2 */
#define M_PI_4      0.785398163397448309615660845819875721f  /* pi/4 */

namespace worldObject
{
	AreaLight* AreaLight::create_ptr(void)
	{
		AreaLight * ptr = new AreaLight();
		ptr->init();
		return ptr;
	}

	void AreaLight::release_ptr(AreaLight * ptr)
	{
#ifndef NDEBUG
		assert(ptr != NULL);
#endif
		ptr->release();
		delete ptr;
	}

	AreaLight::AreaLight() 
	: Light()
	, m_mRotation(glm::mat4(0.0f))
	, m_mTranslation(glm::mat4(0.0f))
	, m_mScale(glm::mat4(0.0f))
	, m_mObjectModel(glm::mat4(0.0f))
	, m_vUp(glm::vec3(0.0f))
	, m_vRight(glm::vec3(0.0f))
	, m_vSize(glm::vec2(0.0f))
	, m_fDistance(0.0f)
	, m_fAngleX(0.0f)
	, m_fAngleY(0.0f)
	, m_fAngleZ(0.0f)
	{}

	AreaLight::~AreaLight() {}

	void AreaLight::init(void)
	{
		Light::init();

		m_vPosition = glm::vec3(0, 1, 0);
		m_vSize = glm::vec2(1, 1);
		m_fDistance = 8.0f;

		m_fDiffuseIntensity = 10.0f;
		m_fSpecularIntensity = 10.0f;
		m_vColor = glm::vec3(1, 1, 1);

		m_fAngleX = M_PI_2;

		m_mTranslation[0] = glm::vec4(1, 0, 0, 0);
		m_mTranslation[1] = glm::vec4(0, 1, 0, 0);
		m_mTranslation[2] = glm::vec4(0, 0, 1, 0);

		initData();
		updateMatrix();
	}

	void AreaLight::release(void)
	{
	}

	void AreaLight::initData(void)
	{
		float SizeX = 1.0f;
		float SizeY = 1.0f;
		float fLeft = - SizeX * .5f;
		float fRight = SizeX * .5f;
		float fTop = SizeY * .5f;
		float fBottom = - SizeY * .5f;
		float Zpos = 0.0f;
		float Zdistance = 0 + 0.1f;

		// Init shape default height in z axis
		m_iTriangleCount = 12;
		m_pTriangleList = { 0, 1, 2, 2, 1, 3, 4, 5, 6, 6, 5, 7, 8, 9, 10, 10, 9, 11, 12, 13, 14, 14, 13, 15, 16, 17, 18, 19, 17, 20, 21, 22, 23, 24, 25, 26, };
		m_pVertices = { 
			fLeft, fTop, Zpos, 
			fRight, fTop, Zpos,
			fLeft, fTop, Zdistance, 
			fRight, fTop, Zdistance,
			
			fLeft, fTop, Zdistance, 
			fRight, fTop, Zdistance,
			fLeft, fBottom, Zdistance, 
			fRight, fBottom, Zdistance, 
			
			fLeft, fBottom, Zdistance,
			fRight, fBottom, Zdistance,
			fLeft, fBottom, Zpos,
			fRight, fBottom, Zpos,
			
			fLeft, fBottom, Zpos, 
			fRight, fBottom, Zpos, 
			fLeft, fTop, Zpos, 
			fRight, fTop, Zpos,
			
			fRight, fTop, Zpos,
			fRight, fBottom, Zpos,
			fRight, fTop, Zdistance,
			fRight, fTop, Zdistance,
			fRight, fBottom, Zdistance, 

			fLeft, fBottom, Zpos, 
			fLeft, fTop, Zpos, 
			fLeft, fBottom, Zdistance,
			fLeft, fBottom, Zdistance,
			fLeft, fTop, Zpos,
			fLeft, fTop, Zdistance,
		};

		//m_pVertices = { -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5 };

		m_pNormals = { 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, };
		m_pUVS = { 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, };

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
	}
	
	void AreaLight::updateMatrix(void)
	{
		// Translation
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
		m_mScale[2] = glm::vec4(0, 0, 1, 0);
		m_mScale[3] = glm::vec4(0, 0, 0, 1);

		m_mObjectModel = m_mTranslation * m_mRotation * m_mScale; // Rotation first then translation

		glm::vec4 col1 = m_mObjectModel[0];
		glm::vec4 col2 = m_mObjectModel[1];
		glm::vec4 col3 = m_mObjectModel[2];
		glm::vec4 col4 = m_mObjectModel[3];

		m_vRight = glm::normalize(glm::vec3(col1.x, col1.y, col1.z));
		m_vUp = glm::normalize(glm::vec3(col2.x, col2.y, col2.z));
		m_vDirection = glm::normalize(glm::vec3(col3.x, col3.y, col3.z));
	}

	void AreaLight::draw(const utils::ShaderProgram * p_pShaderProgram, glm::mat4& p_mObjectToWorld) const
	{
		p_pShaderProgram->setUniform(p_pShaderProgram->getUniformIndex("ObjectToWorld"), m_mObjectModel);
		glBindVertexArray(m_idVao);
		glDrawElementsInstanced(GL_TRIANGLES, m_iTriangleCount * 3, GL_UNSIGNED_INT, (void*)0, 1);
		glBindVertexArray(0);
	}

} // namespace worldObject
