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
  class Mesh;

  class PipelineItem {
    virtual void step(BasePipeline*) = 0;
  };
  
  class BasePipeline {
    protected: 
      Ref<BaseMaterial> material;        
      Ref<Layout> layout;
      Ref<Mesh> mesh;

    public:
    virtual ~BasePipeline() = default;

    virtual void push(Ref<PipelineItem>) = 0;
    
    virtual void push(Ref<BaseMaterial> mat) {
      this->material = mat;
    };

    virtual void push(Ref<Layout> lay)  {
      this->layout = lay;
    };
    
    virtual void push(Ref<Mesh> mesh)  {
      this->mesh = mesh;
    };
    
    virtual Ref<Layout> getLayout() const { 
      return layout;
    }

    virtual Ref<BaseMaterial> getMaterial() const {
      return material;
    }

    virtual Ref<Mesh> getMesh() const {
      return mesh;
    }

    virtual void render() = 0;
  };
}

#endif 
