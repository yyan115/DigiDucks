#version 450 core

layout(location = 0) in vec3 vPosition;  // Vertex Position
layout(location = 1) in vec3 vColor;     // Vertex color
layout(location = 2) in vec2 vTexCoord;  // Texture coordinates (if any)

layout(location = 0) out vec3 fColor;    // Pass the color to the fragment shader
layout(location = 1) out vec2 fTexCoord; // Pass the texture coordinates (if any)

void main() {
    fColor = vColor;
    fTexCoord = vTexCoord;
    gl_Position = vec4(vPosition, 1.0);
}
