#version 330 core

in vec2 fragmentTextureCoord;

uniform usampler2D texture1;
uniform ivec2 inputSize;

out vec3 FragColor;

#define KERNEL_WIDTH 5
#define KERNEL_HEIGHT 5

const float kernel[KERNEL_WIDTH * KERNEL_HEIGHT] = float[]
(
    1, 4, 6, 4, 1,
    4, 16, 24, 16, 4,
    6, 24, 36, 24, 6,
    4, 16, 24, 16, 4,
    1, 4, 6, 4, 1
);
const float kernelSum = 256;

void main()
{
    vec3 color = vec3(0.0, 0.0, 0.0);
    int i = 0;
    for (int y = -(KERNEL_HEIGHT / 2); y <= KERNEL_HEIGHT / 2; ++y)
    {
        for (int x = -(KERNEL_WIDTH / 2); x <= KERNEL_WIDTH / 2; ++x)
        {
            ivec2 coord = ivec2
            (
                int(fragmentTextureCoord.x * inputSize.x) + x,
                int(fragmentTextureCoord.y * inputSize.y) + y
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
            ) * kernel[i];

            ++i;
        }
    }
    FragColor = color / kernelSum;
}