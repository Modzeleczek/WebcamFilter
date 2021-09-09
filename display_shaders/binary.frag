#version 330 core

in vec2 fragmentTextureCoord;

uniform usampler2D texture1;
uniform ivec2 size;

out vec3 FragColor;

void main()
{
    ivec2 coord = ivec2
    (
        int(fragmentTextureCoord.x * size.x),
        int(fragmentTextureCoord.y * size.y)
    );

    ivec2 texelCoord = ivec2(coord.x / 2, coord.y);
    uvec4 pixelPair = texelFetch(texture1, texelCoord, 0);
    float Y = pixelPair[ (coord.x & 1) * 2 ];
    FragColor = vec3(1, 1, 1) * step(128, Y);
}