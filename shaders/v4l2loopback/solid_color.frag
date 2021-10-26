#version 330 core

in vec2 fragmentTextureCoord;

uniform usampler2D texture1;
uniform ivec2 inputSize;

out uvec4 FragColor;

void main()
{
    // Y1  U   Y2  V
    // 255 255 255 0   - niebieski (cyjan)
    // 255 0   255 255 - żółty
    // 255 255 255 255 - różowy
    // FragColor = uvec4(128, 0, 64, 255); // Y1, U, Y2, V
    // return;

    const float R = 0, G = 1, B = 0;
    // zamieniamy RGB na YUV422
    const float Y = 0.299*R + 0.587*G + 0.114*B;
    const float U = 0.493*(B - Y);
    const float V = 0.877*(R - Y);
    FragColor = uvec4(clamp(vec4(Y, U, Y, V) * 256, 0, 255));
}