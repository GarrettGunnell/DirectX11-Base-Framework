#pragma once

#include <d3d11.h>
#include <stdio.h>

class Texture {
public:
	Texture();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const char*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool LoadTarga(const char*, int&, int&);

	unsigned char* targaData;
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* textureView;

	struct TargaHeader {
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};
};