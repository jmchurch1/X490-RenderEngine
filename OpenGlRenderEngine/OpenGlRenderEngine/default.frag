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

vec4 pointLight()
{
	// get the current distance between the light and pixel
	vec3 lightVec = lightPos - crntPos;
	float dist = length(lightVec);
	float a = 500.0f;						// first constant
	float b = 0.001f;						// second constant
	float intensity = 1.0f / (a * dist * dist + b * dist + 1.0f);

	float ambient = 0.2f;
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);

	// diffuse lighting
	float diffuse = max(dot(normal, lightDirection), 0.0f); // take max so no negatives happen

	float specularLight = 0.50f;													// maximum specular intensity
	vec3 viewDirection = normalize(camPos - crntPos);								// view direction towards current pixel
	vec3 reflectionDirection = reflect(-lightDirection, normal);					// reflect negative so we can get the vector facing the plane 
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);	// larger direction between the view and reflect direction, make the specular light weaker  
	float specular = specAmount * specularLight;									// find the specular to add to the ambient and diffuse

	return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient) + texture(specular0, texCoord).r * specular * intensity) * lightColor;
}

vec4 directionalLight()
{
	float ambient = 0.2f;
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(0.0f, 0.0f, -1.0f));

	// diffuse lighting
	float diffuse = max(dot(normal, lightDirection), 0.0f); // take max so no negatives happen

	float specularLight = 0.50f;													// maximum specular intensity
	vec3 viewDirection = normalize(camPos - crntPos);								// view direction towards current pixel
	vec3 reflectionDirection = reflect(-lightDirection, normal);					// reflect negative so we can get the vector facing the plane 
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);	// larger direction between the view and reflect direction, make the specular light weaker  
	float specular = specAmount * specularLight;									// find the specular to add to the ambient and diffuse

	return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 spotLight()
{
	float outerCone = 0.9f;
	float innerCone = 0.95f;

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

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient) + texture(specular0, texCoord).r * specular * intensity) * lightColor;
}

void main()
{
	FragColor = pointLight();
}