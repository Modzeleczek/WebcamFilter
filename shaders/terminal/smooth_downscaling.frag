#version 330 core

in vec2 fragmentTextureCoord;

uniform usampler2D texture1;
uniform ivec2 inputSize;
uniform ivec2 scale;

out uint FragColor;

#define NO_OF_CHARACTERS 9
// .  :  -  =  +  *  #  %  @
// 46 58 45 61 43 42 35 37 64
const uint characters[NO_OF_CHARACTERS] = uint[] ( 46u, 58u, 45u, 61u, 43u, 42u, 35u, 37u, 64u );

void main()
{
    ivec2 topLeftCoord = ivec2 // indeks x i y piksela wejścia, który znajduje się w lewym górnym rogu aktualnie renderowanego piksela wyjścia
    (
        int(fragmentTextureCoord.x * inputSize.x),
        int(fragmentTextureCoord.y * inputSize.y)
    );

    int sum = 0;
    for (int y = 0; y < scale.y; ++y)
    {
        for (int x = 0; x < scale.x; ++x)
        {
            ivec2 coord = ivec2 // indeks x i y aktualnie analizowanego piksela wejścia, który znajduje się w obszarze aktualnie renderowanego piksela wyjścia
            (
                topLeftCoord.x + x,
                topLeftCoord.y + y
            );
            ivec2 texelCoord = ivec2(coord.x / 2, coord.y); // indeks x i y teksela tekstury, w którym znajduje się piksel wejścia
            sum += int(texelFetch(texture1, texelCoord, 0)[ (coord.x & 1) * 2 ]); // Y piksela wejścia
        }
    }
    sum = sum / (scale.x * scale.y);
    FragColor = characters[ (sum * NO_OF_CHARACTERS) >> 8 ]; // / 256
}