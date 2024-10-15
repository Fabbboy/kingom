#ifndef KINGOM_ENGINE_LAYOUT_HH
#define KINGOM_ENGINE_LAYOUT_HH

#include <vector>

#include "buffer.hh"
#include "engine/buffer/vattribute.hh"
#include "engine/memory.hh"
#include "util/result.hh"

namespace kingom::engine {
class Layout {
 private:
  GLuint id = GL_NONE;
  GLuint offset;
  GLuint stride;

  std::vector<VertexAttribute> attributes;

  Ref<VertexBuffer> vbo;
  std::optional<Ref<IndexBuffer>> ebo;

 private:
  void calculate_offset_and_stride();

 public:
  Layout();
  ~Layout();

  void add_attribute(VertexAttribute attribute);
  void attach(Box<VertexBuffer> vbo, Box<IndexBuffer> ebo);
  void attach(Box<VertexBuffer> vbo);
  void attach(Box<IndexBuffer> ebo);

  unsigned int get_count() const {
    return ebo.has_value() ? ebo.value()->get_data()->size() : 0;
  }

  util::Result<void, std::exception> build();
  inline void bind() const { glBindVertexArray(id); }
  inline void unbind() const { glBindVertexArray(0); }

  inline GLuint get_id() const { return id; }
  inline GLuint get_offset() const { return offset; }
  inline GLuint get_stride() const { return stride; }
};
}  // namespace kingom::engine

#endif