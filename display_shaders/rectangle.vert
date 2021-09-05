#version 330 core

layout (location = 0) in vec2 vertexCoord;
layout (location = 1) in vec2 textureCoord;

out vec2 fragmentTextureCoord;

void main()
{
    fragmentTextureCoord = textureCoord; // w fragmentTextureCoord przekazujemy do fragment shadera pozycję fragmentu na teksturze zinterpolowaną liniowo z pozycji wierzchołka, dla którego wykonywany jest vertex shader
    gl_Position = vec4(vertexCoord, 0.0, 1.0);
}