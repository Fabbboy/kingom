#version 330 core

// Input from the vertex attributes
layout(location = 0) in vec3 aPos;        // Position
layout(location = 1) in vec3 aNormal;     // Normal
layout(location = 2) in vec2 aTexCoords;  // Texture Coordinates

// Uniforms
uniform mat4 model;       // Model matrix
uniform mat4 view;        // View matrix
uniform mat4 projection;  // Projection matrix

// Output to fragment shader
out vec3 fragPos;     // Fragment position in world space
out vec3 fragNormal;  // Normal in world space
out vec2 texCoords;   // Texture coordinates

void main() {
  // Compute the fragment position in world space
  fragPos = vec3(model * vec4(aPos, 1.0));

  // Transform the normal to world space
  fragNormal = mat3(transpose(inverse(model))) * aNormal;

  // Pass texture coordinates to fragment shader
  texCoords = aTexCoords;

  // Final position in clip space
  gl_Position = projection * view * vec4(fragPos, 1.0);
}
