#include "material.hh"

namespace kingom::engine::base {
AlbedoMaterial::AlbedoMaterial(Ref<Texture> albedo_texture,
                               glm::vec4 albedo_color, float metallic,
                               float roughness, Ref<Texture> ao_texture,
                               Ref<Texture> normal_texture)
    : albedo_texture(albedo_texture),
      albedo_color(albedo_color),
      metallic(metallic),
      roughness(roughness),
      ao_teture(ao_texture),
      normal_texture(normal_texture) {}

void AlbedoMaterial::apply(Ref<Shader> shader) {
  shader->use();
  shader->set_vec4("material.albedo_color", albedo_color);
  shader->set_float("material.metallic", metallic);
  shader->set_float("material.roughness", roughness);

  if (albedo_texture) {
    albedo_texture->bind(GL_TEXTURE0);
    shader->set_int("material.albedo_texture", 0);
  }

  if (ao_teture) {
    ao_teture->bind(GL_TEXTURE1);
    shader->set_int("material.ao_texture", 1);
  }

  if (normal_texture) {
    normal_texture->bind(GL_TEXTURE2);
    shader->set_int("material.normal_texture", 2);
  }

  shader->set_int("use.albedo_texture", albedo_texture != nullptr);
  shader->set_int("use.ao_texture", ao_teture != nullptr);
  shader->set_int("use.normal_texture", normal_texture != nullptr);
};
}  // namespace kingom::engine::base