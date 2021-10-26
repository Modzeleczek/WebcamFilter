#version 330 core

in vec2 fragmentTextureCoord;

uniform usampler2D texture1;
uniform ivec2 inputSize;

out vec3 FragColor;

void main()
{
    ivec2 coord = ivec2
    (
        int(fragmentTextureCoord.x * inputSize.x),
        int(fragmentTextureCoord.y * inputSize.y)
    );

    float Y, U, V;
    ivec2 texelCoord = ivec2(coord.x / 2, coord.y);
    uvec4 pixelPair = texelFetch(texture1, texelCoord, 0);
    const float denominator = 256.0;
    Y = pixelPair[ (coord.x & 1) * 2 ] / denominator;
    U = pixelPair[1] / denominator;
    V = pixelPair[3] / denominator;

    Y = 1.1643*(Y - 0.0625);
    U = U - 0.5;
    V = V - 0.5;
    FragColor = 1.0 - vec3
    (
        Y + 1.5958*V,
        Y - 0.39173*U - 0.81290*V,
        Y + 2.017*U
    );
}