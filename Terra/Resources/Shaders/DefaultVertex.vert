#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

//texture coordinate output for fragment shader
out vec2 texCoord;

uniform mat4 trans;

//Scale for vertices
uniform float scale;

void main()
{
    gl_Position = trans * vec4(aPos * scale, 1.0f);
    texCoord = aTex;
}