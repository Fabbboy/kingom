#ifndef KINGOM_ENGINE_LAYOUT_HH
#define KINGOM_ENGINE_LAYOUT_HH

#include <vector>

#include "buffer.hh"
#include "engine/buffers/vattribute.hh"

namespace kingom::engine {
class Layout {
 private:
  std::vector<VertexAttribute> attributes;

  BaseBuffer vbo;
  BaseBuffer ebo;

 public:
  Layout()
      : vbo(BaseBuffer(BufferType::Vertex)),
        ebo(BaseBuffer(BufferType::Index)) {}

  void add_attribute(VertexAttribute attribute);
};
}  // namespace kingom::engine

#endif