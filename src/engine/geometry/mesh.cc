#include "engine/geometry/mesh.hh"

namespace kingom::engine {
void Mesh::draw(Ref<BaseMaterialData> data) {
  layout->bind();
  material->bind(data);
  glDrawElements(static_cast<GLenum>(draw_method), layout->get_count(),
                 GL_UNSIGNED_INT, nullptr);
  layout->unbind();
  material->unbind();
}
}  // namespace kingom::engine