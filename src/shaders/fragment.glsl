#version 330 core

// in vec3 vertexColor;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

// texture data + sampler
uniform sampler2D material; // texture unit 0
uniform vec3 viewPos;

// const vec3 sunDirection = normalize(vec3(-1.0, 1.0, 1.0));
const vec3 lightPos = vec3(0.0, 0.0, 0.0);
const vec3 lightColor = vec3(1.0, 1.0, 1.0);
const float specularStrength = 0.5;

void main() {
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular 
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 baseColor = texture(material, texCoord).rgb;
    
    // vec3 baseColor = vec3(0.0,0.0,0.0);


    // fragColor = vec4(baseColor * (specular + diffuse + ambient), 1.0);
    fragColor = vec4(baseColor, 1.0);
    // fragColor = vec4(baseColor * lightStrength, 1.0);
}