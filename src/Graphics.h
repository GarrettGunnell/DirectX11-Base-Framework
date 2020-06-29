#include "d3d.h"
#include "Camera.h"
#include "Model.h"
#include "TextureShader.h"


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics {
public:
	Graphics();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();
	D3D* Direct3D;
	Camera* camera;
	Model* model;
	TextureShader* textureShader;
};