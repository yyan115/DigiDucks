#version 450 core

layout(location = 0) in vec3 fColor;     // Incoming color from the vertex shader
layout(location = 1) in vec2 fTexCoord;  // Incoming texture coordinates

layout(location = 0) out vec4 fFragColor; // Outgoing fragment's color

uniform sampler2D uTex2d;      // The texture (if used)
uniform int uUseTexture;       // Whether to use the texture (1 = true, 0 = false)

void main() {
    vec4 baseColor = vec4(fColor, 1.0);  // Default to vertex color

    // If texture is used, blend the texture color with the base color (vertex color)
    if (uUseTexture == 1) {
        vec4 textureColor = texture(uTex2d, fTexCoord);
        baseColor = mix(baseColor, textureColor, textureColor.a);  // Blend based on texture alpha
    }

    // Final fragment color
    fFragColor = baseColor;
}



//#version 450 core
//
////layout(location = 0) in vec3 fColor;     // Incoming color from the vertex shader
//layout(location = 1) in vec2 fTexCoord;  // Incoming texture coordinates
//
//layout(location = 0) out vec4 fFragColor; // Outgoing fragment's color
//
//uniform sampler2D uTex2d;      // The texture (if used)
//
//uniform int uUseTexture;       // Whether to use the texture (1 = true, 0 = false)
//
//uniform int uBlendColors;      // Whether to blend colors (1 = true, 0 = false)
//
//uniform vec4 uBlendColor;      // The color to blend with the texture or vertex color
//
//void main() {
//	vec4 baseColor = vec4(1.0, 0.0, 1.0, 1.0);
//
//	// If using texture, sample the texture
//	if (uUseTexture == 1) {
//		baseColor = texture(uTex2d, fTexCoord);
//	} //else {
//		// Otherwise, use the vertex color
//	   // baseColor = vec4(fColor, 1.0);
//	//}
//
//	// Apply blending if enabled
//	if (uBlendColors == 1) {
//		fFragColor = baseColor * uBlendColor;
//	}
//	else {
//		fFragColor = baseColor;
//	}
//}