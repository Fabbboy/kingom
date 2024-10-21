#include "material.hh"

namespace kingom::engine::base {

AlbedoMaterial::AlbedoMaterial(Ref<Texture> albedo_texture,
                               Ref<Texture> ao_texture,
                               Ref<Texture> normal_texture,
                               glm::vec4 albedo_color, float metallic,
                               float roughness)
    : albedo_texture(albedo_texture),
      albedo_color(albedo_color),
      metallic(metallic),
      roughness(roughness),
      ao_teture(ao_texture),
      normal_texture(normal_texture) {}

void AlbedoMaterial::apply(Ref<Shader> shader) {
  shader->use();

  // Set the albedo color and other properties
  shader->set_vec4("material.albedo_color", albedo_color);
  shader->set_float("material.metallic", metallic);
  shader->set_float("material.roughness", roughness);

  // Bind the albedo texture
  albedo_texture->bind(GL_TEXTURE0);
  shader->set_int("material.albedo_texture", 0);  // Texture unit 0
  shader->set_int("use.albedo_texture", 1);       // Ensure texture is used

  // Bind the AO texture (even if default)
  ao_teture->bind(GL_TEXTURE1);
  shader->set_int("material.ao_texture", 1);  // Texture unit 1
  shader->set_int("use.ao_texture", 1);       // Ensure texture is used

  // Bind the normal texture (even if default)
  normal_texture->bind(GL_TEXTURE2);
  shader->set_int("material.normal_texture", 2);  // Texture unit 2
  shader->set_int("use.normal_texture", 1);       // Ensure texture is used
}

}  // namespace kingom::engine::base
