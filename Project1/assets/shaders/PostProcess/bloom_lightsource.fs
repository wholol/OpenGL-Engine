#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform vec3 LightColor;

void main()
{
    FragColor = vec4(LightColor , 1.0);

    float brightness = dot(FragColor.rgb , vec3(0.2126, 0.7152, 0.0722));

    //extract the brightness into another color buffer
    if (brightness > 1.0)
    {
        BrightColor = vec4(FragColor.rgb , 1.0);
    }
    else
    {
        BrightColor = vec4(0.0,0.0,0.0, 1.0);
    }

}