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

  virtual glm::vec3 get_position() const = 0;
  virtual glm::vec3 get_rotation() const = 0;

  virtual void move(const glm::vec3& offset) = 0;
  virtual void rotate(const glm::vec3& offset) = 0;
};

namespace base {
class OrthographicCamera : public BaseCamera {
 private:
  glm::vec3 position;
  glm::vec3 rotation;
  float near_clip;
  float far_clip;

  glm::mat4 view_matrix;
  glm::mat4 projection_matrix;

  void update_view_matrix();
  void update_projection_matrix(float left, float right, float bottom,
                                float top);

 public:
  OrthographicCamera(float left, float right, float bottom, float top,
                     float near_clip, float far_clip);

  inline glm::mat4 get_view_matrix() const override { return view_matrix; }
  inline glm::mat4 get_projection_matrix() const override {
    return projection_matrix;
  }

  void set_position(const glm::vec3& new_position) override;
  void set_rotation(const glm::vec3& new_rotation) override;

  void set_near_clip(float new_near_clip);
  void set_far_clip(float new_far_clip);

  glm::vec3 get_position() const override { return position; }
  glm::vec3 get_rotation() const override { return rotation; }
  float get_near_clip() const { return near_clip; }
  float get_far_clip() const { return far_clip; }

  void move(const glm::vec3& offset) override;
  void rotate(const glm::vec3& offset) override;
};

class PerspectiveCamera : public BaseCamera {
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
  PerspectiveCamera(float fov, float aspect_ratio, float near_clip,
                    float far_clip);

  glm::mat4 get_view_matrix() const override { return view_matrix; }
  glm::mat4 get_projection_matrix() const override { return projection_matrix; }

  void set_position(const glm::vec3& new_position) override;
  void set_rotation(const glm::vec3& new_rotation) override;

  void set_fov(float new_fov);
  void set_aspect_ratio(float new_aspect_ratio);
  void set_near_clip(float new_near_clip);
  void set_far_clip(float new_far_clip);

  glm::vec3 get_position() const override { return position; }
  glm::vec3 get_rotation() const override { return rotation; }
  float get_fov() const { return fov; }
  float get_aspect_ratio() const { return aspect_ratio; }
  float get_near_clip() const { return near_clip; }
  float get_far_clip() const { return far_clip; }

  void move(const glm::vec3& offset) override;
  void rotate(const glm::vec3& offset) override;

 private:
  void update_view_matrix();
  void update_projection_matrix();
};
}  // namespace base
}  // namespace kingom::engine

#endif