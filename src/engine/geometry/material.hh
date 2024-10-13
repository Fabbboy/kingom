#ifndef KINGOM_ENGINE_MATERIAL_HH
#define KINGOM_ENGINE_MATERIAL_HH

namespace kingom::engine {
class BaseMaterial {
 public:
  virtual ~BaseMaterial() = default;
  virtual void bind() = 0;
  virtual void unbind() = 0;
};
}  // namespace kingom::engine
#endif