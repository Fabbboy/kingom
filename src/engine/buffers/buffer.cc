#include "engine/buffers/buffer.hh"

namespace kingom::engine {
BaseBuffer::BaseBuffer(BufferType type) : type(type) { glGenBuffers(1, &id); }
BaseBuffer::~BaseBuffer() { glDeleteBuffers(1, &id); }
void BaseBuffer::bind() const { glBindBuffer(static_cast<GLenum>(type), id); }
void BaseBuffer::unbind() const { glBindBuffer(static_cast<GLenum>(type), 0); }
}  // namespace kingom::engine
