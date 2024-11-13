#include "engine/geometry/mesh.hh"
#include "engine/rendering/shader.hh"

namespace kingom::engine {
void Mesh::draw() {
  shader->use();
  layout->bind();
  ShaderPipeline& pipeline = shader->get_pipeline();
  pipeline.apply(shader);
  material->apply(shader);
  glDrawElements(static_cast<GLenum>(draw_method), layout->get_count(),
                 GL_UNSIGNED_INT, nullptr);
  layout->unbind();
  shader->unuse();
}
}  // namespace kingom::engine