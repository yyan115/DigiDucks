#version 450 core

layout(location = 0) in vec2 vPosition;  // Vertex Position
layout(location = 1) in vec3 vColor;     // Vertex color
layout(location = 2) in vec2 vTexCoord;  // Texture coordinates (if any)

layout(location = 0) out vec3 fColor;    // Pass the color to the fragment shader
layout(location = 1) out vec2 fTexCoord; // Pass the texture coordinates (if any)

uniform mat3 uModelToNDC;

void main() {
    fColor = vColor;
    fTexCoord = vTexCoord;

    gl_Position = vec4(vec2(uModelToNDC * vec3(vPosition, 1.f)), 0.0, 1.0);
}
