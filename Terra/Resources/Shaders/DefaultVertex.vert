#version 330 core
//Coordinates
layout (location = 0) in vec3 aPos;
//Texture coordinates
layout (location = 1) in vec2 aTex;

//texture coordinate output for fragment shader
out vec2 texCoord;

//Scale for vertices
uniform float scale;

void main()
{
    gl_Position = vec4(aPos.x * scale, aPos.y * scale , aPos.z * scale, 1.0);
    texCoord = aTex;
}