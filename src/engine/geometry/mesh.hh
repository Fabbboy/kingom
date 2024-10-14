#ifndef KINGOM_ENGINE_MESH_HH
#define KINGOM_ENGINE_MESH_HH

#include "engine/buffers/layout.hh"
#include "engine/geometry/material.hh"

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

class Mesh {
 private:
  Layout layout;
  std::unique_ptr<BaseMaterial> material;
  DrawMethod draw_method;

 public:
  Mesh(Layout& layout, std::unique_ptr<BaseMaterial> material,
       DrawMethod draw_method = DrawMethod::Triangle)
      : layout(layout),
        material(std::move(material)),
        draw_method(draw_method) {}

  void draw();
};
}  // namespace kingom::engine

#endif