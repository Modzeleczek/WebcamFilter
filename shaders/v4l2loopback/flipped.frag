#version 330 core

in vec2 fragmentTextureCoord;

uniform usampler2D texture1;
uniform ivec2 inputSize;

out uvec4 FragColor;

void main()
{
    ivec2 texelCoord = ivec2
    (
        inputSize.x / 2 - int(fragmentTextureCoord.x * inputSize.x), // fragmentTextureCoord.x przyjmuje wartości z zakresu [0, 0.5]
        int(fragmentTextureCoord.y * inputSize.y) // fragmentTextureCoord.y przyjmuje wartości z zakresu [0, 1]
    );
    uvec4 pixelPair = texelFetch(texture1, texelCoord, 0); // wejście: [0]=Y1; [1]=U; [2]=Y2; [3]=V
    // FragColor[0] = pixelPair[2]; // Y1 wyjścia = Y2 wejścia
    // FragColor[1] = pixelPair[1]; // U wyjścia = U wejścia
    // FragColor[2] = pixelPair[0]; // Y2 wyjścia = Y1 wejścia
    // FragColor[3] = pixelPair[3]; // V wyjścia = V wejścia
    FragColor = pixelPair.zyxw;
}