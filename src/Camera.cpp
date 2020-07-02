#include "Camera.h"

Camera::Camera() {
	positionX = positionY = positionZ = 0.0f;
	rotationX = rotationY = rotationZ = 0.0f;
	viewMatrix = XMMATRIX();
}

void Camera::SetPosition(float x, float y, float z) {
	positionX = x;
	positionY = y;
	positionZ = z;
}

void Camera::SetRotation(float x, float y, float z) {
	rotationX = x;
	rotationY = y;
	rotationZ = z;
}

XMFLOAT3 Camera::GetPosition() {
	return XMFLOAT3(positionX, positionY, positionZ);
}

XMFLOAT3 Camera::GetRotation() {
	return XMFLOAT3(rotationX, rotationY, rotationZ);
}

void Camera::GetViewMatrix(XMMATRIX& viewMatrix) {
	viewMatrix = this->viewMatrix;
}

void Camera::Render() {
	XMMATRIX rotationMatrix;

	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3 position = XMFLOAT3(positionX, positionY, positionZ);
	//Where camera is looking at
	XMFLOAT3 lookAt = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMVECTOR upVector = XMLoadFloat3(&up);
	XMVECTOR positionVector = XMLoadFloat3(&position);
	XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);
	
	float pitch = rotationX * degreesToRadians;
	float yaw = rotationY * degreesToRadians;
	float roll = rotationZ * degreesToRadians;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}
