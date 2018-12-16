#include "FPCamera.h"
#include <gl/glew.h>
#include<GL\glfw3.h>
FPCamera::FPCamera(void)
{
	this->Reset(0, 1.0, 3.0,
		0, 0, 0,
		0, 1, 0);
	SetPerspectiveProjection(45.0f,4.0f/3.0f,0.1f,100.0f);
}

FPCamera::~FPCamera(void)
{
}

bool FPCamera::HandleKeyboardInput(int key )
{
	switch (key)
	{
		//Moving forward
	case GLFW_KEY_W:
		
		Walk(0.01);
		break;
		//Moving backword
	case GLFW_KEY_S:
		Walk(-0.01);
		break;
		// Moving right
	case GLFW_KEY_D:
		Strafe(0.01);
		break;
		// Moving left
	case GLFW_KEY_A:
		Strafe(-0.01);
		break;
		// Moving up
	case GLFW_KEY_R:
		Fly(0.01);
		break;
		// Moving down
	case GLFW_KEY_F:
		Fly(-0.01);
		break;
	case GLFW_KEY_E:
		Yaw(0.5f);
		break;
	case GLFW_KEY_Q:
		Yaw(-0.5f);
		break;
	default:
		return false;
		break;
	}
	return true;
}

glm::vec3 FPCamera::GetLookDirection()
{
	return -mDirection;
}

void FPCamera::Reset(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
{
	glm::vec3 eyePt(eyeX, eyeY, eyeZ);
	glm::vec3 centerPt(centerX, centerY, centerZ);
	glm::vec3 upVec(upX, upY, upZ);
	Reset(eyePt, centerPt, upVec);
}

void FPCamera::Reset(const glm::vec3 &eye, const glm::vec3 &center, glm::vec3 &up)
{
	mPosition = eye;
	//still needs normalization
		mDirection = eye - center;
	//i = j x k
		mRight = glm::cross( up,mDirection );
	//j = k x i
		mUp = glm::cross(mDirection, mRight );
	//normalize all
		mUp = glm::normalize(mUp);
		mRight = glm::normalize(mRight);
		mDirection = glm::normalize(mDirection);
	UpdateViewMatrix();
}

glm::mat4 FPCamera::GetViewMatrix()
{
	return mViewMatrix;
}

void FPCamera::UpdateViewMatrix()
{
	glm::vec3 center = mPosition + this->GetLookDirection();
	mViewMatrix = glm::lookAt(mPosition,center,mUp);
}

glm::mat4 FPCamera::GetProjectionMatrix()
{
	return mProjectionMatrix;
}

void FPCamera::SetPerspectiveProjection(float FOV, float aspectRatio, float near, float far)
{
	mProjectionMatrix = glm::perspective(FOV,aspectRatio,near,far);
}

void FPCamera::Slide(float stepR, float stepU, float stepD)
{
	mPosition += stepR *mRight;
	mPosition += stepU * mUp;
	//next transition needs to be tested!!.
	mPosition += stepD * mDirection;
}

void FPCamera::Yaw(float angleDegrees)
{
	
	mDirection	= glm::rotate(mDirection,angleDegrees,mUp);
	mRight		= glm::rotate(mRight,	 angleDegrees,mUp);
}

void FPCamera::Pitch(float angleDegrees)
{
	mUp			= glm::rotate(mUp,		 angleDegrees,mRight);
	mDirection	= glm::rotate(mDirection,angleDegrees,mRight);
}

void FPCamera::Roll(float angleDegrees)
{
	mRight	= glm::rotate(mRight,angleDegrees,mDirection);
	mUp		= glm::rotate(mUp,	 angleDegrees,mDirection);
}

void FPCamera::Walk(float dist)
{
	mPosition -= dist * mDirection;
}

void FPCamera::Strafe(float dist)
{
	mPosition += dist *mRight;
}

void FPCamera::Fly(float dist)
{
	mPosition += dist * mUp;
}





