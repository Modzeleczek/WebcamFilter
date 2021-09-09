#version 330 core

in vec2 fragmentTextureCoord;

uniform usampler2D texture1;
uniform ivec2 size;

out vec3 FragColor;

void main()
{
    ivec2 coord = ivec2
    (
        int(fragmentTextureCoord.x * size.x), // fragmentTextureCoord.x przyjmuje wartości z zakresu [0, 1]
        int(fragmentTextureCoord.y * size.y) // fragmentTextureCoord.y przyjmuje wartości z zakresu [0, 1]
    );

    // wyznaczamy składowe YUV fragmentu (piksela)
    float Y, U, V;
    ivec2 texelCoord = ivec2(coord.x / 2, coord.y);
    uvec4 pixelPair = texelFetch(texture1, texelCoord, 0); // lod (level-of-detail) = 0, czyli samplujemy z pełnowymiarowej tekstury (mipmapy poziomu 0); [0]=Y1; [1]=U; [2]=Y2; [3]=V
    const float denominator = 256.0;
    Y = pixelPair[ (coord.x & 1) * 2 ] / denominator; // 0 lub 2; wyjście jest w formacie RGBA, a wejście YUV422; w jednym wywołaniu fragment shadera wyznaczamy kolor lewego piksela wyjścia (wtedy Y = pixelPair[0]), a w drugim prawego (wtedy Y = pixelPair[2])
    U = pixelPair[1] / denominator; // dzielimy przez 256, bo tekstura ma internalformat GL_RGBA8UI, czyli przesyłamy z RAMu czwórki bajtów o wartościach z przedziału [0, 255] (bez normalizacji do [0, 1])
    V = pixelPair[3] / denominator;

    // zamieniamy składowe YUV na RGB
    Y = 1.1643*(Y - 0.0625); // Y - 16/256.0
    U = U - 0.5; // U - 128/256.0
    V = V - 0.5; // V - 128/256.0
    FragColor = vec3
    (
        Y + 1.5958*V,
        Y - 0.39173*U - 0.81290*V,
        Y + 2.017*U
    );
}