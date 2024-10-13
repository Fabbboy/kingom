#include "engine/buffers/buffer.hh"

namespace kingom::engine::internal {
Buffer::Buffer(BufferType type) : type(type) { glGenBuffers(1, &id); }
Buffer::~Buffer() { glDeleteBuffers(1, &id); }

void Buffer::bind() { glBindBuffer(static_cast<GLenum>(type), id); }
void Buffer::unbind() { glBindBuffer(static_cast<GLenum>(type), 0); }
}  // namespace kingom::engine::internal