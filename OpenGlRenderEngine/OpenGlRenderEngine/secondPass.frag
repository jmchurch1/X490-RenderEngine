#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D screenTexture;

mat3 sx = mat3( 
    1.0, 2.0, 1.0, 
    0.0, 0.0, 0.0, 
   -1.0, -2.0, -1.0 
);
mat3 sy = mat3( 
    1.0, 0.0, -1.0, 
    2.0, 0.0, -2.0, 
    1.0, 0.0, -1.0 
);

const float RADIUS = 0.75;

const float SOFTNESS = 0.6;

const float blurSize = 1.0/1000.0;


vec4 detect()
{
    vec3 diffuse = texture(screenTexture, texCoord.st).rgb;
    mat3 I;
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            vec3 sample  = texelFetch(screenTexture, ivec2(gl_FragCoord) + ivec2(i-1,j-1), 0 ).rgb;
            I[i][j] = length(sample); 
        }
    }

    float gx = dot(sx[0], I[0]) + dot(sx[1], I[1]) + dot(sx[2], I[2]); 
    float gy = dot(sy[0], I[0]) + dot(sy[1], I[1]) + dot(sy[2], I[2]);

    float g = sqrt(pow(gx, 2.0)+pow(gy, 2.0));
    g = smoothstep(0.4f, 0.8f, g);
    vec3 edgeColor = vec3(0.0f, 0.0f, 0.0f);
    return vec4(mix(diffuse, edgeColor, g), 1.0f);
}

vec4 blur()
{
    vec4 texColor = vec4(0.0); // texture2D(u_texture, texCoord)
    texColor += texture2D(screenTexture, texCoord - 4.0*blurSize) * 0.05;
    texColor += texture2D(screenTexture, texCoord - 3.0*blurSize) * 0.09;
    texColor += texture2D(screenTexture, texCoord - 2.0*blurSize) * 0.12;
    texColor += texture2D(screenTexture, texCoord - blurSize) * 0.15;
    texColor += texture2D(screenTexture, texCoord) * 0.16;
    texColor += texture2D(screenTexture, texCoord + blurSize) * 0.15;
    texColor += texture2D(screenTexture, texCoord + 2.0*blurSize) * 0.12;
    texColor += texture2D(screenTexture, texCoord + 3.0*blurSize) * 0.09;
    texColor += texture2D(screenTexture, texCoord + 4.0*blurSize) * 0.05;

    float vignette = smoothstep(RADIUS, RADIUS-SOFTNESS, 0.2f);

    texColor.rgb = mix(texColor.rgb, texColor.rgb * vignette, 0.5);

    return vec4(texColor.rgb, 1.0f);
}

void main()
{
    // FragColor = texture(screenTexture, texCoord);
    FragColor = detect();
}