#version 330 core

in vec2 fragmentTextureCoord;

uniform usampler2D texture1;
uniform ivec2 inputSize;

out uvec4 FragColor;

#define KERNEL_WIDTH 3
#define KERNEL_HEIGHT 3
#define index(x, y) (x + KERNEL_WIDTH * y)

const int kernel[KERNEL_WIDTH * KERNEL_HEIGHT] = int[]
(
    1, 2, 1,
    2, 4, 2,
    1, 2, 1
);
const int kernelSum = 16;

void main()
{
    ivec2 texelCoord = ivec2
    (
        int(fragmentTextureCoord.x * inputSize.x),
        int(fragmentTextureCoord.y * inputSize.y)
    );

    ivec4 texels[KERNEL_WIDTH * KERNEL_HEIGHT];
    int i = 0;
    for (int y = -KERNEL_HEIGHT / 2; y <= KERNEL_HEIGHT / 2; ++y)
    {
        for (int x = -KERNEL_WIDTH / 2; x <= KERNEL_WIDTH / 2; ++x)
        {
            texels[i] = ivec4(texelFetch(texture1, texelCoord + ivec2(x, y), 0));
            ++i;
        }
    }

    int U = 0, V = 0;
    i = 0;
    for (int y = 0; y <= KERNEL_HEIGHT - 1; ++y)
    {
        for (int x = 0; x <= KERNEL_WIDTH - 1; ++x)
        {
            U += texels[i][1] * kernel[i];
            V += texels[i][3] * kernel[i];
            ++i;
        }
    }

    int Y1 = 0;
    int k = 0;
    for (int y = 0; y <= KERNEL_HEIGHT - 1; ++y)
    {
        for (int x = (KERNEL_WIDTH - 1) - KERNEL_WIDTH / 2; x <= (KERNEL_WIDTH - 1) + KERNEL_WIDTH / 2; ++x)
        {
            Y1 += texels[ index(x / 2, y) ][ (x & 1) * 2 ] * kernel[k];
            ++k;
        }
    }

    int Y2 = 0;
    k = 0;
    for (int y = 0; y <= KERNEL_HEIGHT - 1; ++y)
    {
        for (int x = KERNEL_WIDTH - KERNEL_WIDTH / 2; x <= KERNEL_WIDTH + KERNEL_WIDTH / 2; ++x)
        {
            Y2 += texels[ index(x / 2, y) ][ (x & 1) * 2 ] * kernel[k];
            ++k;
        }
    }

    FragColor = min(uvec4(Y1, U, Y2, V) / uint(kernelSum), 255u);
}