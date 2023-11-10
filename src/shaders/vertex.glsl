#version 330 core

layout (location=0) in vec3 vertexPos;
layout (location=1) in vec2 vertexTexCoord;
layout (location=3) in vec3 vertexNormal;
// layout (location=2) in int colorIndex;

// out vec3 vertexColor;
out vec2 texCoord;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


const vec3 colors[4] = vec3[](
    vec3(1.0, 0.0, 0.0), // red
    vec3(0.0, 1.0, 0.0), // green
    vec3(0.0, 0.0, 1.0), // blue
    vec3(0.5, 0.5, 0.0) // orange
);

void main() {
    gl_Position = projection * view * model * vec4(vertexPos, 1.0);
    texCoord = vec2(vertexTexCoord.x, 1.0 - vertexTexCoord.y);
    normal = (model * vec4(vertexNormal, 0.0)).xyz;
    // texCoord = (vertexPos.xy + vec2(1.0)) * vec2(0.5, 0.5); // swizzling
}