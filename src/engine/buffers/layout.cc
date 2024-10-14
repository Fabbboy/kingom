#include "engine/buffers/layout.hh"

#include <iostream>
#include <stdexcept>

namespace kingom::engine {
Layout::Layout() { glGenVertexArrays(1, &id); }
Layout::~Layout() {
  std::cout << "Deleting layout" << std::endl;
  if (id != GL_NONE) glDeleteVertexArrays(1, &id);
}

void Layout::add_attribute(VertexAttribute attribute) {
  attributes.push_back(attribute);
}

void Layout::add_buffers(std::unique_ptr<VertexBuffer> vbo,
                         std::unique_ptr<IndexBuffer> ebo) {
  this->vbo = std::move(vbo);
  this->ebo = std::move(ebo);
}

void Layout::add_buffer(std::unique_ptr<VertexBuffer> vbo) {
  this->vbo = std::move(vbo);
}
void Layout::add_buffer(std::unique_ptr<IndexBuffer> ebo) {
  this->ebo = std::move(ebo);
}

void Layout::calculate_offset_and_stride() {
  offset = 0;
  stride = 0;
  for (auto& attribute : attributes) {
    attribute.set_offset(offset);
    offset += attribute.size;
    stride += attribute.size;
  }
}

util::Result<void, std::exception> Layout::build() {
  if (vbo == nullptr) {
    return util::Result<void, std::exception>::Err(
        std::runtime_error("No vertex buffer object"));
  }

  this->bind();
  vbo->upload_data();
  calculate_offset_and_stride();

  for (unsigned int i = 0; i < attributes.size(); i++) {
    auto& attribute = attributes[i];
    glVertexAttribPointer(i, attribute.size,
                          static_cast<GLenum>(attribute.type),
                          attribute.normalized, stride * sizeof(float),
                          (void*)(attribute.offset * sizeof(float)));
    glEnableVertexAttribArray(i);
  }

  if (ebo.has_value()) {
    ebo.value()->upload_data();
  }

  unbind();
  vbo->unbind();
  if (ebo.has_value()) {
    ebo.value()->unbind();
  }

  return util::Result<void, std::exception>::Ok();
}

}  // namespace kingom::engine