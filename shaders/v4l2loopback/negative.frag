#version 330 core

in vec2 fragmentTextureCoord;

uniform usampler2D texture1;
uniform ivec2 inputSize;

out uvec4 FragColor;

void main()
{
    ivec2 texelCoord = ivec2
    (
        int(fragmentTextureCoord.x * inputSize.x),
        int(fragmentTextureCoord.y * inputSize.y)
    );
    uvec4 pixelPair = texelFetch(texture1, texelCoord, 0); // [0]=Y1; [1]=U; [2]=Y2; [3]=V

    FragColor.xz = 255u - pixelPair.xz; // Y1 i Y2
    FragColor.yw = pixelPair.yw; // U i V
}