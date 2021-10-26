#version 330 core

in vec2 fragmentTextureCoord;

uniform usampler2D texture1;
uniform ivec2 inputSize;

out uint FragColor;

#define NO_OF_CHARACTERS 9
// .  :  -  =  +  *  #  %  @
// 46 58 45 61 43 42 35 37 64
const uint characters[NO_OF_CHARACTERS] = uint[] ( 46u, 58u, 45u, 61u, 43u, 42u, 35u, 37u, 64u );

void main()
{
    // brak skalowania, czyli klatka wyjściowa musi mieć tyle samo pikseli co klatka wejściowa
    ivec2 coord = ivec2
    (
        int(fragmentTextureCoord.x * inputSize.x), // fragmentTextureCoord.x przyjmuje wartości z zakresu [0, 1]
        int(fragmentTextureCoord.y * inputSize.y) // fragmentTextureCoord.y przyjmuje wartości z zakresu [0, 1]
    );
    // wyznaczamy składową Y (tylko jasność) fragmentu (piksela)
    ivec2 texelCoord = ivec2(coord.x / 2, coord.y);
    int Y = int(texelFetch(texture1, texelCoord, 0)[ (coord.x & 1) * 2 ]);
    FragColor = characters[ (Y * NO_OF_CHARACTERS) >> 8 ]; // / 256
}