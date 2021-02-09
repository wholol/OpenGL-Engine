#version 330 core
out FragColor;

struct Material
{ sampler2D diffuse;
  sampler2D specular;
  float shininess;
 };
 
 struct DirLight
 {
 vec3 direction;
 vec3 ambient;			
 vec3 diffuse;			
 vec3 specular;		
 };
 
 struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirlight;
uniform PointLight pointlight[NR_POINT_LIGHTS];
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
vec3 lightdir = normalize(-light.direction);
float diff = max(dot(normal , lightdir) , 0.0);	//diff

//specular
vec3 reflectDir = reflect(-lightdir , normal);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

vec3 ambient = light.ambient * vec3(texture(material.ambient , TexCoords));
vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse , TexCoords));
vec3 specular = light.specular * spec * vec3(texture(material.specular , TexCoords));

return (ambient + diffuse + specular);

}

vec3 CalcPointLight(PointLight light , vec3 normal , vec3 fragPos, vec3 viewDir)
{
	vec3 lightdir = normalize(light.position - fragPos);
	
	float diff = max(dot(normal , lightdir) ,0.0);
	
	vec3 reflectDir = reflect(lightdir , normal);
	float spec = pow (max(dot(reflectDir , viewDir) , 0.0) , material.specular);
	
	vec3 ambient = light.ambient * vec3(texture(material.ambient , TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse , TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular , TexCoords));
	
	return (ambient + diffuse + specular);
}


void main()
{

vec3 norm = normalize(Normal);
vec3 viewDir = normalize(viewPos - FragPos);
vec3 result = CalcDirLight(dirlight , norm , viewDir);	//calcalate directional light

	for (int i = 0 ; i < NR_POINT_LIGHTS; ++i)
	{
		result += CalcPointLight(pointlight[i] , norm , FragPos, viewDir);
	}
	
	FragColor = vec4(result , 1.0);

};