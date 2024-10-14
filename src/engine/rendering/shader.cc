#include "engine/rendering/shader.hh"

#include <istream>

namespace kingom::engine {

util::Result<void, std::exception> Shader::compile_shader(unsigned int id,
                                                          std::string source) {
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int success;
  char infoLog[512];
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id, 512, nullptr, infoLog);
    return util::Result<void, std::exception>::Err(std::runtime_error(
        "Failed to compile shader: " + std::string(infoLog)));
  }

  return util::Result<void, std::exception>::Ok();
};
util::Result<void, std::exception> Shader::link_program() {
  glAttachShader(program_id, vertex_id);
  glAttachShader(program_id, fragment_id);
  glLinkProgram(program_id);

  int success;
  char infoLog[512];
  glGetProgramiv(program_id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program_id, 512, nullptr, infoLog);
    return util::Result<void, std::exception>::Err(
        std::runtime_error("Failed to link program: " + std::string(infoLog)));
  }

  return util::Result<void, std::exception>::Ok();
};

Shader::~Shader() {
  if (isActive(vertex_id)) {
    glDeleteShader(vertex_id);
  }
  if (isActive(fragment_id)) {
    glDeleteShader(fragment_id);
  }
  if (glIsProgram(program_id) == GL_TRUE) {
    glDeleteProgram(program_id);
  }
}
util::Result<ShaderPtr, std::exception> Shader::create(
    std::string vertex_shader, std::string fragment_shader) {
  ShaderPtr shader = make_box<Shader>();
  shader->program_id = glCreateProgram();
  shader->vertex_id = glCreateShader(GL_VERTEX_SHADER);
  shader->fragment_id = glCreateShader(GL_FRAGMENT_SHADER);

  auto result = shader->compile_shader(shader->vertex_id, vertex_shader);
  if (result.is_err()) {
    return util::Result<ShaderPtr, std::exception>::Err(result.unwrap_err());
  }

  result = shader->compile_shader(shader->fragment_id, fragment_shader);
  if (result.is_err()) {
    return util::Result<ShaderPtr, std::exception>::Err(result.unwrap_err());
  }

  result = shader->link_program();
  if (result.is_err()) {
    return util::Result<ShaderPtr, std::exception>::Err(result.unwrap_err());
  }

  return util::Result<ShaderPtr, std::exception>::Ok(std::move(shader));
};

util::Result<ShaderPtr, std::exception> Shader::create(
    std::istream& vertex_shader, std::istream& fragment_shader) {
  std::string vertex_src((std::istreambuf_iterator<char>(vertex_shader)),
                         std::istreambuf_iterator<char>());
  std::string fragment_src((std::istreambuf_iterator<char>(fragment_shader)),
                           std::istreambuf_iterator<char>());

  return create(vertex_src, fragment_src);
};

void Shader::set_bool(const std::string& name, bool value) {
  glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int)value);
};
void Shader::set_int(const std::string& name, int value) {
  glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
};
void Shader::set_float(const std::string& name, float value) {
  glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
};
void Shader::set_vec2(const std::string& name, const glm::vec2& value) {
  glUniform2fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
};
void Shader::set_vec3(const std::string& name, const glm::vec3& value) {
  glUniform3fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
};
void Shader::set_vec4(const std::string& name, const glm::vec4& value) {
  glUniform4fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
};
void Shader::set_mat2(const std::string& name, const glm::mat2& value) {
  glUniformMatrix2fv(glGetUniformLocation(program_id, name.c_str()), 1,
                     GL_FALSE, &value[0][0]);
};
void Shader::set_mat3(const std::string& name, const glm::mat3& value) {
  glUniformMatrix3fv(glGetUniformLocation(program_id, name.c_str()), 1,
                     GL_FALSE, &value[0][0]);
};
void Shader::set_mat4(const std::string& name, const glm::mat4& value) {
  glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1,
                     GL_FALSE, &value[0][0]);
};

}  // namespace kingom::engine