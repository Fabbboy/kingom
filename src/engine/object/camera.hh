#ifndef KINGOM_ENGINE_CAMERA_HH
#define KINGOM_ENGINE_CAMERA_HH

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace kingom::engine {
class BaseCamera {
 public:
  BaseCamera() = default;
  virtual ~BaseCamera() = default;

  virtual glm::mat4 get_view_matrix() const = 0;
  virtual glm::mat4 get_projection_matrix() const = 0;

  virtual void set_position(const glm::vec3& position) = 0;
  virtual void set_rotation(const glm::vec3& rotation) = 0;
  virtual void set_fov(float fov) = 0;
  virtual void set_aspect_ratio(float aspect_ratio) = 0;
  virtual void set_near_clip(float near_clip) = 0;
  virtual void set_far_clip(float far_clip) = 0;

  virtual glm::vec3 get_position() const = 0;
  virtual glm::vec3 get_rotation() const = 0;
  virtual float get_fov() const = 0;
  virtual float get_aspect_ratio() const = 0;
  virtual float get_near_clip() const = 0;
  virtual float get_far_clip() const = 0;

  virtual void move(const glm::vec3& offset) = 0;
  virtual void rotate(const glm::vec3& offset) = 0;

  virtual void update(float deltaTime) = 0;
};

namespace base {
class OrthographicCamera : public BaseCamera {
 private:
  glm::vec3 position;
  glm::vec3 rotation;
  float fov;
  float aspect_ratio;
  float near_clip;
  float far_clip;

  glm::mat4 view_matrix;
  glm::mat4 projection_matrix;

 public:
  OrthographicCamera(float left, float right, float bottom, float top,
                     float near_clip, float far_clip)
      : position(glm::vec3(0.0f, 0.0f, 0.0f)),
        rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
        fov(45.0f),
        aspect_ratio(1.0f),
        near_clip(near_clip),
        far_clip(far_clip) {
    projection_matrix =
        glm::ortho(left, right, bottom, top, near_clip, far_clip);
  }

  glm::mat4 get_view_matrix() const override {
    return glm::translate(glm::mat4(1.0f), position) *
           glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x),
                       glm::vec3(1.0f, 0.0f, 0.0f)) *
           glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y),
                       glm::vec3(0.0f, 1.0f, 0.0f)) *
           glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z),
                       glm::vec3(0.0f, 0.0f, 1.0f));
  }

  glm::mat4 get_projection_matrix() const override { return projection_matrix; }

  void set_position(const glm::vec3& position) override {
    this->position = position;
  }
  void set_rotation(const glm::vec3& rotation) override {
    this->rotation = rotation;
  }
  void set_fov(float fov) override { this->fov = fov; }
  void set_aspect_ratio(float aspect_ratio) override {
    this->aspect_ratio = aspect_ratio;
  }
  void set_near_clip(float near_clip) override { this->near_clip = near_clip; }
  void set_far_clip(float far_clip) override { this->far_clip = far_clip; }

  glm::vec3 get_position() const override { return position; }
  glm::vec3 get_rotation() const override { return rotation; }
  float get_fov() const override { return fov; }
  float get_aspect_ratio() const override { return aspect_ratio; }
  float get_near_clip() const override { return near_clip; }
  float get_far_clip() const override { return far_clip; }

  // Implement the missing functions
  void move(const glm::vec3& offset) override { position += offset; }

  void rotate(const glm::vec3& offset) override { rotation += offset; }

  void update(float deltaTime) override {
    // If you want to implement any logic here, such as camera movement/rotation
    // updates
  }
};

}  // namespace base
}  // namespace kingom::engine

#endif