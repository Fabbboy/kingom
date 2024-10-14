#ifndef KINGOM_ENGINE_LAYOUT_HH
#define KINGOM_ENGINE_LAYOUT_HH

#include <memory>
#include <vector>

#include "buffer.hh"
#include "engine/buffers/vattribute.hh"
#include "util/result.hh"

namespace kingom::engine {
class Layout {
 private:
  GLuint id = GL_NONE;
  GLuint offset;
  GLuint stride;

  std::vector<VertexAttribute> attributes;

  std::shared_ptr<VertexBuffer> vbo;
  std::optional<std::shared_ptr<IndexBuffer>> ebo;

 private:
  void calculate_offset_and_stride();

 public:
  Layout();
  ~Layout();

  void add_attribute(VertexAttribute attribute);
  void add_buffers(std::shared_ptr<VertexBuffer> vbo,
                   std::shared_ptr<IndexBuffer> ebo);
  void add_buffer(std::shared_ptr<VertexBuffer> vbo);
  void add_buffer(std::shared_ptr<IndexBuffer> ebo);

  util::Result<void, std::exception> build();
  inline void bind() const { glBindVertexArray(id); }
  inline void unbind() const { glBindVertexArray(0); }

  inline GLuint get_id() const { return id; }
  inline GLuint get_offset() const { return offset; }
  inline GLuint get_stride() const { return stride; }
};
}  // namespace kingom::engine

#endif