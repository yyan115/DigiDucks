#version 450 core

layout(location = 0) in vec3 inPosition;

// Instance attributes
layout(location = 1) in vec2 instanceStart;
layout(location = 2) in vec2 instanceEnd;
layout(location = 3) in float instanceThickness;
layout(location = 4) in vec4 instanceColor;

uniform mat3 uModelToNDC;

out vec4 vColor;

void main() {
    vec2 direction = instanceEnd - instanceStart;
    float length = length(direction);
    vec2 unitDirection = normalize(direction);

    // Build rotation matrix to align the line
    mat2 rotationMatrix = mat2(
        unitDirection.x, unitDirection.y,
        -unitDirection.y, unitDirection.x
    );

    // Scale the unit line to match the length
    vec2 scaledPosition = inPosition.xy * vec2(length, instanceThickness);

    // Rotate and translate
    vec2 finalPosition = rotationMatrix * scaledPosition + instanceStart;

    vec3 worldPosition = uModelToNDC * vec3(finalPosition, 1.0);
    gl_Position = vec4(worldPosition.xy, 0.0, 1.0);

    vColor = instanceColor;
}
