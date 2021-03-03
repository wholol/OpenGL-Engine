#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

sampler2D blurred_image;
sampler2D non_blurred_image;


void main()
{
    vec3 result = vec4(1.0);

    result += texture(blurred_image , TexCoords).rgb;
    result += texture(non_blurred_image , TexCoords).rgb;

    FragColor = vec4(result , 1.0);



}