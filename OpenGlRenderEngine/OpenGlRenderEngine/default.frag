#version 330 core
out vec4 FragColor;

in vec3 Normal;

in vec3 color;

in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

void main()
{
   FragColor = texture(diffuse0, texCoord);
}