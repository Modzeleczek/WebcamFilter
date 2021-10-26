#version 330 core

in vec2 fragmentTextureCoord;

uniform usampler2D texture1;
uniform ivec2 inputSize;

out vec3 FragColor;

void main()
{
    // wyjście jest w formacie RGBA, ale FragColor jest typu vec3 a nie vec4, więc A jest automatycznie ustawione na nieprzezroczyste
    FragColor = vec3(0, 1, 0);
}