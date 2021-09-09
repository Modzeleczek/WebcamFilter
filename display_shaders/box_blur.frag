#version 330 core

in vec2 fragmentTextureCoord;

uniform usampler2D texture1;
uniform ivec2 size;

out vec3 FragColor;

void main()
{
    int blurWidth = 11;
    int blurWidthHalf = blurWidth / 2;
    vec3 color = vec3(0.0, 0.0, 0.0);
    for (int y = -blurWidthHalf; y <= blurWidthHalf; ++y)
    {
        for (int x = -blurWidthHalf; x <= blurWidthHalf; ++x)
        {
            ivec2 coord = ivec2
            (
                int(fragmentTextureCoord.x * size.x) + x,
                int(fragmentTextureCoord.y * size.y) + y
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
            color += vec3
            (
                Y + 1.5958*V,
                Y - 0.39173*U - 0.81290*V,
                Y + 2.017*U
            );
        }
    }
    FragColor = color / (blurWidth * blurWidth);
}