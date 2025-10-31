#version 330 core

layout (location = 0) in vec3   a_Position;
layout (location = 1) in vec4   a_Color;
layout (location = 2) in vec2   a_Texture;
layout (location = 3) in int    a_TextureID;

//texture coordinate output for fragment shader
out vec2  v_TextureCoord;
out vec4  v_Color;
flat out int v_TextureID;

uniform mat4 u_ProjectionViewMatrix;

void main()
{
    gl_Position = u_ProjectionViewMatrix * vec4(a_Position, 1.0f);
    v_Color = a_Color;
    v_TextureCoord = a_Texture;
    v_TextureID = a_TextureID;
}