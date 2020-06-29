cbuffer MatrixBuffer {
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInput {
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInput {
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

PixelInput vsMain(VertexInput input) {
    PixelInput output;

    // Proper matrix calculations require a w value of 1 not 0
    input.position.w = 1.0f;

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.tex = input.tex;

    return output;
}