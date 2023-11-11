#version 330 core

// in vec3 vertexColor;
in vec2 texCoord;
in vec3 normal;

out vec4 fragColor;

// texture data + sampler
uniform sampler2D material; // texture unit 0

const vec3 sunDirection = normalize(vec3(-1.0, 1.0, 1.0));

void main() {
    float lightStrength = max(0.2, dot(normal, sunDirection));
    vec3 baseColor = texture(material, texCoord).rgb;
    // vec3 baseColor = vec3(1.0, 1.0, 1.0);
    fragColor = vec4(baseColor, 1.0);
    // fragColor = vec4(baseColor * lightStrength, 1.0);
    // float alpha = texture(mask, texCoord).r;
    // fragColor = vec4(baseColor, alpha);

    // vec3 newColor = mix(baseColor, vertexColor, mixValue);

    // vec3 newColor = baseColor * t;
    // float alpha = texture(mask, texCoord).r;
    
    // fragColor = vec4(baseColor, 1);
}