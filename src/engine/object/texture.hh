#ifndef KINGOM_ENGINE_TEXTURE_HH
#define KINGOM_ENGINE_TEXTURE_HH

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include "util/result.hh"
#include <vector>

namespace kingom::engine {
enum class ColorType {
  INFER = GL_NONE,
  RGB = GL_RGB,
  RGBA = GL_RGBA,
  RED = GL_RED,
};

enum class TextureType {
  TEXTURE_1D = GL_TEXTURE_1D,
  TEXTURE_2D = GL_TEXTURE_2D,
  TEXTURE_3D = GL_TEXTURE_3D,
};

enum class TextureWrapping {
  REPEAT = GL_REPEAT,
  MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
  CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
  CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
};

enum class TextureFilter {
  NEAREST = GL_NEAREST,
  LINEAR = GL_LINEAR,
  NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
  LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
  NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
  LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
};

class Texture {
 private:
  GLuint id;
  int width, height;
  std::vector<unsigned char> image_data;
  ColorType color_type;
  TextureType texture_type;
  TextureWrapping wrapping;
  TextureFilter filter;

 private:
  util::Result<void, std::exception> load_image(std::string path);
  void create_texture();

 public:
  Texture() = default;
  ~Texture();

  static util::Result<Texture, std::exception> create(std::string path,
                                                      ColorType color_type = ColorType::INFER,
                                                      TextureType texture_type = TextureType::TEXTURE_2D,
                                                      TextureWrapping wrapping = TextureWrapping::REPEAT,
                                                      TextureFilter filter = TextureFilter::LINEAR);

  static util::Result<Texture, std::exception> create(glm::vec4 color,
                                                      TextureWrapping wrapping = TextureWrapping::REPEAT,
                                                      TextureFilter filter = TextureFilter::LINEAR);
  inline GLuint get_id() { return id; }

  inline void bind() { glBindTexture(static_cast<GLenum>(texture_type), id); }
  inline void unbind() { glBindTexture(static_cast<GLenum>(texture_type), 0); }
};
}  // namespace kingom::engine

#endif