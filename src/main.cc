#include <fstream>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "engine/engine.hh"

using namespace kingom::engine;

class CustomMaterialData : public BaseMaterialData {
 public:
  CustomMaterialData(glm::mat4 view, glm::mat4 projection)
      : view(view), projection(projection) {}

  glm::mat4 view;
  glm::mat4 projection;
};

class CustomMaterial : public BaseMaterial {
 public:
  CustomMaterial(Raw<Shader> shader, Raw<Texture> tex)
      : shader(shader), tex(tex) {}

  void bind(Ref<BaseMaterialData> data) override {
    shader->use();
    tex->bind(GL_TEXTURE0);
    shader->set_int("texture1", 0);

    auto custom_data = std::static_pointer_cast<CustomMaterialData>(data);
    shader->set_mat4("view", custom_data->view);
    shader->set_mat4("projection", custom_data->projection);
    shader->set_mat4("model", glm::mat4(1.0f));
  }

  void unbind() override {}

 private:
  Raw<Shader> shader;
  Raw<Texture> tex;
};

int main() {
  auto result = kingom::engine::WindowDesc()
                    .set_width(800)
                    .set_height(600)

                    .build();

  if (result.is_err()) {
    std::cerr << "Failed to create window: " << result.unwrap_err().what()
              << std::endl;
    return -1;
  }

  auto window = result.unwrap();
  window->activate();
  auto renderer = window->get_renderer();

  std::fstream vertex_file("../assets/codeScreen.vert");
  std::fstream fragment_file("../assets/codeScreen.frag");

  auto ret_shader = Shader::create(vertex_file, fragment_file);
  if (ret_shader.is_err()) {
    std::cerr << "Failed to create shader: " << ret_shader.unwrap_err().what()
              << std::endl;
    return -1;
  }
  auto shader = ret_shader.unwrap();

  auto tex_ret = kingom::engine::Texture::create("../assets/codeScreen.png");
  if (tex_ret.is_err()) {
    std::cerr << "Failed to create texture: " << tex_ret.unwrap_err().what()
              << std::endl;
    return -1;
  }
  auto tex = tex_ret.unwrap();

  std::vector<float> vertices = {
      // positions       // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // top right
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f, 0.0f, 1.0f   // top left
  };

  kingom::engine::VertexBuffer vertex_buffer;
  vertex_buffer.set_data(vertices.data(), vertices.size());

  std::vector<unsigned int> indices = {
      0, 1, 3,  // first triangle
      1, 2, 3   // second triangle
  };

  kingom::engine::IndexBuffer index_buffer;
  index_buffer.set_data(indices.data(), indices.size());

  kingom::engine::Layout layout;
  layout.add_attribute(kingom::engine::VertexAttribute(
      3, kingom::engine::VertexAttributeType::FLOAT));
  layout.add_attribute(kingom::engine::VertexAttribute(
      2, kingom::engine::VertexAttributeType::FLOAT));

  layout.attach(make_box(vertex_buffer), make_box(index_buffer));

  auto res = layout.build();
  if (res.is_err()) {
    std::cerr << "Failed to build layout: " << res.unwrap_err().what()
              << std::endl;
    return -1;
  }

  Box<BaseMaterial> material =
      make_box<CustomMaterial>(shader.get(), tex.get());
  Mesh mesh(make_box(layout), std::move(material));

  auto camera =
      make_box<base::PerspectiveCamera>(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
  camera->set_position(glm::vec3(0.0f, 0.0f, 3.0f));

  while (!window->should_close()) {
    window->poll_events();
    renderer->clear();

    glm::mat4 view = camera->get_view_matrix();
    glm::mat4 projection = camera->get_projection_matrix();

    std::cout << glm::to_string(view) << std::endl;

    mesh.draw(make_box<CustomMaterialData>(view, projection));

    renderer->swap();
  }

  return 0;
}
