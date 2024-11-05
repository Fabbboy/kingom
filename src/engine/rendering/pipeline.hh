#ifndef KINGOM_ENGINE_PIPELINE_HH 
#define KINGOM_ENGINE_PIPELINE_HH

/*
pipeline pip;

pip.push(material)
pip.push(shader)
pip.push(mesh)

while render {
  pip.adjust(ShaderVariable(VariableType::int, "mvp", mvp))
  pip.render()
}
*/

#include "engine/memory.hh"

namespace kingom::engine{
  class BasePipeline; 
  class BaseMaterial; 
  class Layout;

  class PipelineItem {
    virtual void step(BasePipeline*) = 0;
  };
  
  class BasePipeline {
    public:
    virtual ~BasePipeline() = default;

    virtual void push(Ref<PipelineItem>) = 0;
    virtual void render() = 0;
  };

  namespace base {
    class DefaultPipeline : public BasePipeline {
      public:
        Ref<BaseMaterial> material;        
        Ref<Layout> layout;

        void push(Ref<PipelineItem> item) override; 
    };
  }
}

#endif 
