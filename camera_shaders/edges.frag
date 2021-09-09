#version 330 core

in vec2 fragmentTextureCoord;

uniform usampler2D texture1; // indeks texture unita
uniform ivec2 size; // szerokość i wysokość obrazu w pikselach

out uvec4 FragColor;

#define KERNEL_WIDTH 3
#define KERNEL_HEIGHT 3
#define index(x, y) (x + KERNEL_WIDTH * y)

// dopiero w OpenGL 4.3 można definiować tablice tablic
// const float kernel[KERNEL_HEIGHT][KERNEL_WIDTH] = float[][]
// (
//     float[](-1, -1, -1),
//     float[](-1, 8, -1),
//     float[](-1, -1, -1)
// );

const int kernel[KERNEL_WIDTH * KERNEL_HEIGHT] = int[]
(
    -1, -1, -1,
    -1, 8, -1,
    -1, -1, -1
);

void main()
{
    ivec2 texelCoord = ivec2
    (
        int(fragmentTextureCoord.x * size.x),
        int(fragmentTextureCoord.y * size.y)
    );
    // texelFetch(texture1, texelCoord, 0); // [0]=Y1; [1]=U; [2]=Y2; [3]=V

    ivec4 texels[KERNEL_WIDTH * KERNEL_HEIGHT];
    int i = 0;
    for (int y = -KERNEL_HEIGHT / 2; y <= KERNEL_HEIGHT / 2; ++y)
    {
        for (int x = -KERNEL_WIDTH / 2; x <= KERNEL_WIDTH / 2; ++x)
        {
            texels[i] = ivec4(texelFetch(texture1, texelCoord + ivec2(x, y), 0)); // rzutujemy 8-bitowy uint na 32-bitowy int
            ++i;
        }
    }

    int U = 0, V = 0;
    i = 0;
    for (int y = 0; y <= KERNEL_HEIGHT - 1; ++y) // obliczamy konwolucję na składowych koloru (U i V), sumując po tekselach (parach pikseli) otaczających aktualny teksel o współrzędnych texelCoord
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
    // // górny wiersz
    // Y1 += texels[ index(KERNEL_WIDTH / 2 - 1, KERNEL_HEIGHT / 2 - 1) ][2] * kernel[0];
    // Y1 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 - 1) ][0] * kernel[1];
    // Y1 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 - 1) ][2] * kernel[2];
    // // środkowy wiersz
    // Y1 += texels[ index(KERNEL_WIDTH / 2 - 1, KERNEL_HEIGHT / 2) ][2] * kernel[3];
    // Y1 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2) ][0] * kernel[4];
    // Y1 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2) ][2] * kernel[5];
    // // dolny wiersz
    // Y1 += texels[ index(KERNEL_WIDTH / 2 - 1, KERNEL_HEIGHT / 2 + 1) ][2] * kernel[6];
    // Y1 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 + 1) ][0] * kernel[7];
    // Y1 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 + 1) ][2] * kernel[8];

    // float Y2 = 0; // jasność prawego piksela z aktualnego teksela (pary pikseli)
    // // górny wiersz
    // Y2 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 - 1) ][0] * kernel[0];
    // Y2 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 - 1) ][2] * kernel[1];
    // Y2 += texels[ index(KERNEL_WIDTH / 2 + 1, KERNEL_HEIGHT / 2 - 1) ][0] * kernel[2];
    // // środkowy wiersz
    // Y2 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2) ][0] * kernel[3];
    // Y2 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2) ][2] * kernel[4];
    // Y2 += texels[ index(KERNEL_WIDTH / 2 + 1, KERNEL_HEIGHT / 2) ][0] * kernel[5];
    // // dolny wiersz
    // Y2 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 + 1) ][0] * kernel[6];
    // Y2 += texels[ index(KERNEL_WIDTH / 2, KERNEL_HEIGHT / 2 + 1) ][2] * kernel[7];
    // Y2 += texels[ index(KERNEL_WIDTH / 2 + 1, KERNEL_HEIGHT / 2 + 1) ][0] * kernel[8];

    // konwolucja w pętlach
    int Y1 = 0;
    int k = 0;
    for (int y = 0; y <= KERNEL_HEIGHT - 1; ++y) // obliczamy konwolucję na składowej jasności lewego piksela (Y1) z aktualnego teksela (pary pikseli), sumując po pikselach otaczających aktualny piksel
    {
        // int x = (KERNEL_WIDTH / 2) * 2;
        // równoważne, bo KERNEL_WIDTH zawsze jest nieparzyste
        // int x = KERNEL_WIDTH - 1;
        // obliczamy X aktualnego piksela (lewego z aktualnego teksela) i przesuwamy się do lewej krawędzi aktualnego obszaru pod kernelem
        for (int x = (KERNEL_WIDTH - 1) - KERNEL_WIDTH / 2; x <= (KERNEL_WIDTH - 1) + KERNEL_WIDTH / 2; ++x)
        {                                    // 0 lub 2
            Y1 += texels[ index(x / 2, y) ][ (x & 1) * 2 ] * kernel[k];
            ++k;
        }
    }

    int Y2 = 0;
    k = 0;
    for (int y = 0; y <= KERNEL_HEIGHT - 1; ++y) // obliczamy konwolucję na składowej jasności lewego piksela (Y1) z aktualnego teksela (pary pikseli), sumując po pikselach otaczających aktualny piksel
    {
        // int x = (KERNEL_WIDTH / 2) * 2 + 1;
        // równoważne, bo KERNEL_WIDTH zawsze jest nieparzyste
        // int x = KERNEL_WIDTH;
        // obliczamy X aktualnego piksela (prawego z aktualnego teksela) i przesuwamy się do lewej krawędzi aktualnego obszaru pod kernelem
        for (int x = KERNEL_WIDTH - KERNEL_WIDTH / 2; x <= KERNEL_WIDTH + KERNEL_WIDTH / 2; ++x)
        {                                    // 0 lub 2
            Y2 += texels[ index(x / 2, y) ][ (x & 1) * 2 ] * kernel[k];
            ++k;
        }
    }

    // środkowy teksel obszaru, nad którym aktualnie znajduje się kernel konwolucji
    FragColor = uvec4(clamp(ivec4(Y1, U, Y2, V), 0, 255));
}