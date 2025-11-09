#version 330 core
out vec4 FragColor;

in vec2  v_TextureCoord;
in vec4  v_Color;
in float v_TextureID;
in float v_ShouldUseTexture;

uniform sampler2D u_Textures[32];

void main()
{
    bool bUseTexture = v_ShouldUseTexture > 0.0f;
    int index = int(v_TextureID);
    if(bUseTexture)
    {
        FragColor = texture(u_Textures[index],v_TextureCoord);
    }
    else
    {
        FragColor = v_Color;
    }
}