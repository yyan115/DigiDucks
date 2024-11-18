#version 450 core

layout(location = 0) in vec3 inPosition;

// Instance attributes
layout(location = 1) in vec2 instanceCenter;
layout(location = 2) in vec2 instanceSize;
layout(location = 3) in float instanceRotation;
layout(location = 4) in vec4 instanceColor;

uniform mat3 uModelToNDC;

out vec4 vColor;

void main() {
    // Rotation matrix
    float radians = radians(instanceRotation);
    mat2 rotationMatrix = mat2(
        cos(radians), sin(radians),
        -sin(radians), cos(radians)
    );

    // Scale and rotate vertex
    vec2 scaledPosition = inPosition.xy * instanceSize;
    vec2 rotatedPosition = rotationMatrix * scaledPosition;

    // Translate to center position
    vec2 finalPosition = rotatedPosition + instanceCenter;

    vec3 worldPosition = uModelToNDC * vec3(finalPosition, 1.0);
    gl_Position = vec4(worldPosition.xy, 0.0, 1.0);
    vColor = instanceColor;
}
