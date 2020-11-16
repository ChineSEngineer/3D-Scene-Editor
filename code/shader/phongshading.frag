#version 150 core

in vec3 fragNormal;
in vec3 fragPosition;

out vec4 outColor;

uniform vec3 color;
// uniform vec3 eyePosition;
// uniform vec3 lightPosition; 

void main() {
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 eyePosition = vec3(0.0, 0.0, 10); 
    vec3 lightPosition = vec3(1.0, 1.0, 1.0);

    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 lightDir = normalize(lightPosition - fragPosition);
    float diff = max(dot(fragNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * 2;

    // specular
    float specularStrength = 0.7;
    vec3 viewDir = normalize(eyePosition - fragPosition);
    vec3 reflectDir = reflect(-lightDir, fragNormal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * color;
    outColor = vec4(result, 1.0);
};
