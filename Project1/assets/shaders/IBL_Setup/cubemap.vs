#version 330 core

//sets up the cube faces for the screen.

layout (location = 0) in vec3 aPos;

out vec3 WorldPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    WorldPos = aPos;
    gl_Position = projection * view * vec4(WorldPos , 1.0); //no translation/rotation for cubemaps needed.
}