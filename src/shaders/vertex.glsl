#version 330 core

layout (location=0) in vec3 vertexPos;
layout (location=1) in vec2 vertexTexCoord;
layout (location=2) in vec3 vertexNormal;
// layout (location=2) in int colorIndex;

// out vec3 vertexColor;
out vec3 fragPos;
out vec2 texCoord;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // fragPos: only model matrix for world space coords
    fragPos = vec3(model * vec4(vertexPos, 1.0));
    gl_Position = projection * view * model * vec4(vertexPos, 1.0);
    texCoord = vec2(vertexTexCoord.x, vertexTexCoord.y);
    // normal = (model * vec4(vertexNormal, 0.0)).xyz;
    normal = mat3(transpose(inverse(model))) * vertexNormal;
    // texCoord = (vertexPos.xy + vec2(1.0)) * vec2(0.5, 0.5); // swizzling
}