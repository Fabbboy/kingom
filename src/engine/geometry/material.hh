#ifndef KINGOM_ENGINE_MATERIAL_HH
#define KINGOM_ENGINE_MATERIAL_HH

#include "engine/memory.hh"
#include "engine/rendering/shader.hh"
#include "engine/rendering/pipeline.hh"

namespace kingom::engine {
class BaseMaterial : public PipelineItem {
 public:
  virtual ~BaseMaterial() = default;
  virtual void apply(Ref<Shader> shader) = 0;
  virtual void step(BasePipeline* pipeline) override = 0;
};
}  // namespace kingom::engine
#endif
