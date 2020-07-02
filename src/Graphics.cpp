#include "Graphics.h"


Graphics::Graphics() {
	Direct3D = nullptr;
	camera = nullptr;
	model = nullptr;
	textureShader = nullptr;
}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd) {
	bool result;


	// Create the Direct3D object.
	Direct3D = new D3D;
	if(!Direct3D)
		return false;

	// Initialize the Direct3D object.
	result = Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	camera = new Camera;
	camera->SetPosition(0.0f, 0.0f, -5.0f);

	model = new Model;
	result = model->Initialize(Direct3D->GetDevice(), Direct3D->GetDeviceContext(), "bird.tga");
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	textureShader = new TextureShader();
	result = textureShader->Initialize(Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the color shader", L"Error", MB_OK);
		return false;
	}

	return true;
}


void Graphics::Shutdown() {
	if (textureShader) {
		textureShader->Shutdown();
		delete textureShader;
		textureShader = nullptr;
	}

	if (model) {
		model->Shutdown();
		delete model;
		model = nullptr;
	}

	if (camera) {
		delete camera;
		camera = nullptr;
	}

	if (Direct3D) {
		Direct3D->Shutdown();
		delete Direct3D;
		Direct3D = nullptr;
	}
}


bool Graphics::Frame() {
	bool result;

	result = Render();
	if (!result)
		return false;

	return true;
}


bool Graphics::Render() {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	Direct3D->BeginScene(1.0f, 0.5f, 0.5f, 1.0f);

	camera->Render();

	Direct3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	Direct3D->GetProjectionMatrix(projectionMatrix);

	model->Render(Direct3D->GetDeviceContext());

	result = textureShader->Render(Direct3D->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, model->GetTexture());
	if (!result)
		return false;

	// Present the rendered scene to the screen.
	Direct3D->EndScene();

	return true;
}