#ifndef KINGDOM_ENGINE_BUFFER_HH
#define KINGDOM_ENGINE_BUFFER_HH

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstddef>

namespace kingom::engine {

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

class BaseBuffer {
 protected:
  GLuint id;
  BufferType type;

 public:
  BaseBuffer(BufferType type);
  virtual ~BaseBuffer();

  void bind() const;
  void unbind() const;

  inline GLuint get_id() const { return id; }
  inline BufferType get_type() const { return type; }

  template <typename T>
  void set_data(const T* data, size_t count, DrawMode mode = DrawMode::Static) {
    bind();
    glBufferData(static_cast<GLenum>(type), count * sizeof(T), data,
                 static_cast<GLenum>(mode));
  }
};

// Specialized buffers
class VertexBuffer : public BaseBuffer {
 public:
  VertexBuffer() : BaseBuffer(BufferType::Vertex) {}
};

class IndexBuffer : public BaseBuffer {
 public:
  IndexBuffer() : BaseBuffer(BufferType::Index) {}
};

class UniformBuffer : public BaseBuffer {
 public:
  UniformBuffer() : BaseBuffer(BufferType::Uniform) {}
};

class StorageBuffer : public BaseBuffer {
 public:
  StorageBuffer() : BaseBuffer(BufferType::Storage) {}
};

}  // namespace kingom::engine

#endif  // KINGDOM_ENGINE_BUFFER_HH
