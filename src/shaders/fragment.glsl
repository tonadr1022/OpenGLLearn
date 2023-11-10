#version 330 core

// in vec3 vertexColor;
in vec2 texCoord;

out vec4 fragColor;

// texture data + sampler
uniform sampler2D material; // texture unit 0
uniform sampler2D mask; // texture unit 1
uniform float mixValue;

void main() {
    vec3 baseColor = texture(material, texCoord).rgb;
    fragColor = vec4(baseColor, 1);
    // float alpha = texture(mask, texCoord).r;
    // fragColor = vec4(baseColor, alpha);

    // vec3 newColor = mix(baseColor, vertexColor, mixValue);

    // vec3 newColor = baseColor * t;
    // float alpha = texture(mask, texCoord).r;
    
    // fragColor = vec4(baseColor, 1);
}