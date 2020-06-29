#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "Texture.h"

using namespace DirectX;

class Model {
public:
	Model();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	struct Vertex {
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	bool InitializeBuffers(ID3D11Device*);
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, const char*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	void ReleaseTexture();

	ID3D11Buffer* vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;
	Texture* texture;
};