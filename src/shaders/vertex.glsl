#version 330 core

layout (location=0) in vec2 vertexPos;
layout (location=1) in int colorIndex;
// layout (location=1) in vec3 vertexColor;

out vec3 fragmentColor;

const vec3 colors[4] = vec3[](
    vec3(1.0, 0.0, 0.0), // red
    vec3(0.0, 1.0, 0.0), // green
    vec3(0.0, 0.0, 1.0), // blue
    vec3(0.5, 0.5, 0.0) // orange
);

void main() {
    gl_Position = vec4(vertexPos, 0.0, 1.0);
    // fragmentColor = vertexColor;
    fragmentColor = colors[colorIndex];
}