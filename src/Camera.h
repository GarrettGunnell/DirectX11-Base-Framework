#pragma once

#define degreesToRadians 0.0174532925

#include <DirectXMath.h>
using namespace DirectX;


class Camera {
public:
	Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX&);

	void* operator new(size_t i) {
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p) {
		_mm_free(p);
	}
private:
	float positionX, positionY, positionZ;
	float rotationX, rotationY, rotationZ;
	XMMATRIX viewMatrix;
};