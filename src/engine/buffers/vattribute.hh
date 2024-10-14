#ifndef KINGOM_ENGINE_VATTR_HH
#define KINGOM_ENGINE_VATTR_HH

#include <GL/glew.h>

namespace kingom::engine {

enum class VertexAttributeType {
  FLOAT = GL_FLOAT,
  INT = GL_INT,
  UNSIGNED_INT = GL_UNSIGNED_INT,
  BYTE = GL_BYTE,
  UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
  SHORT = GL_SHORT,
  UNSIGNED_SHORT = GL_UNSIGNED_SHORT
};
struct VertexAttribute {
  GLint size;
  VertexAttributeType type;
  GLboolean normalized;
  GLint offset;

  VertexAttribute(int size, VertexAttributeType type, bool normalized = false,
                  int offset = 0)
      : size(size),
        type(type),
        normalized(normalized),
        offset(0) {}

  inline void set_offset(int offset) { this->offset = offset; };
  inline void set_size(int size) { this->size = size; };
  inline void set_type(VertexAttributeType type) { this->type = type; }
  inline void set_normalized(bool normalized) { this->normalized = normalized; }
};
}  // namespace kingom::engine

#endif