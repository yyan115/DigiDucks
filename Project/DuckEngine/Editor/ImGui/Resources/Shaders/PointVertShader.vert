#version 450 core

layout(location = 0) in vec3 inPosition;

// Instance attributes
layout(location = 1) in vec2 instancePosition;
layout(location = 2) in float instanceSize;
layout(location = 3) in vec4 instanceColor;

uniform mat3 uModelToNDC;

out vec4 vColor;

void main() {
    vec2 scaledPosition = inPosition.xy * instanceSize;
    vec2 finalPosition = instancePosition + scaledPosition;

    vec3 worldPosition = uModelToNDC * vec3(finalPosition, 1.0);
    gl_Position = vec4(worldPosition.xy, 0.0, 1.0);
    vColor = instanceColor;
}
