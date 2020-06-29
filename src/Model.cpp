#include "Model.h"

Model::Model() {
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	texture = nullptr;
}

bool Model::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* textureFilename) {
	bool result;

	result = InitializeBuffers(device);
	if (!result) {
		return false;
	}

	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result) {
		return false;
	}

	return true;
}

void Model::Shutdown() {
	ReleaseTexture();
	ShutdownBuffers();
}

void Model::Render(ID3D11DeviceContext* deviceContext) {
	RenderBuffers(deviceContext);
}

int Model::GetIndexCount() {
	return this->indexCount;
}

ID3D11ShaderResourceView* Model::GetTexture() {
	return texture->GetTexture();
}

bool Model::InitializeBuffers(ID3D11Device* device) {
	Vertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	this->vertexCount = this->indexCount = 3;

	vertices = new Vertex[this->vertexCount];
	if (!vertices) {
		return false;
	}

	indices = new unsigned long[this->indexCount];
	if (!indices) {
		return false;
	}

	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[1].texture = XMFLOAT2(0.5f, 0.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[2].texture = XMFLOAT2(1.0f, 1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * this->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &this->vertexBuffer);
	if (FAILED(result)) {
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * this->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &this->indexBuffer);
	if (FAILED(result)) {
		return false;
	}

	delete[] vertices;
	delete[] indices;
	vertices = nullptr;
	indices = nullptr;

	return true;
}

void Model::ShutdownBuffers() {
	if (indexBuffer) {
		indexBuffer->Release();
		indexBuffer = nullptr;
	}

	if (vertexBuffer) {
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext) {
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(Vertex);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(this->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Model::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* filename) {
	bool result;
	texture = new Texture();
	if (!texture) {
		return false;
	}

	result = texture->Initialize(device, deviceContext, filename);
	if (!result) {
		return false;
	}

	return true;
}

void Model::ReleaseTexture() {
	if (texture) {
		texture->Shutdown();
		delete texture;
		texture = nullptr;
	}
}