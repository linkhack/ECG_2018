#include "Camera.h"
using namespace glm;
/*
 Creates Camera, fov in degrees
*/

Camera::Camera(float fov, float aspect, float near, float far)
{ 
	pitch = 0.0f;
	yaw = 0.0f;
	radius = 6.0f;
	position = vec3(0.0f, 0.0f, 6.0f);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 right = vec3(1.0f, 0.0f, 0.0f);
	projectionMatrix = perspective(radians(fov), aspect, near, far);
	mat4 rotation = rotate(mat4(1.0f), pitch, right)*rotate(mat4(1.0f), -yaw, up);
	viewMatrix = transpose(rotation) * translate(mat4(1.0f), -position);
}


Camera::~Camera()
{
}

mat4 Camera::getViewProjectionMatrix()
{
	return projectionMatrix*viewMatrix;
}

void Camera::update(int x, int y, float zoom, bool dragging)
{
	mat4 rotation = mat4(1.0f);
	radius = zoom;
	if (radius < 0.2) radius = 0.2;
	int deltaX = x - lastX;
	int deltaY = y - lastY;
	if (dragging)
	{
		float deltaAnglePitch = 2 * pi<float>()*float(deltaY) / 600.0;
		float deltaAngleYaw = pi<float>()*float(deltaX) / 600.0;
		pitch += deltaAnglePitch;
		yaw -= deltaAngleYaw;
		if (pitch < -pi<float>() / 2.0 + 0.01)
		{
			pitch = -pi<float>() / 2.0 + 0.01;
		}
		if (pitch > pi<float>() / 2.0 - 0.01)
		{
			pitch = pi<float>() / 2.0 - 0.01;
		}
	}
	position = radius * vec3(cosf(pitch)*sin(yaw), sinf(pitch), cosf(pitch)*cosf(yaw));

	std::cout << position.x << " " << position.y << " " << position.z << std::endl;
	/*
	vec3 lookAt = normalize(position);
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = normalize(cross(lookAt, up));
	vec3 left = cross(lookAt,right);
	*/
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 lookingDirection =-position;
	vec3 front = normalize(lookingDirection);
	vec3 right = normalize(cross(front, up));
	vec3 zAxis = cross(right, front);

	//rotation = mat4(vec4(front, 0.0f), vec4(right, 0.0f), vec4(zAxis, 0.0f), vec4(0, 0, 0, 1));
	rotation = mat4(vec4(right, 0.0f), vec4(zAxis, 0.0f), vec4(-front, 0.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f));
	
	viewMatrix = transpose(rotation)*translate(mat4(1.0f),-position);
	//viewMatrix = rotation*translate(mat4(1.0f),-position);
	lastX = x;
	lastY = y;
}