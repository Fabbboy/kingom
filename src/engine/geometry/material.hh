#ifndef KINGOM_ENGINE_MATERIAL_HH
#define KINGOM_ENGINE_MATERIAL_HH

#include "engine/memory.hh"
#include "engine/rendering/shader.hh"
namespace kingom::engine {
class BaseMaterial {
 public:
  virtual ~BaseMaterial() = default;
  virtual void apply(Ref<Shader> shader) = 0;
};
}  // namespace kingom::engine
#endif