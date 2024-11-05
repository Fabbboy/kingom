#include <fstream>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "engine/engine.hh"

using namespace kingom::engine;

struct BasicMaterial : public BaseMaterial  {
  Ref<Texture> tex;

  BasicMaterial(Ref<Texture> tex) : tex(tex) {}

  void apply(Ref<Shader> shader) override  {
    shader->use();
    tex->bind(GL_TEXTURE0);
    shader->set_int("texture1", 0);
  }
};

int main() {
  auto result =
    kingom::engine::WindowDesc().set_width(800).set_height(600).build();

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
  Ref<Texture> tex = tex_ret.unwrap();

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

  auto layout = make_box<Layout>();
  layout->add_attribute(kingom::engine::VertexAttribute(
        3, kingom::engine::VertexAttributeType::FLOAT));
  layout->add_attribute(kingom::engine::VertexAttribute(
        2, kingom::engine::VertexAttributeType::FLOAT));

  layout->attach(make_box(vertex_buffer), make_box(index_buffer));

  auto res = layout->build();
  if (res.is_err()) {
    std::cerr << "Failed to build layout: " << res.unwrap_err().what()
      << std::endl;
    return -1;
  }
  Ref<BasicMaterial> material = make_ref<BasicMaterial>(
      tex);

  auto mesh = make_box<Mesh>(std::move(layout), material, shader);

  auto camera =
    make_box<base::OrthographicCamera>(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

  while (!window->should_close()) {
    window->poll_events();
    renderer->clear();

    glm::mat4 view = camera->get_view_matrix();
    glm::mat4 projection = camera->get_projection_matrix();
    shader->use();
    shader->set_mat4("view", view);
    shader->set_mat4("projection", projection);
    shader->set_mat4("model", glm::mat4(1.0f));
    mesh->draw();

    std::cout << glm::to_string(view) << std::endl;

    renderer->swap();
  }

  return 0;
}
