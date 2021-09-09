#version 330 core

in vec2 fragmentTextureCoord;

uniform usampler2D texture1;
uniform ivec2 size;

out vec3 FragColor;

void main()
{
    ivec2 coord = ivec2
    (
        size.x - int(fragmentTextureCoord.x * size.x),
        int(fragmentTextureCoord.y * size.y)
    );

    float Y, U, V;
    ivec2 texelCoord = ivec2(coord.x / 2, coord.y);
    vec4 pixelPair = texelFetch(texture1, texelCoord, 0);
    Y = pixelPair[ (coord.x & 1) * 2 ] / 256.0;
    U = pixelPair[1] / 256.0;
    V = pixelPair[3] / 256.0;

    Y = 1.1643*(Y - 0.0625);
    U = U - 0.5;
    V = V - 0.5;
    FragColor = vec3
    (
        Y + 1.5958*V,
        Y - 0.39173*U - 0.81290*V,
        Y + 2.017*U
    );
}