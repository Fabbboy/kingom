#version 330 core

// Inputs from the vertex shader
in vec3 fragPos;     // Fragment position in world space
in vec3 fragNormal;  // Normal in world space
in vec2 texCoords;   // Texture coordinates

// Uniforms for material properties
struct Material {
  vec4 albedo_color;         // Base albedo color
  sampler2D albedo_texture;  // Albedo texture (required)
  sampler2D ao_texture;      // Ambient occlusion texture (required)
  sampler2D normal_texture;  // Normal map (required)
  float metallic;            // Metallic property
  float roughness;           // Roughness property
};

// Uniform to determine which textures to use (can be ignored if everything is mandatory)
struct UseTextures {
  int albedo_texture;
  int ao_texture;
  int normal_texture;
};

uniform Material material;

// Lighting and view positions
uniform vec3 lightPos;  // Light position in world space
uniform vec3 viewPos;   // Camera/viewer position in world space

// Outputs the final color
out vec4 FragColor;

// Function to calculate lighting (simplified for now)
vec3 calculateLighting(vec3 normal, vec3 viewDir, vec3 lightDir) {
  vec3 ambient = vec3(0.1);  // Simplified ambient lighting

  // Diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = diff * vec3(1.0);  // Simplified diffuse light

  // Specular shading (simple blinn-phong for now)
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
  vec3 specular = spec * vec3(1.0);  // Simplified specular light

  return ambient + diffuse + specular;  // Basic combined lighting
}

void main() {
  // Fetch normal map (required)
  vec3 normal = normalize(fragNormal);
  vec3 tangentNormal =
      texture(material.normal_texture, texCoords).xyz * 2.0 - 1.0;
  normal = normalize(tangentNormal);  // Normalize the normal from texture

  // View and light direction vectors
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 lightDir = normalize(lightPos - fragPos);

  // Calculate lighting (diffuse, specular)
  vec3 lighting = calculateLighting(normal, viewDir, lightDir);

  // Fetch albedo color (required)
  vec3 albedo = texture(material.albedo_texture, texCoords).rgb;

  // Apply Ambient Occlusion (required)
  float ao = texture(material.ao_texture, texCoords).r;

  // Final color = lighting * albedo * ambient occlusion
  vec3 finalColor = lighting * albedo * ao;

  FragColor = vec4(finalColor, 1.0);  // Output final color with full alpha
}
