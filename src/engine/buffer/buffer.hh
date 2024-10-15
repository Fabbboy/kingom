#ifndef KINGDOM_ENGINE_BUFFER_HH
#define KINGDOM_ENGINE_BUFFER_HH

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstddef>
#include <optional>
#include <vector>

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

template <BufferType T, typename D>
class BaseBuffer {
 protected:
  GLuint id = GL_NONE;
  BufferType type;
  std::optional<std::vector<D>> data;
  DrawMode mode;
  bool is_data_uploaded = false;

 public:
  BaseBuffer(DrawMode mode = DrawMode::Static) : type(T), mode(mode) {}

  virtual ~BaseBuffer() {
    if (id != GL_NONE) glDeleteBuffers(1, &id);
  }

  inline BufferType get_type() const { return type; }
  inline GLuint get_id() const { return id; }
  inline std::optional<std::vector<D>> get_data() const { return data; }

  void bind() {
    if (id == GL_NONE) {
      glGenBuffers(1, &id);
    }
    glBindBuffer(static_cast<GLenum>(type), id);
  }

  void unbind() const { glBindBuffer(static_cast<GLenum>(type), 0); }

  void set_data(const D* input_data, std::size_t size) {
    data = std::vector<D>(input_data, input_data + size);
    is_data_uploaded = false;
  }

  void set_data(const std::vector<D>& input_data) {
    data = input_data;
    is_data_uploaded = false;
  }

  void upload_data() {
    if (!is_data_uploaded && data.has_value()) {
      bind();
      glBufferData(static_cast<GLenum>(type), data->size() * sizeof(D),
                   data->data(), static_cast<GLenum>(mode));
      is_data_uploaded = true;
    }
  }
};

class VertexBuffer : public BaseBuffer<BufferType::Vertex, float> {
 public:
  using BaseBuffer<BufferType::Vertex, float>::BaseBuffer;
};

class IndexBuffer : public BaseBuffer<BufferType::Index, unsigned int> {
 public:
  using BaseBuffer<BufferType::Index, unsigned int>::BaseBuffer;
};
}  // namespace kingom::engine

#endif  // KINGDOM_ENGINE_BUFFER_HH
