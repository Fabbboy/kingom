#ifndef KINGOM_ENGINE_MATERIAL_HH
#define KINGOM_ENGINE_MATERIAL_HH

#include "engine/memory.hh"
namespace kingom::engine {
class BaseMaterialData {
 public:
  virtual ~BaseMaterialData() = default;
};

class BaseMaterial {
 public:
  virtual ~BaseMaterial() = default;
  virtual void bind(Ref<BaseMaterialData> data) = 0;
  virtual void unbind() = 0;
};
}  // namespace kingom::engine
#endif