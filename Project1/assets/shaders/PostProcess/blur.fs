#version 330 core

in vec2 TexCoords;

out FragColor;

uniform sampler2D image;            //sample from other pingpong texture

uniform bool horizontal;
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{
    vec3 result = texture(image, TexCoords).rgb * weight[0];

    vec2 texel_size = 1.0 / textureSize(image , 0);

    //let TexCoord be the center of the kernel, then we sample both let and right side
    if (horizontal)
    {
        for (int i = 1; i < 5 ;++i)
        {
            result += texture(image , TexCoords.x + (texel_size * i)).rgb * weight[i];    //sample RHS
            result += texture(image , TexCoords.x - (texel_size * i)).rgb * weight[i];      //sample LHS
        }
    }

    else{

        for (int i = 1; i < 5 ;++i)
        {
            result += texture(image , TexCoords.y + (texel_size * i)).rgb * weight[i];    //sample RHS
            result += texture(image , TexCoords.y - (texel_size * i)).rgb * weight[i];
        }
    }
    FragColor = vec4(result , 1.0);
}