#ifndef KINGOM_ENGINE_SHADER_HH
#define KINGOM_ENGINE_SHADER_HH

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <string>

#include "util/result.hh"

namespace kingom::engine {
class Shader {
 private:
  unsigned int program_id;
  unsigned int vertex_id, fragment_id;

 private:
  util::Result<void, std::exception> compile_shader(unsigned int id,
                                                    std::string source);
  util::Result<void, std::exception> link_program();

  inline bool isActive(unsigned int id) { return glIsShader(id) == GL_TRUE; };

 public:
  Shader() = default;
  ~Shader();

  static util::Result<Shader, std::exception> create(
      std::string vertex_shader, std::string fragment_shader);

  static util::Result<Shader, std::exception> create(
      std::istream& vertex_shader, std::istream& fragment_shader);

  inline void use() { glUseProgram(program_id); }

  void set_bool(const std::string& name, bool value);
  void set_int(const std::string& name, int value);
  void set_float(const std::string& name, float value);
  void set_vec2(const std::string& name, const glm::vec2& value);
  void set_vec3(const std::string& name, const glm::vec3& value);
  void set_vec4(const std::string& name, const glm::vec4& value);
  void set_mat2(const std::string& name, const glm::mat2& value);
  void set_mat3(const std::string& name, const glm::mat3& value);
  void set_mat4(const std::string& name, const glm::mat4& value);

  inline unsigned int get_id() { return program_id; }
};
}  // namespace kingom::engine

#endif