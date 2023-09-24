#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 crntPos;

in vec3 color;

in vec2 texCoord;


uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
	float ambient = 0.2f;
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);

	// diffuse lighting
	float diffuse = max(dot(normal, lightDirection), 0.0f); // take max so no negatives happen

	float specularLight = 0.50f;													// maximum specular intensity
	vec3 viewDirection = normalize(camPos - crntPos);								// view direction towards current pixel
	vec3 reflectionDirection = reflect(-lightDirection, normal);					// reflect negative so we can get the vector facing the plane 
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);	// larger direction between the view and reflect direction, make the specular light weaker  
	float specular = specAmount * specularLight;									// find the specular to add to the ambient and diffuse

	FragColor = texture(diffuse0, texCoord) * lightColor * (diffuse + ambient + specular);

}