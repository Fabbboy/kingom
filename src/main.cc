#include <iostream>
#include <sstream>

#include "engine/object/texture.hh"
#include "engine/rendering/shader.hh"
#include "engine/window/window.hh"
#include "engine/window/window_desc.hh"

static void glfwErrorCallback(int error, const char* description) {
  std::cerr << "GLFW Error: " << description << std::endl;
}

int main() {
  glfwSetErrorCallback(glfwErrorCallback);
  auto result =
      kingom::engine::WindowDesc().set_width(800).set_height(600).build();

  if (result.is_err()) {
    std::cerr << "Failed to create window: " << result.unwrap_err().what()
              << std::endl;
    return -1;
  }

  auto window = result.unwrap();
  auto renderer = window->get_renderer();

  std::stringstream vertexShader(
      R"(
    #version 460 core
    layout (location = 0) in vec3 aPos;       
    layout (location = 1) in vec2 aTexCoord;  

    out vec2 TexCoord;  // Pass texture coordinates to fragment shader

    void main() {
      gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
      TexCoord = aTexCoord;  // Pass the texture coordinates
    }
  )");

  std::stringstream fragmentShader(
      R"(
    #version 460 core
    out vec4 FragColor;

    in vec2 TexCoord; 

    uniform sampler2D texture1;

    void main() {
      FragColor = texture(texture1, TexCoord);
    }
  )");

  auto shader = kingom::engine::Shader::create(vertexShader, fragmentShader);

  auto tex = kingom::engine::Texture::create("../assets/codeScreen.png");
  //auto tex = kingom::engine::Texture::create(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

  auto verticies = std::vector<float>{
      0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // top right
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f, 0.0f, 1.0f   // top left
  };

  auto indicies = std::vector<unsigned int>{
      0, 1, 3,  // first triangle
      1, 2, 3   // second triangle
  };

  // create buffer
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float),
               verticies.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int),
               indicies.data(), GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  if (shader.is_err()) {
    std::cerr << "Failed to create shader: " << shader.unwrap_err().what()
              << std::endl;
    return -1;
  }

  while (!window->should_close()) {
    window->poll_events();
    renderer->clear();

    // Activate the texture unit 0
    glActiveTexture(GL_TEXTURE0);

    // Bind the texture
    tex.unwrap().bind();

    // Set the sampler uniform to use texture unit 0
    shader.unwrap().use();
    shader.unwrap().set_int("texture1", tex.unwrap().get_id());

    // Now draw the object
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    renderer->swap();
  }

  return 0;
}
