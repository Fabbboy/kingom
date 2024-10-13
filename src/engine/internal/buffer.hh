#ifndef KINGOM_ENGINE_BUFFER_HH
#define KINGOM_ENGINE_BUFFER_HH

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace kingom::engine::internal {
enum class BufferType {
  Vertex = GL_ARRAY_BUFFER,
  Index = GL_ELEMENT_ARRAY_BUFFER,
  Uniform = GL_UNIFORM_BUFFER,
  Storage = GL_SHADER_STORAGE_BUFFER,
  Generic = GL_COPY_READ_BUFFER
};

enum class DrawMode {
  Static = GL_STATIC_DRAW,
  Dynamic = GL_DYNAMIC_DRAW,
  Stream = GL_STREAM_DRAW
};

class Buffer {
 private:
  GLuint id;
  BufferType type;

 public:
  Buffer(BufferType type);
  ~Buffer();

  void bind();
  void unbind();

  inline GLuint get_id() { return id; };
  inline BufferType get_type() { return type; };

  template <typename T>
  void set_data(size_t size, T* data, DrawMode mode = DrawMode::Static) {
    bind();
    glBufferData(static_cast<GLenum>(type), size, data,
                 static_cast<GLenum>(mode));
  };
};
}  // namespace kingom::engine::internal

#endif