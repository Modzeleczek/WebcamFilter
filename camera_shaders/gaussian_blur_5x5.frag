#version 330 core

in vec2 fragmentTextureCoord;

uniform usampler2D texture1;
uniform ivec2 size;

out uvec4 FragColor;

#define KERNEL_WIDTH 5
#define KERNEL_HEIGHT 5
#define index(x, y) (x + KERNEL_WIDTH * y)

const int kernel[KERNEL_WIDTH * KERNEL_HEIGHT] = int[]
(
    1, 4, 6, 4, 1,
    4, 16, 24, 16, 4,
    6, 24, 36, 24, 6,
    4, 16, 24, 16, 4,
    1, 4, 6, 4, 1
);
const int kernelSum = 256;

void main()
{
    ivec2 texelCoord = ivec2
    (
        int(fragmentTextureCoord.x * size.x),
        int(fragmentTextureCoord.y * size.y)
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

    // konwolucja "na piechotę"
    // float Y1 = 0; // jasność lewego piksela z aktualnego teksela (pary pikseli)
    // // 0 wiersz
    // Y1 += texels[ index(KERNEL_WIDTH / 2 - 1, KERNEL_HEIGHT / 2 - 2) ][0] * kernel[0];
    // Y1 += texels[ index(KERNEL_WIDTH / 2 - 1, KERNEL_HEIGHT / 2 - 2) ][2] * kernel[1];
    // Y1 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 - 2) ][0] * kernel[2];
    // Y1 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 - 2) ][2] * kernel[3];
    // Y1 += texels[ index(KERNEL_WIDTH / 2 + 1, KERNEL_HEIGHT / 2 - 2) ][0] * kernel[4];
    // // 1 wiersz
    // Y1 += texels[ index(KERNEL_WIDTH / 2 - 1, KERNEL_HEIGHT / 2 - 1) ][0] * kernel[5];
    // Y1 += texels[ index(KERNEL_WIDTH / 2 - 1, KERNEL_HEIGHT / 2 - 1) ][2] * kernel[6];
    // Y1 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 - 1) ][0] * kernel[7];
    // Y1 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 - 1) ][2] * kernel[8];
    // Y1 += texels[ index(KERNEL_WIDTH / 2 + 1, KERNEL_HEIGHT / 2 - 1) ][0] * kernel[9];
    // // 2 wiersz
    // Y1 += texels[ index(KERNEL_WIDTH / 2 - 1, KERNEL_HEIGHT / 2) ][0] * kernel[10];
    // Y1 += texels[ index(KERNEL_WIDTH / 2 - 1, KERNEL_HEIGHT / 2) ][2] * kernel[11];
    // Y1 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2) ][0] * kernel[12];
    // Y1 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2) ][2] * kernel[13];
    // Y1 += texels[ index(KERNEL_WIDTH / 2 + 1, KERNEL_HEIGHT / 2) ][0] * kernel[14];
    // // 3 wiersz
    // Y1 += texels[ index(KERNEL_WIDTH / 2 - 1, KERNEL_HEIGHT / 2 + 1) ][0] * kernel[15];
    // Y1 += texels[ index(KERNEL_WIDTH / 2 - 1, KERNEL_HEIGHT / 2 + 1) ][2] * kernel[16];
    // Y1 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 + 1) ][0] * kernel[17];
    // Y1 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 + 1) ][2] * kernel[18];
    // Y1 += texels[ index(KERNEL_WIDTH / 2 + 1, KERNEL_HEIGHT / 2 + 1) ][0] * kernel[19];
    // // 4 wiersz
    // Y1 += texels[ index(KERNEL_WIDTH / 2 - 1, KERNEL_HEIGHT / 2 + 2) ][0] * kernel[20];
    // Y1 += texels[ index(KERNEL_WIDTH / 2 - 1, KERNEL_HEIGHT / 2 + 2) ][2] * kernel[21];
    // Y1 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 + 2) ][0] * kernel[22];
    // Y1 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 + 2) ][2] * kernel[23];
    // Y1 += texels[ index(KERNEL_WIDTH / 2 + 1, KERNEL_HEIGHT / 2 + 2) ][0] * kernel[24];

    // float Y2 = 0; // jasność prawego piksela z aktualnego teksela (pary pikseli)
    // // 0 wiersz
    // Y2 += texels[ index(KERNEL_WIDTH / 2 - 1, KERNEL_HEIGHT / 2 - 2) ][2] * kernel[0];
    // Y2 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 - 2) ][0] * kernel[1];
    // Y2 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 - 2) ][2] * kernel[2];
    // Y2 += texels[ index(KERNEL_WIDTH / 2 + 1, KERNEL_HEIGHT / 2 - 2) ][0] * kernel[3];
    // Y2 += texels[ index(KERNEL_WIDTH / 2 + 1, KERNEL_HEIGHT / 2 - 2) ][2] * kernel[4];
    // // 1 wiersz
    // Y2 += texels[ index(KERNEL_WIDTH / 2 - 1, KERNEL_HEIGHT / 2 - 1) ][2] * kernel[5];
    // Y2 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 - 1) ][0] * kernel[6];
    // Y2 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 - 1) ][2] * kernel[7];
    // Y2 += texels[ index(KERNEL_WIDTH / 2 + 1, KERNEL_HEIGHT / 2 - 1) ][0] * kernel[8];
    // Y2 += texels[ index(KERNEL_WIDTH / 2 + 1, KERNEL_HEIGHT / 2 - 1) ][2] * kernel[9];
    // // 2 wiersz
    // Y2 += texels[ index(KERNEL_WIDTH / 2 - 1, KERNEL_HEIGHT / 2) ][2] * kernel[10];
    // Y2 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2) ][0] * kernel[11];
    // Y2 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2) ][2] * kernel[12];
    // Y2 += texels[ index(KERNEL_WIDTH / 2 + 1, KERNEL_HEIGHT / 2) ][0] * kernel[13];
    // Y2 += texels[ index(KERNEL_WIDTH / 2 + 1, KERNEL_HEIGHT / 2) ][2] * kernel[14];
    // // 3 wiersz
    // Y2 += texels[ index(KERNEL_WIDTH / 2 - 1, KERNEL_HEIGHT / 2 + 1) ][2] * kernel[15];
    // Y2 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 + 1) ][0] * kernel[16];
    // Y2 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 + 1) ][2] * kernel[17];
    // Y2 += texels[ index(KERNEL_WIDTH / 2 + 1, KERNEL_HEIGHT / 2 + 1) ][0] * kernel[18];
    // Y2 += texels[ index(KERNEL_WIDTH / 2 + 1, KERNEL_HEIGHT / 2 + 1) ][2] * kernel[19];
    // // 4 wiersz
    // Y2 += texels[ index(KERNEL_WIDTH / 2 - 1, KERNEL_HEIGHT / 2 + 2) ][2] * kernel[20];
    // Y2 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 + 2) ][0] * kernel[21];
    // Y2 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 + 2) ][2] * kernel[22];
    // Y2 += texels[ index(KERNEL_WIDTH / 2 + 1, KERNEL_HEIGHT / 2 + 2) ][0] * kernel[23];
    // Y2 += texels[ index(KERNEL_WIDTH / 2 + 1, KERNEL_HEIGHT / 2 + 2) ][2] * kernel[24];

    // konwolucja w pętlach
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