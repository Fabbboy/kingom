#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>

#include "engine/buffers/buffer.hh"
#include "engine/buffers/layout.hh"
#include "engine/geometry/texture.hh"
#include "engine/rendering/shader.hh"
#include "engine/window/window.hh"
#include "engine/window/window_desc.hh"

static void glfwErrorCallback(int error, const char* description) {
  std::cerr << "GLFW Error: " << description << std::endl;
}

void check_gl_errors(const std::string& msg) {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    std::cerr << "OpenGL Error (" << msg << "): " << err << std::endl;
  }
}

int main() {
  auto result = kingom::engine::WindowDesc()
                    .set_width(800)
                    .set_height(600)

                    .build();
  glfwSetErrorCallback(glfwErrorCallback);

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

  layout.add_buffers(
      std::make_shared<kingom::engine::VertexBuffer>(vertex_buffer),
      std::make_shared<kingom::engine::IndexBuffer>(index_buffer));

  auto res = layout.build();
  if (res.is_err()) {
    std::cerr << "Failed to build layout: " << res.unwrap_err().what()
              << std::endl;
    return -1;
  }

  // what currently is:
  //  glVertexAttribPointer(0, 4, 5126, , 32, 0000000000000000)
  // glEnableVertexAttribArray(0);
  // glVertexAttribPointer(1, 4, 5126, , 32, 0000000000000010)
  // glEnableVertexAttribArray(1);
  // what worked:
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
  // (void*)0); glEnableVertexAttribArray(0); glVertexAttribPointer(1, 2,
  // GL_FLOAT, GL_FALSE, 5 * sizeof(float),
  //                       (void*)(3 * sizeof(float)));
  // glEnableVertexAttribArray(1);

  GLenum error;
  error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cerr << "OpenGL Error: " << error << std::endl;
    return -1;
  }

  while (!window->should_close()) {
    window->poll_events();
    renderer->clear();

    shader->use();
    tex->bind(GL_TEXTURE0);
    shader->set_int("texture1", 0);

    layout.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    renderer->swap();
  }

  return 0;
}
