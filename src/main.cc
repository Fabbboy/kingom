#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "engine/object/texture.hh"
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
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

  auto window = result.unwrap();
  window->activate();
  auto renderer = window->get_renderer();
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cerr
        << "OpenGL Error (After setting current and getting renderer object): "
        << error << std::endl;
  }

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

    void main() {
       // FragColor = texture(texture1, TexCoord);
       //calculate a random value not time dependent
        float randomValue = fract(sin(dot(gl_FragCoord.xy, vec2(12.9898,78.233))) * 43758.5453);
        //apply cosine with the random value to the texture
        FragColor = texture(texture1, TexCoord) * vec4(randomValue, randomValue, randomValue, 1.0);

    }
  )";

  // auto ret_shader = kingom::engine::Shader::create(vertexShaderStream.str(),
  //                                                  fragmentShaderStream.str());
  // if (ret_shader.is_err()) {
  //   std::cerr << "Failed to create shader: " <<
  //   ret_shader.unwrap_err().what()
  //             << std::endl;
  //   return -1;
  // }

  // auto shader = ret_shader.unwrap();
  unsigned int vert, frag, prog;
  vert = glCreateShader(GL_VERTEX_SHADER);
  frag = glCreateShader(GL_FRAGMENT_SHADER);
  prog = glCreateProgram();

  error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cerr << "OpenGL Error (After creating shader programs): " << error
              << std::endl;
  }
  std::string vertexShaderSource = vertexShaderStream.str();
  const char* vertexShaderSourceCStr = vertexShaderSource.c_str();

  glShaderSource(vert, 1, &vertexShaderSourceCStr, NULL);
  glCompileShader(vert);

  error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cerr << "OpenGL Error (After compiling vert): " << error << std::endl;
  }

  int success;
  char infoLog[512];
  glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vert, 512, NULL, infoLog);
    std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
    return -1;
  }

  std::string fragmentShaderSource = fragmentShaderStream.str();
  const char* fragmentShaderSourceCStr = fragmentShaderSource.c_str();

  glShaderSource(frag, 1, &fragmentShaderSourceCStr, NULL);
  glCompileShader(frag);

  error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cerr << "OpenGL Error (After compiling frag): " << error << std::endl;
  }

  glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(frag, 512, NULL, infoLog);
    std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
    return -1;
  }

  glAttachShader(prog, vert);
  glAttachShader(prog, frag);
  glLinkProgram(prog);

  glGetProgramiv(prog, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(prog, 512, NULL, infoLog);
    std::cerr << "Shader program linking failed: " << infoLog << std::endl;
    return -1;
  }
  error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cerr << "OpenGL Error (After linking shaders): " << error << std::endl;
  }

  glDeleteShader(vert);
  glDeleteShader(frag);
  error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cerr << "OpenGL Error (After deleting shaders): " << error
              << std::endl;
  }

  // auto tex = kingom::engine::Texture::create(
  //     glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));  // Red color texture
  // // if (tex.is_err()) {
  //   std::cerr << "Failed to create texture: " << tex.unwrap_err().what()
  //             << std::endl;
  //   return -1;
  // }
  // glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
  // std::vector<unsigned char> image_data = {
  //     static_cast<unsigned char>(color.r * 255),
  //     static_cast<unsigned char>(color.g * 255),
  //     static_cast<unsigned char>(color.b * 255),
  //     static_cast<unsigned char>(color.a * 255)};
  // unsigned int id;
  // glGenTextures(1, &id);
  // if (id == 0) {
  //   std::cerr << "Error: Failed to generate a valid texture ID." <<
  //   std::endl;
  // }

  // error = glGetError();
  // if (error != GL_NO_ERROR) {
  //   std::cerr << "OpenGL Error (After glGenTextures): " << error <<
  //   std::endl;
  // }

  // glBindTexture(GL_TEXTURE_2D, id);
  // check_gl_errors("After glBindTexture");

  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  // check_gl_errors("After glTexParameteri GL_TEXTURE_WRAP_S");

  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // check_gl_errors("After glTexParameteri GL_TEXTURE_WRAP_T");

  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // check_gl_errors("After glTexParameteri GL_TEXTURE_MIN_FILTER");

  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // check_gl_errors("After glTexParameteri GL_TEXTURE_MAG_FILTER");

  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
  //              image_data.data());
  // check_gl_errors("After glTexImage2D");

  // glGenerateMipmap(GL_TEXTURE_2D);
  // glBindTexture(GL_TEXTURE_2D, 0);

  auto tex_ret =
      kingom::engine::Texture::create(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  if (tex_ret.is_err()) {
    std::cerr << "Failed to create texture: " << tex_ret.unwrap_err().what()
              << std::endl;
    return -1;
  }
  auto tex = tex_ret.unwrap();

  // Vertices with texture coordinates
  float vertices[] = {
      // positions       // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // top right
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
      -0.5f, 0.5f,  0.0f, 0.0f, 1.0f   // top left
  };

  unsigned int indices[] = {
      0, 1, 3,  // first triangle
      1, 2, 3   // second triangle
  };

  // Create buffer objects
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Texture coordinate attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  check_gl_errors("Initial Setup");

  while (!window->should_close()) {
    window->poll_events();
    renderer->clear();

    glUseProgram(prog);  // Make sure the shader program is active
    glActiveTexture(GL_TEXTURE0);  // Activate the texture unit first before
                                   // binding texture
    glBindTexture(GL_TEXTURE_2D, tex.get_id());  // Bind the texture

    // Set the uniform to use texture unit 0 in the shader
    GLint textureUniformLocation = glGetUniformLocation(prog, "texture1");
    if (textureUniformLocation == -1) {
      std::cerr << "Uniform 'texture1' not found in shader program"
                << std::endl;
    } else {
      glUniform1i(textureUniformLocation,
                  0);  // Tell shader to use texture unit 0
    }

    glBindVertexArray(VAO);                               // Bind the VAO
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // Draw call

    tex.unbind();  // Unbind the texture

    renderer->swap();  // Swap the buffers for display
  }

  return 0;
}
