#ifndef KINGOM_ENGINE_MESH_HH
#define KINGOM_ENGINE_MESH_HH

#include "engine/buffer/layout.hh"
#include "engine/geometry/material.hh"
#include "engine/rendering/pipeline.hh"

namespace kingom::engine {
enum class DrawMethod {
  Triangle = GL_TRIANGLES,
  TriangleStrip = GL_TRIANGLE_STRIP,
  TriangleFan = GL_TRIANGLE_FAN,
  Line = GL_LINES,
  LineStrip = GL_LINE_STRIP,
  LineLoop = GL_LINE_LOOP,
  Point = GL_POINTS
};

class Mesh : public PipelineItem {
 private:
  Box<Layout> layout;
  Ref<BaseMaterial> material;
  Ref<Shader> shader;
  DrawMethod draw_method;

 public:
  Mesh(Box<Layout> layout, Ref<BaseMaterial> material, Ref<Shader> shader,
       DrawMethod draw_method = DrawMethod::Triangle)
      : layout(std::move(layout)),
        material(material),
        shader(shader),
        draw_method(draw_method) {}

  void draw();
  void step(BasePipeline* pipeline) override;
};
}  // namespace kingom::engine

#endif
