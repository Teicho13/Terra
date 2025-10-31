#version 330 core
out vec4 FragColor;

in vec2  v_TextureCoord;
in vec4  v_Color;
flat in int v_TextureID;

uniform sampler2D u_texture;

void main()
{
    if(v_TextureID == 0)
    {
        FragColor = v_Color;
    }
    else
    {
        FragColor = texture(u_texture,v_TextureCoord);
    }
}