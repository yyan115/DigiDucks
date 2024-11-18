#version 450 core

layout(location = 0) in vec3 inPosition;

// Instance attributes
layout(location = 1) in vec2 instancePosition;
layout(location = 2) in float instanceSize; // For points and circles
layout(location = 2) in float instanceThickness; // For lines
layout(location = 2) in vec2 instanceSizeVec; // For rectangles
layout(location = 3) in vec4 instanceColor;

// Uniforms
uniform mat3 uModelToNDC;

out vec4 vColor;

void main() {
    vec2 scaledPosition = inPosition.xy;

    // Adjust based on the shape being rendered
    // For points
    scaledPosition *= instanceSize;

    // For lines and rectangles, you might need additional calculations

    vec3 worldPosition = uModelToNDC * vec3(instancePosition + scaledPosition, 1.0);
    gl_Position = vec4(worldPosition.xy, 0.0, 1.0);
    vColor = instanceColor;
}
