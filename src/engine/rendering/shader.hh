#ifndef KINGOM_ENGINE_SHADER_HH
#define KINGOM_ENGINE_SHADER_HH

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

#include "engine/memory.hh"
#include "util/result.hh"

namespace kingom::engine {
class Shader;

struct ShaderPipeline {
  std::unordered_map<std::string, bool> bools;
  std::unordered_map<std::string, int> ints;
  std::unordered_map<std::string, float> floats;
  std::unordered_map<std::string, glm::vec2> vec2s;
  std::unordered_map<std::string, glm::vec3> vec3s;
  std::unordered_map<std::string, glm::vec4> vec4s;
  std::unordered_map<std::string, glm::mat2> mat2s;
  std::unordered_map<std::string, glm::mat3> mat3s;
  std::unordered_map<std::string, glm::mat4> mat4s;

  void apply(Ref<Shader> shader);
};

class Shader {
 private:
  unsigned int program_id;
  unsigned int vertex_id, fragment_id;
  ShaderPipeline pipeline;

 private:
  util::Result<void, std::exception> compile_shader(unsigned int id,
                                                    std::string source);
  util::Result<void, std::exception> link_program();

  inline bool isActive(unsigned int id) { return glIsShader(id) == GL_TRUE; };

 public:
  Shader() = default;
  ~Shader();

  static util::Result<Ref<Shader>, std::exception> create(
      std::string vertex_shader, std::string fragment_shader);

  static util::Result<Ref<Shader>, std::exception> create(
      std::istream& vertex_shader, std::istream& fragment_shader);

  inline void use() { glUseProgram(program_id); }
  inline void unuse() { glUseProgram(0); }

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
  inline ShaderPipeline& get_pipeline() { return pipeline; }
};
typedef Ref<Shader> ShaderPtr;
}  // namespace kingom::engine

#endif
