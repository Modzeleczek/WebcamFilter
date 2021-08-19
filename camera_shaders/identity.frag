#version 330 core

in vec2 fragmentTextureCoord;

uniform sampler2D texture1;
uniform ivec2 size;

out vec4 FragColor;

void main()
{
    ivec2 texelCoord = ivec2
    (
        int(fragmentTextureCoord.x * size.x),
        int(fragmentTextureCoord.y * size.y)
    );
    FragColor = texelFetch(texture1, texelCoord, 0);
}