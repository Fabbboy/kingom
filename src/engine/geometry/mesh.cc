#include "engine/geometry/mesh.hh"

namespace kingom::engine {
void Mesh::draw() {
  shader->use();
  layout->bind();
  material->apply(shader);
  glDrawElements(static_cast<GLenum>(draw_method), layout->get_count(),
                 GL_UNSIGNED_INT, nullptr);
  layout->unbind();
  shader->unuse();
}
void Mesh::step(BasePipeline* pipeline) {
  glDrawElements(static_cast<GLenum>(draw_method), pipeline->getLayout()->get_count(), GL_UNSIGNED_INT, nullptr);
}
}  // namespace kingom::engine
