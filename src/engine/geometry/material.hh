#ifndef KINGOM_ENGINE_MATERIAL_HH
#define KINGOM_ENGINE_MATERIAL_HH

#include "engine/geometry/texture.hh"
#include "engine/memory.hh"
#include "engine/rendering/shader.hh"
namespace kingom::engine {
class BaseMaterial {
 public:
  virtual ~BaseMaterial() = default;
  virtual void apply(Ref<Shader> shader) = 0;
};

namespace base {
class AlbedoMaterial : public BaseMaterial {
 private:
  Ref<Texture> albedo_texture;
  glm::vec4 albedo_color;

  float metallic;
  float roughness;

  Ref<Texture> ao_teture = nullptr;
  Ref<Texture> normal_texture = nullptr;

 public:
  AlbedoMaterial(Ref<Texture> albedo_texture,
                 glm::vec4 albedo_color = glm::vec4(1.0f),
                 float metallic = 0.0f, float roughness = 0.0f,
                 Ref<Texture> ao_texture = nullptr,
                 Ref<Texture> normal_texture = nullptr);

  inline void set_albedo_texture(Ref<Texture> new_texture) {
    albedo_texture = new_texture;
  }
  inline void set_albedo_color(glm::vec4 new_color) {
    albedo_color = new_color;
  }
  inline void set_metallic(float new_metallic) { metallic = new_metallic; }
  inline void set_roughness(float new_roughness) { roughness = new_roughness; }
  inline void set_ao_texture(Ref<Texture> new_texture) {
    ao_teture = new_texture;
  }
  inline void set_normal_texture(Ref<Texture> new_texture) {
    normal_texture = new_texture;
  }

  inline Ref<Texture> get_albedo_texture() const { return albedo_texture; }
  inline glm::vec4 get_albedo_color() const { return albedo_color; }
  inline float get_metallic() const { return metallic; }
  inline float get_roughness() const { return roughness; }
  inline Ref<Texture> get_ao_texture() const { return ao_teture; }
  inline Ref<Texture> get_normal_texture() const { return normal_texture; }

  void apply(Ref<Shader> shader) override;
};
}  // namespace base
}  // namespace kingom::engine
#endif