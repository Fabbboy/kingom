#include "engine/geometry/texture.hh"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "engine/stb_image.hh"
namespace kingom::engine {

util::Result<void, std::exception> Texture::load_image(std::string path,
                                                       bool flip) {
  stbi_set_flip_vertically_on_load(flip);
  int channels;
  unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  if (!data) {
    return util::Result<void, std::exception>::Err(
        std::runtime_error("Failed to load image"));
  }

  if (color_type == ColorType::INFER) {
    switch (channels) {
      case 1:
        color_type = ColorType::RED;
        break;
      case 3:
        color_type = ColorType::RGB;
        break;
      case 4:
        color_type = ColorType::RGBA;
        break;
      default:
        stbi_image_free(data);
        return util::Result<void, std::exception>::Err(
            std::runtime_error("Invalid number of channels"));
    }
  }

  image_data =
      std::vector<unsigned char>(data, data + width * height * channels);
  stbi_image_free(data);

  return util::Result<void, std::exception>::Ok();
}

void Texture::create_texture() {
  glGenTextures(1, &id);

  glBindTexture(static_cast<GLenum>(texture_type), id);

  glTexParameteri(static_cast<GLenum>(texture_type), GL_TEXTURE_WRAP_S,
                  static_cast<GLint>(wrapping));

  if (texture_type == TextureType::TEXTURE_2D) {
    glTexParameteri(static_cast<GLenum>(texture_type), GL_TEXTURE_WRAP_T,
                    static_cast<GLint>(wrapping));
  }

  if (texture_type == TextureType::TEXTURE_3D) {
    throw std::runtime_error("3D textures not supported yet");
  }

  glTexParameteri(static_cast<GLenum>(texture_type), GL_TEXTURE_MIN_FILTER,
                  static_cast<GLint>(filter));

  glTexParameteri(static_cast<GLenum>(texture_type), GL_TEXTURE_MAG_FILTER,
                  static_cast<GLint>(filter));

  switch (texture_type) {
    case TextureType::TEXTURE_1D:
      glTexImage1D(static_cast<GLenum>(texture_type), 0,
                   static_cast<GLint>(color_type), width, 0,
                   static_cast<GLenum>(color_type), GL_UNSIGNED_BYTE,
                   image_data.data());
      break;
    case TextureType::TEXTURE_2D:
      glTexImage2D(static_cast<GLenum>(texture_type), 0,
                   static_cast<GLint>(color_type), width, height, 0,
                   static_cast<GLenum>(color_type), GL_UNSIGNED_BYTE,
                   image_data.data());

      break;

    case TextureType::TEXTURE_3D:
      break;
  }

  glGenerateMipmap(static_cast<GLenum>(texture_type));
  glBindTexture(static_cast<GLenum>(texture_type), 0);
}

util::Result<TexturePtr, std::exception> Texture::create(
    std::string path, bool flip, ColorType color_type, TextureType texture_type,
    TextureWrapping wrapping, TextureFilter filter) {
  TexturePtr texture = std::make_shared<Texture>();
  texture->color_type = color_type;
  texture->texture_type = texture_type;
  texture->wrapping = wrapping;
  texture->filter = filter;

  auto result = texture->load_image(path, flip);
  if (result.is_err()) {
    return util::Result<TexturePtr, std::exception>::Err(
        result.unwrap_err());
  }

  texture->create_texture();

  return util::Result<TexturePtr, std::exception>::Ok(texture);
}

util::Result<TexturePtr, std::exception> Texture::create(
    glm::vec4 color, TextureWrapping wrapping, TextureFilter filter) {
  TexturePtr texture = std::make_shared<Texture>();
  texture->color_type = ColorType::RGBA;
  texture->texture_type = TextureType::TEXTURE_2D;
  texture->wrapping = wrapping;
  texture->filter = filter;
  texture->width = 1;
  texture->height = 1;

  texture->image_data = {static_cast<unsigned char>(color.r * 255),
                         static_cast<unsigned char>(color.g * 255),
                         static_cast<unsigned char>(color.b * 255),
                         static_cast<unsigned char>(color.a * 255)};

  texture->create_texture();

  // ensure there are no errors
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cout << "OpenGL Error: " << error << std::endl;
    std::exit(1);
  }

  return util::Result<TexturePtr, std::exception>::Ok(texture);
}

Texture::~Texture() { glDeleteTextures(1, &id); }

void Texture::bind(GLenum textureUnit) {
  glActiveTexture(textureUnit);
  glBindTexture(static_cast<GLenum>(texture_type), id);
}

void Texture::unbind() { glBindTexture(static_cast<GLenum>(texture_type), 0); }
}  // namespace kingom::engine
