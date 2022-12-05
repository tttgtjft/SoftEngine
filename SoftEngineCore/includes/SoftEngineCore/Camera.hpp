#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace SoftEngine {

	class Camera
	{
	public:
		enum class ProjectionMode
		{
			Perspective,
			Orthographic
		};

		Camera(const glm::vec3& position = { 0, 0, 0 },
			   const glm::vec3& rotation = { 0, 0, 0 },
		 	   const ProjectionMode projection_mode = ProjectionMode::Perspective);

		void set_position(const glm::vec3& position);
		void set_rotation(const glm::vec3& rotation);
		void set_position_rotation(const glm::vec3& position, const glm::vec3& rotation);
		void set_projection_mode(const ProjectionMode projection_mode);

		const glm::vec3 get_camera_position() const { return m_position; }
		const glm::vec3 get_camera_rotation() const { return m_rotation; }
		
		glm::mat4 get_view_matrix() const { return m_view_matrix; }
		glm::mat4 get_projection_matrix() const { return m_projection_matrix; }

		void move_forward(const float delta);
		void move_right(const float delta);
		void move_up(const float delta);

		void add_movement_rotation(const glm::vec3& movement_delta, const glm::vec3& rotation_delta);

	private:
		void update_view_matrix();
		void update_projection_matrix();

		glm::mat4 m_view_matrix;
		glm::mat4 m_projection_matrix;

		glm::vec3 m_position;
		glm::vec3 m_rotation; //x - roll, y - pitch, z - yaw

		static constexpr glm::vec3 s_world_up = { 0.0f, 0.0f, 1.0f };
		static constexpr glm::vec3 s_world_right = { 0.0f, -1.0f, 0.0f };
		static constexpr glm::vec3 s_world_forward = { 1.0f, 0.0f, 0.0f };

		glm::vec3 m_direction;
		glm::vec3 m_right;
		glm::vec3 m_up;

		ProjectionMode m_projection_mode;
	};

}