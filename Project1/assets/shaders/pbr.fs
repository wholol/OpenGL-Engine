#version 330 core

struct Light_Params
{
    vec3 LightPos;
    vec3 LightColor;
};

out vec4 FragColor;
in vec2 aTex;
in vec3 WorldPos;
in vec3 Normal;

#define MAX_LIGHTS 4    //maximum lights for scene

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

uniform Light_Params light[MAX_LIGHTS];

uniform vec3 camPos;

#define PI 3.14159

vec3 Schlick(float cos_theta , vec3 F0);
float DistributionGGX(vec3 N, vec3 H, float a); //approximates microfacets
float GeometryShlickGGX(vec3 N , vec3 V , float k);     //approimate self-shdowing (v = light dir and view dir)
float GeometrySmith(vec3 N , vec3 V, vec3 L, float k);  //approximates self-shadowing


void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);
   
    vec3 Lo = vec3(0.0);    //total light
    
    vec3 F0 = vec3(0.04);           //refraction index basically.
    F0 = mix(F0, albedo, metallic); //metallic surfaces
    

    for (int i = 0 ; i < MAX_LIGHTS; ++i)
    {
       vec3 L = normalize(light[i].LightPos - WorldPos);
       vec3 H = normalize( V + L );     //halfway vector

       //radiance calc
       float distance = length(light[i].LightPos - WorldPos);   //world space calcualtion
       float atten = 1.0 / (distance * distance);           //since we are using gamma correction.
       vec3 radiance = light[i].LightColor * atten;
        
        //Cook Torrance
        float NDF = DistributionGGX(N,H,roughness);
        float G = GeometrySmith(N,V,L,roughness);
        vec3 F = Schlick(clamp(dot(H, V), 0.0, 1.0), F0);

        vec3 specular = (NDF * G * F) / max( (4.0f * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) ) , 0.001f );
        vec3 kS = F;   //kS is the ratio that gets reflected
        
        vec3 fLambert = albedo / PI;
         
        vec3 kD = vec3(1.0) - kS;

        kD *= 1.0 - metallic;   //remove diffuse strength based on "metalness"

        float n_dot_l = max(dot(N, L), 0.0); 
        Lo += (( kD * fLambert ) + specular ) * radiance * n_dot_l;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));    //HDR
    color = pow (color , vec3(1.0 / 2.2));

    FragColor = vec4(color , 1.0);

}


//util functions
vec3 Schlick(float cos_theta , vec3 F0)
{
    return F0 + (1 - F0) * pow(max(1.0 - cos_theta , 0.0) , 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float a)
{
    float a2 = a*a;
    float n_dot_h2 = max(dot(N,H) , 0.0) * max(dot(N,H) , 0.0);

    float NDF = a2  / (PI * (n_dot_h2 * (a2 - 1) + 1 ) * (n_dot_h2 * (a2 - 1) + 1 ));

    return NDF;
}

float GeometryShlickGGX(vec3 N , vec3 V , float roughness)
{ 
    float n_dot_v = max(dot(N,V) ,0.0);

    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;      //note: this is NOT FOR IBL.

    return (n_dot_v) / ((n_dot_v) * (1 - k) + k);
}

float GeometrySmith(vec3 N , vec3 V, vec3 L, float k)
{
    float GeometryShlickGGX_view = GeometryShlickGGX(N,V,k);
    float GeometryShlickGGX_lightdir = GeometryShlickGGX(N,L,k);

    return GeometryShlickGGX_view * GeometryShlickGGX_lightdir;
}