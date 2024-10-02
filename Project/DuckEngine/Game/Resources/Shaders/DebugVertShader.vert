    #version 450 core
    layout(location = 0) in vec3 position;
    uniform mat3 uModelToNDC;
    void main() {
        vec3 worldPosition = uModelToNDC * vec3(position.xy, 1.0);
        gl_Position = vec4(worldPosition.xy, 0.0, 1.0);
    }