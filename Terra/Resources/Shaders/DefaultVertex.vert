#version 330 core

layout (location = 0) in vec3   a_Position;
layout (location = 1) in vec4   a_Color;
layout (location = 2) in vec2   a_Texture;
layout (location = 3) in float  a_TextureID;
layout (location = 4) in float  a_ShouldUseTexture;
layout (location = 5) in float  a_FlipX;
layout (location = 6) in float  a_FlipY;

//texture coordinate output for fragment shader
out vec2  v_TextureCoord;
out vec4  v_Color;
out float v_TextureID;
out float v_ShouldUseTexture;

uniform mat4 u_ProjectionViewMatrix;

void main()
{
    gl_Position = u_ProjectionViewMatrix * vec4(a_Position, 1.0f);
    v_Color = a_Color;
    v_TextureCoord = a_Texture;
    v_TextureID = a_TextureID;
    v_ShouldUseTexture = a_ShouldUseTexture;

    bool bFlipX = a_FlipX > 0.0f;
    bool bFlipY = a_FlipY > 0.0f;
    if(bFlipX)
    {
       v_TextureCoord.x = 1.0 - v_TextureCoord.x; 
    }
    if(bFlipY)
    {
        v_TextureCoord.y = 1.0 - v_TextureCoord.y;
    }
}