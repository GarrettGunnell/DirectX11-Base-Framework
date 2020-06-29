cbuffer MatrixBuffer {
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct vInput {
	float4 position : POSITION;
	float4 color : COLOR;
}

struct pInput {
	float4 position : SV_POSITION;
	float4 color : COLOR;
}

pInput vsMain(vInput IN) {
	pInput OUT;

	IN.position.w = 1.0f;

	OUT.position = mul(IN.position, worldMatrix);
	OUT.position = mul(OUT.position, viewMatrix);
	OUT.position = mul(OUT.position, projectionMatrix);

	OUT.color = IN.color;

	return OUT;
}