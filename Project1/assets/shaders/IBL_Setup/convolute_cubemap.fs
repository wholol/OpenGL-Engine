#version 330 core

out vec4 FragColor;
in vec3 WorldPos;  //position in world space

uniform samplerCube environmentMap; //sample from pre-convoluted cubemap

const float PI = 3.14159;

void main()
{
    vec3 Normal = normalize(WorldPos);

    vec3 irradiance = vec3(0.0);

    vec3 up = vec3(0.0 , 1.0 , 0.0);
    vec3 right = cross(up , Normal);
    up = cross(Normal, right);

    float samples = 0.0f;
    float sampledt = 0.025f;
    
    //reimann sum here
    for (float phi = 0.0f ; phi < 2.0 * PI ; phi += sampledt)       //latitude
    {
        for (float theta = 0.0f ; theta < 0.5 * PI ; theta += sampledt)
        {
            vec3 tangentSample = vec3(sin(theta) * cos(phi) , sin(theta) * sin (phi) , cos(theta));
            
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * Normal;
            irradiance += texture(environmentMap , sampleVec).rgb * cos(theta) * sin(theta);
            ++samples;
        }
    }

    irradiance = PI * irradiance * (1.0 / float(samples));

    FragColor = vec4(irradiance , 1.0);

}