#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>

#include "engine/buffers/buffer.hh"
#include "engine/buffers/layout.hh"
#include "engine/geometry/material.hh"
#include "engine/geometry/mesh.hh"
#include "engine/geometry/texture.hh"
#include "engine/rendering/shader.hh"
#include "engine/window/window.hh"
#include "engine/window/window_desc.hh"

using namespace kingom::engine;

class CustomMaterial : public BaseMaterial {
 public:
  CustomMaterial(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> tex)
      : shader(shader), tex(tex) {}

  void bind() override {
    shader->use();
    tex->bind(GL_TEXTURE0);
    shader->set_int("texture1", 0);
  }

  void unbind() override {}

 private:
  std::shared_ptr<Shader> shader;
  std::shared_ptr<Texture> tex;
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

  std::stringstream vertexShaderStream, fragmentShaderStream;
  vertexShaderStream << R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 TexCoord;

    void main() {
        gl_Position = vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }
  )";

  fragmentShaderStream << R"(
    #version 330 core
    out vec4 FragColor;

    in vec2 TexCoord;

    uniform sampler2D texture1;
    uniform float time;

    void main() {
      FragColor = texture(texture1, TexCoord);
    }
  )";

  auto ret_shader = kingom::engine::Shader::create(vertexShaderStream.str(),
                                                   fragmentShaderStream.str());
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

  layout.attach(std::make_unique<VertexBuffer>(vertex_buffer),
                std::make_unique<IndexBuffer>(index_buffer));

  auto res = layout.build();
  if (res.is_err()) {
    std::cerr << "Failed to build layout: " << res.unwrap_err().what()
              << std::endl;
    return -1;
  }

  std::unique_ptr<BaseMaterial> material =
      std::make_unique<CustomMaterial>(std::move(shader), std::move(tex));
  auto mesh = Mesh(layout, std::move(material));

  while (!window->should_close()) {
    window->poll_events();
    renderer->clear();

    mesh.draw();

    renderer->swap();
  }

  return 0;
}
