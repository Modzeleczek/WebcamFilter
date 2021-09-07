#version 330 core

in vec2 fragmentTextureCoord;

uniform usampler2D texture1; // indeks texture unita
uniform ivec2 size; // szerokość i wysokość obrazu w pikselach

out uvec4 FragColor;

void main()
{
    ivec2 texelCoord = ivec2
    (
        int(fragmentTextureCoord.x * size.x), // fragmentTextureCoord.x przyjmuje wartości z zakresu [0, 0.5]
        int(fragmentTextureCoord.y * size.y) // fragmentTextureCoord.y przyjmuje wartości z zakresu [0, 1]
    );
    FragColor = texelFetch(texture1, texelCoord, 0);
}