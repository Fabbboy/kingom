#include "camera.hh"

namespace kingom::engine::base {
OrthographicCamera::OrthographicCamera(float left, float right, float bottom,
                                       float top, float near_clip,
                                       float far_clip)
    : position(0.0f, 0.0f, 0.0f),
      rotation(0.0f, 0.0f, 0.0f),
      near_clip(near_clip),
      far_clip(far_clip) {
  update_projection_matrix(left, right, bottom, top);
  update_view_matrix();
}

void OrthographicCamera::update_view_matrix() {
  glm::mat4 rotation_matrix = glm::mat4(1.0f);
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(rotation.y),
                                glm::vec3(0.0f, 1.0f, 0.0f));  // Yaw
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(rotation.x),
                                glm::vec3(1.0f, 0.0f, 0.0f));  // Pitch
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(rotation.z),
                                glm::vec3(0.0f, 0.0f, 1.0f));  // Roll

  glm::vec3 direction =
      glm::vec3(rotation_matrix * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
  glm::vec3 up = glm::vec3(rotation_matrix * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

  view_matrix = glm::lookAt(position, position + direction, up);
}

void OrthographicCamera::update_projection_matrix(float left, float right,
                                                  float bottom, float top) {
  projection_matrix = glm::ortho(left, right, bottom, top, near_clip, far_clip);
}

void OrthographicCamera::set_position(const glm::vec3& new_position) {
  position = new_position;
  update_view_matrix();
}

void OrthographicCamera::set_rotation(const glm::vec3& new_rotation) {
  rotation = new_rotation;
  update_view_matrix();
}

void OrthographicCamera::set_fov(float new_fov) {
  fov = new_fov;
  update_projection_matrix(-aspect_ratio * new_fov, aspect_ratio * new_fov,
                           -new_fov, new_fov);
}

void OrthographicCamera::set_aspect_ratio(float new_aspect_ratio) {
  aspect_ratio = new_aspect_ratio;
  update_projection_matrix(-aspect_ratio * fov, aspect_ratio * fov, -fov, fov);
}

void OrthographicCamera::set_near_clip(float new_near_clip) {
  near_clip = new_near_clip;
  update_projection_matrix(-aspect_ratio * fov, aspect_ratio * fov, -fov, fov);
}

void OrthographicCamera::set_far_clip(float new_far_clip) {
  far_clip = new_far_clip;
  update_projection_matrix(-aspect_ratio * fov, aspect_ratio * fov, -fov, fov);
}

void OrthographicCamera::move(const glm::vec3& offset) {
  position += offset;
  update_view_matrix();
}

void OrthographicCamera::rotate(const glm::vec3& offset) {
  rotation += offset;
  update_view_matrix();
}

PerspectiveCamera::PerspectiveCamera(float fov, float aspect_ratio,
                                     float near_clip, float far_clip)
    : position(0.0f, 0.0f, 0.0f),
      rotation(0.0f, 0.0f, 0.0f),
      fov(fov),
      aspect_ratio(aspect_ratio),
      near_clip(near_clip),
      far_clip(far_clip) {
  update_projection_matrix();
  update_view_matrix();
}
void PerspectiveCamera::set_position(const glm::vec3& new_position) {
  position = new_position;
  update_view_matrix();
}

void PerspectiveCamera::set_rotation(const glm::vec3& new_rotation) {
  rotation = new_rotation;
  update_view_matrix();
}

void PerspectiveCamera::set_fov(float new_fov) {
  fov = new_fov;
  update_projection_matrix();
}

void PerspectiveCamera::set_aspect_ratio(float new_aspect_ratio) {
  aspect_ratio = new_aspect_ratio;
  update_projection_matrix();
}

void PerspectiveCamera::set_near_clip(float new_near_clip) {
  near_clip = new_near_clip;
  update_projection_matrix();
}

void PerspectiveCamera::set_far_clip(float new_far_clip) {
  far_clip = new_far_clip;
  update_projection_matrix();
}

void PerspectiveCamera::move(const glm::vec3& offset) {
  position += offset;
  update_view_matrix();
}

void PerspectiveCamera::rotate(const glm::vec3& offset) {
  rotation += offset;
  update_view_matrix();
}

void PerspectiveCamera::update_view_matrix() {
  glm::mat4 rotation_matrix = glm::mat4(1.0f);
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(rotation.y),
                                glm::vec3(0.0f, 1.0f, 0.0f));  // Yaw
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(rotation.x),
                                glm::vec3(1.0f, 0.0f, 0.0f));  // Pitch
  rotation_matrix = glm::rotate(rotation_matrix, glm::radians(rotation.z),
                                glm::vec3(0.0f, 0.0f, 1.0f));  // Roll

  glm::vec3 direction =
      glm::vec3(rotation_matrix * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
  glm::vec3 up = glm::vec3(rotation_matrix * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

  view_matrix = glm::lookAt(position, position + direction, up);
}
void PerspectiveCamera::update_projection_matrix() {
  projection_matrix =
      glm::perspective(glm::radians(fov), aspect_ratio, near_clip, far_clip);
}
}  // namespace kingom::engine::base