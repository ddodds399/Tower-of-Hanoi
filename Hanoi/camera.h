#ifndef CAMERA_INCLUDED_H
#define CAMERA_INCLUDED_H
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Camera
{
public:
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
	{
		m_pos = pos;
		radius = pos.z;
		m_forward = glm::vec3(0.0f, 0.0f, 1.0f);
		m_up = glm::vec3(0.0f, 1.0f, 0.0f);
		m_perspective = glm::perspective(fov, aspect, zNear, zFar);
	}

	inline glm::mat4 GetViewProjection() const
	{
		return m_perspective * glm::lookAt(m_pos, glm::vec3(0.0f, 2.5f, 0.0f), m_up);
	}


	inline glm::mat4 GetSkyboxProjection() const
	{
		return m_perspective * glm::lookAt(m_pos, m_pos + m_forward, m_up);
	}

	void MoveForward(float amt)
	{
		m_pos += m_forward * amt;
		radius = m_pos.z;
	}

	void MoveBackward(float amt)
	{
		m_pos -= m_forward * amt;
		radius = m_pos.z;
	}

	void MoveRight(float amt)
	{
	m_pos += glm::cross(m_up, m_forward) * amt;
	}

	inline glm::vec3& GetCameraPos() { return m_pos; }
	inline glm::vec3& GetCameraForward() { return m_forward; }
	inline glm::vec3& GetCameraUp() { return m_up; }

	void OrbitRight()
	{
		GLfloat camX = sin(count) * radius;
		GLfloat camZ = cos(count) * radius;
		m_pos = glm::vec3(camX, m_pos.y, camZ);
		count += 0.01f;

	}

	void OrbitLeft()
	{
		GLfloat camX = sin(count) * radius;
		GLfloat camZ = cos(count) * radius;
		m_pos = glm::vec3(camX, m_pos.y, camZ);
		count -= 0.01f;

	}

	void RotateY(float angle)
	{
	 static const glm::vec3 UP(0.0f, 1.0f, 0.0f);
	 glm::mat4 rotation = glm::rotate(angle, UP);
	 m_forward = glm::vec3(glm::normalize(rotation * glm::vec4(m_forward, 0.0)));
	 m_up = glm::vec3(glm::normalize(rotation * glm::vec4(m_up, 0.0)));
	}

protected:
private:
	glm::mat4 m_perspective;
	glm::vec3 m_pos;
	glm::vec3 m_forward;
	glm::vec3 m_up;
	float count = 0.005f;
	GLfloat radius;
};
#endif // CAMERA_INCLUDED_H