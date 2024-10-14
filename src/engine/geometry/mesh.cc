#include "engine/geometry/mesh.hh"

namespace kingom::engine {
void Mesh::draw() {
  layout->bind();
  material->bind();
  glDrawElements(static_cast<GLenum>(draw_method), layout->get_count(),
                 GL_UNSIGNED_INT, nullptr);
  layout->unbind();
  material->unbind();
}
}  // namespace kingom::engine